//
// Created by Dev on 5/5/2018.
//
#include <Data/UIML/Parser.h>
#include <Data/UIML/TokenBlob.h>
#include <Core/Context.h>
#include <Data/UIML/AST.h>
#include <Data/UIML/ParserBlob.h>
#include <Data/UIML/Diagnostic.h>
#include <Core/StackArray.h>

DJINN_NS3(Data, UIML);

COM::Result Parser::Initialize(Memory::Context *context, Diagnostic *diag) {
    this->context = context;
    this->diag = diag;

    // OK
    return COM::kOK;
}

COM::Result Parser::Compile(TokenBlob *blob, ParserBlob *out) {
    if (blob->tokens.Size() == 0) {
        diag->Add(Severity::eError, {}, "Unexpected end");
        return COM::kError;
    }

    // Prepare snapshot
    Snapshot snapshot;
    snapshot.it = &blob->tokens[0];
    snapshot.end = &blob->tokens[blob->tokens.Size() - 1];

    // Parse as body
    if (COM::CheckedResult result = ParseBody(snapshot, &out->node)) {
        return result;
    }

    // Must be end
    if (!snapshot.IsEnd()) {
        diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end");
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result Parser::ParseBody(Snapshot &snapshot, Node **out) {
    auto begin = snapshot.Tok().span;

    // While not end
    Core::Array<Node *> nodes;
    while (!snapshot.IsEnd() && !(snapshot.Is("<"_id) && snapshot.PeekIs("/"_id))) {
        if (snapshot.Is("<"_id) && snapshot.PeekIs("none"_id)) {
            snapshot.Next(2);

            if (!snapshot.Is(">"_id)) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected '>'");
                return COM::kError;
            }
            snapshot.Next();

            Node *element;
            if (COM::CheckedResult result = ParseBody(snapshot, &element)) {
                return result;
            }

            nodes.Add(element);

            // Expecting close
            if (!(snapshot.Is("<"_id) && snapshot.PeekIs("/"_id) && snapshot.PeekIs("none"_id, 2) && snapshot.PeekIs(">"_id, 3))) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of previous container");
                return COM::kError;
            }
            snapshot.Next(4);
        } else {
            Node *element;
            if (COM::CheckedResult result = ParseElement(snapshot, &element)) {
                return result;
            }

            nodes.Add(element);
        }
    }

    // OK
    *out = context->New(BodyNode(Span::Range(begin, snapshot.Tok().span), nodes));
    return COM::kOK;
}

