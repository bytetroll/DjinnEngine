//
// Created by Dev on 9/9/2018.
//

#include <UI/Style.h>
#include <UI/IUIElement.h>
#include <Base/DefaultLogs.h>
#include <Reflection/IReflectionRegistry.h>

DJINN_NS2(UI);

Style::Style(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

COM::Result Style::Initialize() {
    // Get reflection registry
    CheckMsg(registry->GetPipeline()->GetInterface(&reflectionRegistry), Error, "Expected reflection registry to be installed");

    // OK
    return COM::kOK;
}

StyleClass *Style::GetOrCreateClass(StyleBinding type, USize id, USize state) {
    auto hash = CombineHash(type, id, state);

    if (classes.Contains(hash)) {
        return classes[hash];
    }

    // Create class
    auto _class = classPool.PopConstruct(type, id, state);

    // ...
    classes.Add(hash, _class);

    // OK
    return _class;
}

ElementStyle Style::CompileStyle(IUIElement *element) {
    ElementStyle style;
    style.registry = registry;

    // Hash
    USize crc = element->GetName().Hash();

    // Stateless type
    if (auto _class = classes.GetOrDefault(CombineHash(StyleBinding::eType, element->GetClassID().Get(), 0), nullptr)) {
        for (auto&& value : _class->GetValues()) {
            style.values.Add(value);
        }
    }

    // State type
    if (auto _class = classes.GetOrDefault(CombineHash(StyleBinding::eType, element->GetClassID().Get(), element->GetStyleState()), nullptr)) {
        for (auto&& value : _class->GetValues()) {
            style.values.Add(value);
        }
    }

    // Stateless id
    if (auto _class = classes.GetOrDefault(CombineHash(StyleBinding::eID, crc, 0), nullptr)) {
        for (auto&& value : _class->GetValues()) {
            style.values.Add(value);
        }
    }

    // State id
    if (auto _class = classes.GetOrDefault(CombineHash(StyleBinding::eID, crc, element->GetStyleState()), nullptr)) {
        for (auto&& value : _class->GetValues()) {
            style.values.Add(value);
        }
    }

    // Classes
    for (auto styleClass : element->GetStyleClasses()) {
        // Stateless class
        if (auto _class = classes.GetOrDefault(CombineHash(StyleBinding::eClass, styleClass, 0), nullptr)) {
            for (auto&& value : _class->GetValues()) {
                style.values.Add(value);
            }
        }

        // State class
        else if (auto _class = classes.GetOrDefault(CombineHash(StyleBinding::eClass, styleClass, element->GetStyleState()), nullptr)) {
            for (auto&& value : _class->GetValues()) {
                style.values.Add(value);
            }
        }

        // Unknown
        else {
            ErrorLog(this).Write("Could not find style class '", styleClass.GetString("[hash]"), "' for element '", element->GetName(), "'");
        }
    }

    return style;
}

