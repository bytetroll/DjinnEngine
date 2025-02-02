//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 5/9/2018.
//

#include <UI/UIMLHost.h>
#include <Game/IWorld.h>
#include <Game/IActorSystem.h>
#include <Data/IUIMLAsset.h>
#include <Data/UIMLContent.h>
#include <Base/DefaultLogs.h>
#include <Core/StackArray.h>
#include <Game/IEntitySystem.h>
#include <Reflection/IReflectionRegistry.h>
#include <Reflection/IClassInfo.h>
#include <Reflection/IMethodInfo.h>
#include <Reflection/IMemberInfo.h>
#include <Reflection/IEnumInfo.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <UI/IControl.h>
#include <UI/UIMLAnnotation.h>
#include <UI/IUICanvas.h>


DJINN_NS2(UI);

UIMLHost::UIMLHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIMLHost(registry, outer, host) {

}

COM::Result UIMLHost::Initialize() {
    // Get reflection
    if (!registry->GetPipeline()->GetInterface(&reflectionRegistry)) {
        ErrorLog(this).Write("Expected reflection registry to be installed");
        return COM::kError;
    }

    // Get all uiml classes
    Check(reflectionRegistry->BindClassAttributeInstalled(Attributes::UIML::Class::kCID, [&](Reflection::IClassInfo *classInfo, Reflection::IAttribute *_attr) {
        Register(classInfo->GetReflectionID().AsCOMClassID(), static_cast<Attributes::UIML::Class*>(_attr)->name);
    }));

    // OK
    return COM::kOK;
}

void UIMLHost::Register(COM::ClassID cid, const Core::StringView &name) {
    Reflection::IClassInfo* classInfo;
    CheckMsgVoid(reflectionRegistry->GetClass(cid, &classInfo), Error, "Failed to get class info for ", name.ToString().Ptr());

    auto &compClass = nativeComponentClasses[cid] = {};

    for (auto member : classInfo->GetMembers()) {
        // Bind?
        if (auto attr = member->GetAttribute<Attributes::UIML::Bind>()) {
            compClass.binds.Add(Core::ComputeCRC64(attr->name ? attr->name : member->GetName()), [=](void* frame, void* value) {
                member->GetDynamic(frame).Set(value);
            });
        }
    }

    auto &comp = nativeComponents[Core::ComputeCRC64(name.Ptr(), name.End())] = {};

    comp.classID = cid;

    for (auto method : classInfo->GetMethods()) {
        // Setter?
        if (auto attr = method->GetAttribute<Attributes::UIML::Setter>()) {
            if (method->GetParameters().Size() == 1) {
                auto &param = method->GetParameters()[0];

                // Enum parameter?
                if (Reflection::IEnumInfo *_enum; reflectionRegistry->GetEnum(Reflection::ClassID(param.type.GetHash()), &_enum)) {
                    comp.setters.Add(Core::ComputeCRC64(attr->property), [=](void *frame, const Core::StringView &value) {
                        if (Int64 enumValue; _enum->ToIndex(value, enumValue)) {
                            method->GetDynamic(frame).InvokeUnsafe<int>(enumValue);
                        } else {
                            ErrorLog(this).Write("Could not convert '", value.ToString(), "' to enum type ", _enum->GetQualifiedName());
                        }
                    });
                }

                    // String view?
                else if (param.type.Is<const Core::StringView &>()) {
                    comp.setters.Add(Core::ComputeCRC64(attr->property), [=](void *frame, const Core::StringView &value) {
                        method->Get <void(const Core::StringView&)>(frame).Invoke(value);
                    });
                }

                    // Float?
                else if (param.type.Is<float>()) {
                    comp.setters.Add(Core::ComputeCRC64(attr->property), [=](void *frame, const Core::StringView &value) {
                        method->Get<void(float)>(frame).Invoke(static_cast<float>(atof(value.Ptr())));
                    });
                }

                    // Int32?
                else if (param.type.Is<Int32>()) {
                    comp.setters.Add(Core::ComputeCRC64(attr->property), [=](void *frame, const Core::StringView &value) {
                        method->Get<void(Int32)>(frame).Invoke(static_cast<Int32>(atoi(value.Ptr())));
                    });
                }

                    // ?
                else if (param.type.Is<bool>()) {
                    comp.setters.Add(Core::ComputeCRC64(attr->property), [=](void *frame, const Core::StringView &value) {
                        method->Get<void(bool)>(frame).Invoke(value == "true" || value == "True");
                    });
                }

                    // Unknown
                else {
                    ErrorLog(this).Write("Setter '", method->GetDecoratedSignature(), "' has an incompatible signature");
                }
            } else {
                ErrorLog(this).Write("Setter '", method->GetDecoratedSignature(), "' has an incompatible signature");
            }
        }

        // Event?
        if (auto attr = method->GetAttribute<Attributes::UIML::Event>()) {
            comp.events.Add(Core::ComputeCRC64(attr->name), [=](void *frame, Game::IActorSystemInstance *instance, const Core::DynamicDelegate &delegate) {
                if (auto dyn = method->Get < void(Game::IActorSystemInstance * , const Core::DynamicDelegate &)>(frame)) {
                dyn.Invoke(instance, delegate);
            } else {
                ErrorLog(this).Write("Event '", method->GetDecoratedSignature(), "' has an incompatible signature");
            }
            });
        }

        // Socket attach?
        if (auto attr = method->GetAttribute<Attributes::UIML::SocketAttach>()) {
            Socket socket;
            if (auto delegate = method->Get<void(IUITransformComponent*)>(nullptr)) {
                socket = [=](void *frame, IUITransformComponent *transform) {
                    method->GetDynamic(frame).Invoke(transform);
                };
            } else {
                socket = [=](void *frame, IUITransformComponent *transform) {
                    method->GetDynamic(frame).InvokeUnsafe(transform->GetElement());
                };
            }

            if (attr->name == nullptr) {
                comp.defaultSocketAttach = socket;
            } else {
                comp.socketAttachs.Add(Core::ComputeCRC64(attr->name), socket);
            }
        }

        // Socket detach?
        if (auto attr = method->GetAttribute<Attributes::UIML::SocketDetach>()) {
            auto socket = [=](void *frame, IUITransformComponent *transform) { method->GetDynamic(frame).Invoke(transform); };

            if (attr->name == nullptr) {
                comp.defaultSocketDetach = socket;
            } else {
                comp.socketDetachs.Add(Core::ComputeCRC64(attr->name), socket);
            }
        }
    }
}