COM::Result Parser::ParseElement(Snapshot &snapshot, Node **out) {
    auto begin = snapshot.Tok().span;

    if (!snapshot.Is("<"_id)) {
        switch (snapshot.TokID()) {
            case kTTString:
            case kTTChar:
            case kTTDouble:
            case kTTFloat:
            case kTTInt64: {
                *out = context->New(ValueNode(snapshot.Tok()));
                snapshot.Next();
                return COM::kOK;
            }
            default: {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected value element", "Did you mean a string? Strings must be encapsulated with \"abc\"");
                return COM::kError;
            }
        }
    }
    snapshot.Next();

    // Fetch id
    Token tid = snapshot.Tok();
    Node *id = nullptr;
    switch (tid.type) {
        case "style"_id:
        case "component"_id:
        case "category"_id:
        case "data"_id:
        case "group"_id: {
            snapshot.Next();
            break;
        }
        default: {
            if (snapshot.Is(kTTID)) {
                id = context->New(ValueNode(snapshot.Tok()));
                snapshot.Next();
            } else {
                if (COM::CheckedResult result = ParseValue(snapshot, &id)) {
                    return result;
                }
            }
        }
    }

    // Parse properties
    Core::StackArray4<ElementProperty> properties;
    while (!snapshot.Is(">"_id) && !(snapshot.Is("/"_id) && snapshot.PeekIs(">"_id))) {
        switch (snapshot.TokID()) {
            case kTTID: {
                ElementProperty property;

                // Parse id value
                if (snapshot.Is(kTTID)) {
                    property.id = context->New(ValueNode(snapshot.Tok()));
                    snapshot.Next();
                } else {
                    if (COM::CheckedResult result = ParseValue(snapshot, &property.id)) {
                        return result;
                    }
                }

                // Expecting =
                if (!snapshot.Is("="_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected '=' on property");
                    return COM::kError;
                }
                snapshot.Next();

                // Parse value
                if (COM::CheckedResult result = ParseValue(snapshot, &property.value)) {
                    return result;
                }

                // ...
                properties.Add(property);
                break;
            }
            default: {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Unknown property type");
                return COM::kError;
            }
        }
    }

    // Style?
    switch (tid.type) {
        case "style"_id: {
            if (!snapshot.Is(">"_id)) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Empty style containers not supported");
                return COM::kError;
            }
            snapshot.Next();

            // Parse body
            Node *body;
            if (COM::CheckedResult result = ParseStyleBody(snapshot, &body)) {
                return result;
            }

            // Create node
            *out = context->New(StyleNode(Span::Range(begin, snapshot.Tok().span), body));

            // Expecting close
            if (!(snapshot.Is("<"_id) && snapshot.PeekIs("/"_id) && snapshot.PeekIs("style"_id, 2) && snapshot.PeekIs(">"_id, 3))) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of previous style container");
                return COM::kError;
            }
            snapshot.Next(4);
            break;
        }
        case "component"_id: {
            if (snapshot.Is("/"_id)) {
                snapshot.Next();

                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Create node
                *out = context->New(ComponentNode(Span::Range(begin, snapshot.Tok().span), tid.value.chp, nullptr, properties));
            } else {
                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Parse body
                Node *body;
                if (COM::CheckedResult result = ParseBody(snapshot, &body)) {
                    return result;
                }

                // Create node
                *out = context->New(ComponentNode(Span::Range(begin, snapshot.Tok().span), tid.value.chp, body, properties));

                // Expecting close
                if (!(snapshot.Is("<"_id) && snapshot.PeekIs("/"_id) && snapshot.PeekIs("component"_id, 2) && snapshot.PeekIs(">"_id, 3))) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of previous component container");
                    return COM::kError;
                }
                snapshot.Next(4);
            }
            break;
        }
        case "group"_id: {
            if (snapshot.Is("/"_id)) {
                snapshot.Next();

                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Create node
                *out = context->New(GroupNode(Span::Range(begin, snapshot.Tok().span), nullptr, properties));
            } else {
                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Parse body
                Node *body;
                if (COM::CheckedResult result = ParseBody(snapshot, &body)) {
                    return result;
                }

                // Create node
                *out = context->New(GroupNode(Span::Range(begin, snapshot.Tok().span), body, properties));

                // Expecting close
                if (!(snapshot.Is("<"_id) && snapshot.PeekIs("/"_id) && snapshot.PeekIs("group"_id, 2) && snapshot.PeekIs(">"_id, 3))) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of previous component container");
                    return COM::kError;
                }
                snapshot.Next(4);
            }
            break;
        }
        case "category"_id: {
            if (snapshot.Is("/"_id)) {
                snapshot.Next();

                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Create node
                *out = context->New(CategoryNode(Span::Range(begin, snapshot.Tok().span), properties));
            } else {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>', category does not support a body");
                return COM::kError;
            }
            break;
        }
        case "data"_id: {
            if (snapshot.Is("/"_id)) {
                snapshot.Next();

                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Create node
                *out = context->New(DataNode(Span::Range(begin, snapshot.Tok().span), properties));
            } else {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>', data does not support a body");
                return COM::kError;
            }
            break;
        }
        default: {
            if (snapshot.Is("/"_id)) {
                snapshot.Next();

                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Create node
                *out = context->New(ElementNode(Span::Range(begin, snapshot.Tok().span), id, nullptr, properties));
            } else {
                if (!snapshot.Is(">"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of element '>'");
                    return COM::kError;
                }
                snapshot.Next();

                // Parse body
                Node *body;
                if (COM::CheckedResult result = ParseBody(snapshot, &body)) {
                    return result;
                }

                // Create node
                *out = context->New(ElementNode(Span::Range(begin, snapshot.Tok().span), id, body, properties));

                // Expecting close
                if (!(snapshot.Is("<"_id) && snapshot.PeekIs("/"_id) && (snapshot.PeekIs(tid.type, 2) || snapshot.PeekIs("any"_id, 2)) && snapshot.PeekIs(">"_id, 3))) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of previous container");
                    return COM::kError;
                }
                snapshot.Next(4);
            }
            break;
        }
    }
    return COM::kOK;
}

