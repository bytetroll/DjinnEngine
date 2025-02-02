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
 on 5/8/2018.
//

#include <Data/UIML/Semantic.h>
#include <Data/UIML/AST.h>
#include <Data/UIML/Diagnostic.h>
#include <Data/UIML/ParserBlob.h>
#include <Data/UIML/SemanticBlob.h>

DJINN_NS3(Data, UIML);

COM::Result Semantic::Initialize(Memory::Context *context, Diagnostic *diag) {
    this->context = context;
    this->diag = diag;

    // OK
    return COM::kOK;
}

COM::Result Semantic::Compile(ParserBlob *blob, SemanticBlob *out) {
    this->out = out;

    // Add body
    SemanticScope scope;
    scope.Add(&out->content.body);

    // Passthrough
    if (COM::CheckedResult result = Visit(blob->node, scope)) {
        return result;
    }

    // OK
    return COM::kOK;
}

COM::Result Semantic::Visit(Node *node, SemanticScope scope) {
    if (!node) {
        return COM::kOK;
    }

    switch (node->GetClassID()) {
        case BodyNode::kCID: {
            return Visit(static_cast<BodyNode *>(node), scope);
        }
        case StyleNode::kCID: {
            return Visit(static_cast<StyleNode *>(node), scope);
        }
        case ElementNode::kCID: {
            return Visit(static_cast<ElementNode *>(node), scope);
        }
        case ComponentNode::kCID: {
            return Visit(static_cast<ComponentNode *>(node), scope);
        }
        case StyleElementNode::kCID: {
            return Visit(static_cast<StyleElementNode *>(node), scope);
        }
        case StyleAttributeNode::kCID: {
            return Visit(static_cast<StyleAttributeNode *>(node), scope);
        }
        case StyleUsingNode::kCID: {
            return Visit(static_cast<StyleUsingNode *>(node), scope);
        }
        case ValueNode::kCID: {
            return Visit(static_cast<ValueNode *>(node), scope);
        }
        case RGBANode::kCID: {
            return Visit(static_cast<RGBANode *>(node), scope);
        }
        case ArrayNode::kCID: {
            return Visit(static_cast<ArrayNode *>(node), scope);
        }
        case GroupNode::kCID: {
            return Visit(static_cast<GroupNode *>(node), scope);
        }
        case CategoryNode::kCID: {
            return Visit(static_cast<CategoryNode *>(node), scope);
        }
        case DataNode::kCID: {
            return Visit(static_cast<DataNode *>(node), scope);
        }
        case BindNode::kCID: {
            return Visit(static_cast<BindNode *>(node), scope);
        }
    }

    // Shouldn't happen
    diag->Add(Severity::eError, node->GetSpan(), "Internal error");
    return COM::kError;
}

COM::Result Semantic::Visit(BodyNode *node, SemanticScope scope) {
    COM::Result result = COM::kOK;
    for (auto child : node->nodes) {
        if (COM::CheckedResult checked = Visit(child, scope)) {
            result = checked;
        }
    }
    return result;
}

COM::Result Semantic::Visit(StyleNode *node, SemanticScope scope) {
    if (COM::CheckedResult result = Visit(node->body, scope)) {
        return result;
    }
    return COM::kOK;
}

COM::Result Semantic::Visit(GroupNode *node, SemanticScope scope) {
    // Declaration or insertion?
    if (node->body) {
        CompiledGroup group;
        scope.Add(&group);
        scope.Add(&group.body);

        if (COM::CheckedResult result = Visit(node->body, scope)) {
            return result;
        }

        // Visit properties
        for (auto &property : node->properties) {
            if (COM::CheckedResult result{COM::kOK}; (result = Visit(property.id, scope)) || (result = Visit(property.value, scope))) {
                return result;
            }

            if (!property.id->value.IsOf<Core::String>()) {
                diag->Add(Severity::eError, property.id->GetSpan(), "element property must be of an id type");
                return COM::kError;
            }

            switch (property.id->value.Get<Core::String>().Hash()) {
                case "name"_id: {
                    if (!property.value->value.IsOf<Core::String>()) {
                        diag->Add(Severity::eError, property.id->GetSpan(), "element property must be of an id type");
                        return COM::kError;
                    }

                    group.name = property.id->value.Get<Core::String>();
                    group.id = property.id->value.Get<Core::String>().Hash();
                    break;
                }
                default: {
                    diag->Add(Severity::eError, property.id->GetSpan(), "unknown group property");
                    return COM::kError;
                }
            }
        }


        // OK
        out->content.groups.Add(group);
        return COM::kOK;
    } else {
        auto compiled = scope.Get<CompiledBody>();

        CompiledElement element;
        element.type = CompiledElementType::eGroup;
        element.relativeIndex = compiled->elements.Size();

        if (auto parent = scope.Get<CompiledElement>()) {
            element.relativeParent = parent->relativeIndex;
        }

        // Visit properties
        for (auto &property : node->properties) {
            if (COM::CheckedResult result{COM::kOK}; (result = Visit(property.id, scope)) || (result = Visit(property.value, scope))) {
                return result;
            }

            if (!property.id->value.IsOf<Core::String>()) {
                diag->Add(Severity::eError, property.id->GetSpan(), "element property must be of an id type");
                return COM::kError;
            }

            switch (property.id->value.Get<Core::String>().Hash()) {
                case "name"_id: {
                    if (!property.value->value.IsOf<Core::String>()) {
                        diag->Add(Severity::eError, property.id->GetSpan(), "element property must be of an id type");
                        return COM::kError;
                    }

                    element.group.name = property.id->value.Get<Core::String>();
                    break;
                }
                default: {
                    diag->Add(Severity::eError, property.id->GetSpan(), "unknown group property");
                    return COM::kError;
                }
            }
        }

        // OK
        compiled->elements.Add(element);
        return COM::kOK;
    }
}