COM::Result UIMLHost::Build(IUICanvas* canvas, Data::IUIMLAsset *asset, COM::IUnknown* model, const Core::Visitor<void(IUIElement*)>& visitor) {
    if (!asset) {
        return COM::kInvalidParameter;
    }

    // Async construction not supported
    Platform::LockGuard guard(mutex);

    // Get content
    auto content = asset->GetContent();

    // Inject style to canvas style
    {
        auto sharedStyle = canvas->GetStyle();

        for (auto &style : content->styles) {
            for (auto &header : style.headers) {
                StyleBinding binding;
                switch (header.bindMode) {
                    case Data::UIML::StyleBindMode::eClass:
                        binding = StyleBinding::eClass;
                        break;
                    case Data::UIML::StyleBindMode::eComponent:
                        binding = StyleBinding::eType;
                        break;
                    case Data::UIML::StyleBindMode::eName:
                        binding = StyleBinding::eID;
                        break;
                }

                auto _class = sharedStyle->GetOrCreateClass(binding, header.name.Hash(), Hash(header.mode));

                for (auto &attribute : style.attributes) {
                    _class->Add(StyleID(attribute.id), attribute.value);
                }
            }
        }
    }

    // Merge groups
    for (auto &group : content->groups) {
        auto &merged = groups[group.id];
        for (auto &element : group.body.elements) {
            merged.body.elements.Add(element);
        }
    }

    // Base meta
    Meta meta;
    meta.scene = canvas;

    // Get actor system
    CheckMsg(canvas->GetWorld()->GetPipeline()->GetInterface(&meta.system), Error, "Expected actor system to be installed");

    // Prepare base model
    if ((meta.model.instance = model)) {
        meta.model.nativeClass = nativeComponentClasses.GetNullable(model->GetClassID());

        // Attempt to find reflected class
        if (!reflectionRegistry->GetClass(Reflection::ClassID(model->GetClassID()), &meta.model.modelClassInfo)) {
            ErrorLog(this).Write("Missing reflection data from: '", model->GetClassName(), "'");
            return COM::kError;
        }
    }

    // ...
    if (COM::CheckedResult result = BuildBody(content->body, [=](IUITransformComponent *child) { visitor(child->GetElement()); }, meta)) {
        return result;
    }

    // OK
    return COM::kOK;
}