COM::Result Parser::ParseStyleBody(Snapshot &snapshot, Node **out) {
    auto begin = snapshot.Tok().span;

    // While not end
    Core::Array<Node *> nodes;
    while (!snapshot.Is("<"_id)) {
        Node *element;
        if (COM::CheckedResult result = ParseStyleElement(snapshot, &element)) {
            return result;
        }

        nodes.Add(element);
    }

    // OK
    *out = context->New(BodyNode(Span::Range(begin, snapshot.Tok().span), nodes));
    return COM::kOK;
}

COM::Result Parser::ParseStyleElement(Snapshot &snapshot, Node **out) {
    auto begin = snapshot.Tok().span;

    Core::StackArray4<StyleBind> binds;
    if (COM::CheckedResult result = ParseHeaders(snapshot, binds)) {
        return result;
    }

    if (!snapshot.Is("{"_id)) {
        diag->Add(Severity::eError, snapshot.TokSpan(), "Expected '{' on a style element");
        return COM::kError;
    }
    snapshot.Next();

    auto bodyBegin = snapshot.Tok().span;

    Core::Array<Node *> elements;
    while (!snapshot.Is("}"_id)) {
        switch (snapshot.TokID()) {
            case kTTID: {
                auto begin = snapshot.Tok().span;

                Core::String name = snapshot.Tok().value.chp;
                snapshot.Next();

                if (snapshot.Is("-"_id)) {
                    snapshot.Next();

                    if (!snapshot.Is(kTTID)) {
                        diag->Add(Severity::eError, snapshot.TokSpan(), "Expected style attribute id");
                        return COM::kError;
                    }

                    name += "-";
                    name += snapshot.Tok().value.chp;
                    snapshot.Next();
                }

                name.ToLowercase();

                if (!snapshot.Is(":"_id)) {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected style attribute seperator ':'");
                    return COM::kError;
                }
                snapshot.Next();

                Core::StackArray4<Node *> values;
                do {
                    Node *value;
                    if (COM::CheckedResult result = ParseValue(snapshot, &value)) {
                        return result;
                    }

                    values.Add(value);
                } while (!snapshot.Is(";"_id));

                Node *value;
                if (values.Size() > 1) {
                    value = context->New(ArrayNode(Span::Range(values.First()->GetSpan(), values.Last()->GetSpan()), values));
                } else {
                    value = values.First();
                }

                elements.Add(context->New(StyleAttributeNode(Span::Range(begin, snapshot.Tok().span), name, value)));
                break;
            }
            case "using"_id: {
                snapshot.Next();

                StyleBind header;
                if (COM::CheckedResult result = ParseHeader(snapshot, header)) {
                    return result;
                }

                elements.Add(context->New(StyleUsingNode(Span::Range(begin, snapshot.Tok().span), header)));
                break;
            }
            default: {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected attribute");
                break;
            }
        }

        if (!snapshot.Is(";"_id)) {
            diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of style attribute ';'");
            return COM::kError;
        }
        snapshot.Next();
    }
    snapshot.Next();

    *out = context->New(StyleElementNode(
            Span::Range(begin, snapshot.Tok().span),
            binds,
            context->New(BodyNode(
                    Span::Range(bodyBegin, snapshot.Tok().span),
                    elements
            ))
    ));
    return COM::kOK;
}