COM::Result Semantic::Visit(CategoryNode *node, SemanticScope scope) {
    auto compiled = scope.Get<CompiledBody>();

    CompiledElement element;
    element.type = CompiledElementType::eCategory;
    element.relativeIndex = compiled->elements.Size();

    if (auto parent = scope.Get<CompiledElement>()) {
        element.relativeParent = parent->relativeIndex;
    }

    // Visit properties
    for (auto &property : node->properties) {
        if (COM::CheckedResult result{COM::kOK}; (result = Visit(property.id, scope)) || (result = Visit(property.value, scope))) {
            return result;
        }

        if (!property.id->value.IsOf<Core::String>()) {
            diag->Add(Severity::eError, property.id->GetSpan(), "element property must be of an id type");
            return COM::kError;
        }

        if (!property.value->value.IsOf<Core::String>()) {
            diag->Add(Severity::eError, property.id->GetSpan(), "element property value must be of an id type");
            return COM::kError;
        }

        switch (property.id->value.Get<Core::String>().Hash()) {
            case "name"_id: {
                element.category.name = property.value->value.Get<Core::String>();
                break;
            }
            default: {
                diag->Add(Severity::eError, property.id->GetSpan(), "unknown category property");
                return COM::kError;
            }
        }
    }

    // OK
    compiled->elements.Add(element);
    return COM::kOK;
}

COM::Result Semantic::Visit(DataNode *node, SemanticScope scope) {
    auto compiled = scope.Get<CompiledBody>();

    return COM::kOK;
}

COM::Result Semantic::Visit(ElementNode *node, SemanticScope scope) {
    if (COM::CheckedResult result = Visit(node->id, scope)) {
        return result;
    }

    auto body = scope.Get<CompiledBody>();

    CompiledElement element;
    element.type = CompiledElementType::eGeneric;
    element.relativeIndex = body->elements.Size();

    if (!node->id->value.IsOf<Core::String>()) {
        diag->Add(Severity::eError, node->id->GetSpan(), "element type must be of an id type");
        return COM::kError;
    }
    element.generic.id = node->id->value.Get<Core::String>();

    if (auto parent = scope.Get<CompiledElement>()) {
        element.relativeParent = parent->relativeIndex;
    }

    scope.Add(&element);

    // Visit properties
    for (auto &property : node->properties) {
        if (COM::CheckedResult result{COM::kOK}; (result = Visit(property.id, scope)) || (result = Visit(property.value, scope))) {
            return result;
        }

        if (!property.id->value.IsOf<Core::String>()) {
            diag->Add(Severity::eError, property.id->GetSpan(), "element property must be of an id type");
            return COM::kError;
        }

        if (!property.value->value.IsOf<Core::String>()) {
            diag->Add(Severity::eError, property.id->GetSpan(), "element property value must be of an id type");
            return COM::kError;
        }

        switch (property.id->value.Get<Core::String>().Hash()) {
            case "name"_id: {
                element.generic.name = property.value->value.Get<Core::String>();
                continue;
            }
            case "class"_id: {
                element.generic.classes = property.value->value.Get<Core::String>();
                continue;
            }
        }

        element.generic.properties.Add({property.id->value.Get<Core::String>(), property.value->value.Get<Core::String>()});
    }

    body->elements.Add(element);

    if (COM::CheckedResult result = Visit(node->body, scope)) {
        return result;
    }

    // OK
    return COM::kOK;
}

COM::Result Semantic::Visit(ComponentNode *node, SemanticScope scope) {
    scope.Remove<CompiledElement>();

    // OK
    return COM::kOK;
}