COM::Result UIMLHost::BuildBody(const Data::UIML::CompiledBody &body, const Core::Delegate<void(IUITransformComponent *)> &_parent, Meta meta) {
    Core::StackArray16<Core::Delegate<void(IUITransformComponent *)>> relativeParents;

    // Spawn all elements
    for (auto &element : body.elements) {
        // Determine actual parent
        auto parent = _parent;
        if (element.relativeParent.IsValid() && relativeParents.Size() > element.relativeParent.Get()) {
            parent = relativeParents[element.relativeParent.Get()];
        }

        // Instantiate
        switch (element.type) {
            case Data::UIML::CompiledElementType::eGeneric: {
                // Attempt to get native component
                NativeComponent *native;
                if (!(native = nativeComponents.GetNullable(element.generic.id.Hash()))) {
                    ErrorLog(this).Write("Failed to resolve native component '", element.generic.id, "'");
                    continue;
                }

                // Create component
                IUIElement *component;
                if (Game::IActor *actor; !meta.system->CreateActor(meta.scene, native->classID, &actor) || !(component = COM::Cast<IUIElement>(actor))) {
                    ErrorLog(this).Write("Failed to create component");
                    continue;
                }

                // Apply parent
                if (parent) {
                    parent.Invoke(component->GetTransform());
                }

                // Handle properties
                for (auto &property : element.generic.properties) {
                    // Setter?
                    if (Setter * setter = native->setters.GetNullable(property.id.Hash())) {
                        // Invoke setter
                        setter->Invoke(component, property.value);
                        continue;
                    }

                    // Event?
                    if (Event * event = native->events.GetNullable(property.id.Hash())) {
                        // Get method with name
                        Reflection::IMethodInfo *method;
                        if (!meta.model.modelClassInfo || !(method = meta.model.modelClassInfo->GetMethod(property.value.AsCharArray()))) {
                            ErrorLog(this).Write("Model '", meta.model.instance->GetClassName(), "' has no method '", property.value, "'");
                            continue;
                        }

                        // Invoke setter
                        event->Invoke(component, component, method->GetDynamic(meta.model.instance));
                        continue;
                    }

                    // ...
                    ErrorLog(this).Write("Unknown property or event '", property.id, "'");
                }

                // Attempt class based styling
                // TODO: Standardize this
                {
                    USize begin = 0;
                    while (begin != element.generic.classes.Length()) {
                        // Ignore whitespace
                        while (std::isspace(element.generic.classes[begin]) && begin < element.generic.classes.Length()) begin++;

                        // Get name
                        USize end = begin;
                        while (!std::isspace(element.generic.classes[end]) && end < element.generic.classes.Length()) end++;

                        // Add style
                        component->AddStyleClass(StyleClassID(Core::ComputeCRC64(&element.generic.classes[begin], &element.generic.classes[end])));

                        // ...
                        begin = end;
                    }
                }

                // Name?
                if (element.generic.name != "") {
                    component->SetName(element.generic.name);

                    if (meta.model.nativeClass) {
                        auto it = meta.model.nativeClass->binds.Find(Core::ComputeCRC64(element.generic.name.AsCharArray()));
                        if (it != meta.model.nativeClass->binds.End()) {
                            it->Invoke(meta.model.instance, component);
                        }
                    }
                }

                // Set relative transformation
                relativeParents.ResizePassive(element.relativeIndex + 1);
                relativeParents[element.relativeIndex] = [=](IUITransformComponent *transform) {
                    if (native->defaultSocketAttach) {
                        native->defaultSocketAttach.Invoke(component, transform);
                    } else {
                        ErrorLog(this).Write("Element '", component->GetName(), "' has no default socket to attach '", transform->GetElement()->GetName(), "'");
                    }
                };
                break;
            }
            case Data::UIML::CompiledElementType::eGroup: {
                // Find group
                auto group = groups.Find(element.group.name.Hash());
                if (group == groups.End()) {
                    ErrorLog(this).Write("Unknown group '", element.group.name, "'");
                    continue;
                }

                // Build body of group
                if (COM::CheckedResult result = BuildBody(group->body, parent, meta)) {
                    return result;
                }
                break;
            }
            default:
                break;
        }
    }

    // OK
    return COM::kOK;
}