COM::Result Parser::ParseValue(Parser::Snapshot &snapshot, Node **out) {
    switch (snapshot.TokID()) {
        case "-"_id: {
            snapshot.Next();

            auto tok = snapshot.Tok();

            switch (tok.type) {
                case kTTDouble: {
                    tok.value.fp64 *= -1;
                    break;
                }
                case kTTFloat: {
                    tok.value.fp32 *= -1;
                    break;
                }
                case kTTInt64: {
                    tok.value.i64 *= -1;
                    break;
                }
                default: {
                    diag->Add(Severity::eError, snapshot.TokSpan(), "Expected numeric value");
                    return COM::kError;
                }
            }
            snapshot.Next();

            *out = context->New(ValueNode(tok));
            return COM::kOK;
        }
        case "{"_id: {
            snapshot.Next();

            Token mode;
            Token value;
            if (snapshot.PeekIs("}"_id)) {
                value = snapshot.Tok();
                snapshot.Next();
            } else {
                mode = snapshot.Tok();
                snapshot.Next();
                value = snapshot.Tok();
                snapshot.Next();
            }

            if (!snapshot.Is("}"_id)) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected end of bind '}'");
                return COM::kError;
            }
            snapshot.Next();

            *out = context->New(BindNode(mode, value));
            return COM::kOK;
        }
        case kTTID:
        case kTTString:
        case kTTChar: {
            *out = context->New(ValueNode(snapshot.Tok()));
            snapshot.Next();
            return COM::kOK;
        }
        case kTTDouble:
        case kTTFloat:
        case kTTInt64: {
            auto tok = snapshot.Tok();
            snapshot.Next();
            *out = context->New(ValueNode(tok));
            return COM::kOK;
        }
        case "rgb"_id: {
            auto begin = snapshot.Tok().span;
            snapshot.Next();

            if (!snapshot.Is("("_id)) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected '('");
                return COM::kError;
            }
            snapshot.Next();

            Node *r;
            if (COM::CheckedResult result = ParseValue(snapshot, &r)) {
                return result;
            }

            if (!snapshot.Is(","_id)) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected ','");
                return COM::kError;
            }
            snapshot.Next();

            Node *g;
            if (COM::CheckedResult result = ParseValue(snapshot, &g)) {
                return result;
            }

            if (!snapshot.Is(","_id)) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected ','");
                return COM::kError;
            }
            snapshot.Next();

            Node *b;
            if (COM::CheckedResult result = ParseValue(snapshot, &b)) {
                return result;
            }

            if (!snapshot.Is(")"_id)) {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Expected ')'");
                return COM::kError;
            }
            snapshot.Next();

            *out = context->New(RGBANode(Span::Range(begin, snapshot.Tok().span), r, g, b, nullptr));
            return COM::kOK;
        }
        default: {
            diag->Add(Severity::eError, snapshot.TokSpan(), "Expected value or id", "Did you mean a string? Strings must be encapsulated with \"abc\"");
            return COM::kError;
        }
    }
}

COM::Result Parser::ParseHeaders(Parser::Snapshot &snapshot, Core::StackArray4<StyleBind> &out) {
    for (;;) {
        StyleBind bind;
        if (COM::CheckedResult result = ParseHeader(snapshot, bind)) {
            return result;
        }

        out.Add(bind);

        if (snapshot.Is(","_id)) {
            snapshot.Next();
        } else {
            break;
        }
    }

    return COM::kOK;
}

COM::Result Parser::ParseHeader(Parser::Snapshot &snapshot, StyleBind &out) {
    out.bindMode = StyleBindMode::eComponent;
    out.mode = StyleMode::eNone;

    switch (snapshot.TokID()) {
        case "."_id: {
            out.bindMode = StyleBindMode::eClass;
            snapshot.Next();
            break;
        }
        case kTTString: {
            out.bindMode = StyleBindMode::eName;
            break;
        }
    }

    switch (snapshot.TokID()) {
        case kTTString: {
            out.name = snapshot.Tok().value.chp;
            snapshot.Next();
            break;
        }
        case kTTID: {
            out.name = snapshot.Tok().value.chp;
            snapshot.Next();
            break;
        }
        default: {
            diag->Add(Severity::eError, snapshot.TokSpan(), "Expected style header id");
            return COM::kError;
        }
    }

    if (snapshot.Is(":"_id)) {
        snapshot.Next();
        switch (snapshot.TokID()) {
            case "hover"_id: {
                out.mode = StyleMode::eHover;
                snapshot.Next();
                break;
            }
            case "pressed"_id: {
                out.mode = StyleMode::ePressed;
                snapshot.Next();
                break;
            }
            default: {
                diag->Add(Severity::eError, snapshot.TokSpan(), "Unknown style mode");
                return COM::kError;
            }
        }
    }

    return COM::kOK;
}