COM::Result Semantic::Visit(StyleElementNode *node, SemanticScope scope) {
    CompiledStyle style;
    for (auto bind : node->binds) {
        CompiledHeader compiled;
        compiled.bindMode = bind.bindMode;
        compiled.mode = bind.mode;
        compiled.name = bind.name;
        style.headers.Add(compiled);
    }

    scope.Add(&style);
    if (COM::CheckedResult result = Visit(node->body, scope)) {
        return result;
    }

    out->content.styles.Add(style);
    return COM::kOK;
}

COM::Result Semantic::Visit(StyleAttributeNode *node, SemanticScope scope) {
    auto compiled = scope.Get<CompiledStyle>();

    if (COM::CheckedResult result = Visit(node->value, scope)) {
        return result;
    }

    StyleAttribute attribute;
    attribute.value = node->value->value;
    attribute.id = Core::ComputeCRC64(node->name.AsCharArray());
    compiled->attributes.Add(attribute);

    return COM::kOK;
}

COM::Result Semantic::Visit(StyleUsingNode *node, SemanticScope scope) {
    auto compiled = scope.Get<CompiledStyle>();

    CompiledHeader header;
    header.bindMode = node->bind.bindMode;
    header.mode = node->bind.mode;
    header.name = node->bind.name;
    compiled->dependencies.Add(header);

    return COM::kOK;
}

COM::Result Semantic::Visit(ValueNode *node, SemanticScope scope) {
    switch (node->tok.type) {
        case kTTChar: {
            node->value = static_cast<float>(node->tok.type);
            return COM::kOK;
        }
        case kTTDouble: {
            node->value = static_cast<float>(node->tok.value.fp64);
            return COM::kOK;
        }
        case kTTFloat: {
            node->value = static_cast<float>(node->tok.value.fp32);
            return COM::kOK;
        }
        case kTTInt64: {
            node->value = static_cast<float>(node->tok.value.i64);
            return COM::kOK;
        }
        default: {
            node->value = Core::String(node->tok.value.chp);
            return COM::kOK;
        }
    }

    // Shouldn't happen
    diag->Add(Severity::eError, node->GetSpan(), "Internal error");
    return COM::kError;
}

COM::Result Semantic::Visit(BindNode *node, SemanticScope scope) {
    USize mode;
    switch (node->mode.type) {
        case kTTNone: {
            mode = 0;
            break;
        }
        case kTTString:
        case kTTID: {
            mode = Core::ComputeCRC64(node->mode.value.chp);
            break;
        }
        default: {
            diag->Add(Severity::eError, node->GetSpan(), "Invalid binding mode");
            return COM::kError;
        }
    }

    switch (node->tok.type) {
        case kTTString:
        case kTTID: {
            node->value = Core::String(node->tok.value.chp);
            return COM::kOK;
        }
        default: {
            diag->Add(Severity::eError, node->GetSpan(), "Invalid binding");
            return COM::kError;
        }
    }
}

COM::Result Semantic::Visit(RGBANode *node, SemanticScope scope) {
    if (COM::CheckedResult result{COM::kOK}; (result = Visit(node->colors[0], scope)) || (result = Visit(node->colors[1], scope)) || (result = Visit(node->colors[2], scope)) ||
                                             (result = Visit(node->colors[3], scope))) {
        return result;
    }

    for (UInt32 i = 0; i < 4; i++) {
        if (node->colors[i] && !node->colors[i]->value.IsOf<float>()) {
            diag->Add(Severity::eError, node->colors[i]->GetSpan(), "Color component not a scalar");
            return COM::kError;
        }
    }

    node->value = Math::Vector4(
            node->colors[0] ? node->colors[0]->value.Get<float>() : 0,
            node->colors[1] ? node->colors[1]->value.Get<float>() : 0,
            node->colors[2] ? node->colors[2]->value.Get<float>() : 0,
            node->colors[3] ? node->colors[3]->value.Get<float>() : 1
    );

    return COM::kOK;
}

COM::Result Semantic::Visit(ArrayNode *node, SemanticScope scope) {
    for (auto child : node->nodes) {
        if (COM::CheckedResult result = Visit(child, scope)) {
            return result;
        }

        if (!child->value.IsOf<float>()) {
            diag->Add(Severity::eError, child->GetSpan(), "expected float");
            return COM::kError;
        }
    }

    switch (node->nodes.Size()) {
        case 2:
            node->value = Math::Vector2(node->nodes[0]->value.Get<float>(), node->nodes[1]->value.Get<float>());
            break;
        case 3:
            node->value = Math::Vector3(node->nodes[0]->value.Get<float>(), node->nodes[1]->value.Get<float>(), node->nodes[2]->value.Get<float>());
            break;
        case 4:
            node->value = Math::Vector4(node->nodes[0]->value.Get<float>(), node->nodes[1]->value.Get<float>(), node->nodes[2]->value.Get<float>(), node->nodes[3]->value.Get<float>());
            break;
    }

    return COM::kOK;
}
