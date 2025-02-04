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
 on 5/6/2018.
//
#pragma once

#include <COM/ID.h>
#include <Core/Array.h>
#include <Core/StackArray.h>
#include "Token.h"
#include "StyleBind.h"
#include "StyleMode.h"
#include "StyleAttribute.h"

namespace Djinn::Data::UIML {
    class Node {
    public:
        Node(const Span &span, const COM::ClassID &cid) : cid(cid), span(span) {

        }

        COM::ClassID GetClassID() const {
            return cid;
        }

        Span GetSpan() const {
            return span;
        }

        StyleValue value;

    private:
        Span span;
        COM::ClassID cid;
    };

    class BodyNode : public Node {
    public:
        DJINN_COM_CLASSID();

        BodyNode(const Span &span, const Core::Array<Node *> &nodes) : Node(span, kCID), nodes(nodes) {

        }

        Core::Array<Node *> nodes;
    };

    class ArrayNode : public Node {
    public:
        DJINN_COM_CLASSID();

        ArrayNode(const Span &span, const Core::StackArray4<Node *> &nodes) : Node(span, kCID), nodes(nodes) {

        }

        Core::StackArray4<Node *> nodes;
    };

    class StyleNode : public Node {
    public:
        DJINN_COM_CLASSID();

        StyleNode(const Span &span, Node *body) : Node(span, kCID), body(body) {

        }

        Node *body;
    };

    struct ElementProperty {
        Node *id;
        Node *value;
    };

    class ElementNode : public Node {
    public:
        DJINN_COM_CLASSID();

        ElementNode(const Span &span, Node *id, Node *body, const Core::StackArray4<ElementProperty> &properties) : Node(span, kCID), id(id), body(body), properties(properties) {

        }

        Node *id;
        Core::StackArray4<ElementProperty> properties;
        Node *body;
    };

    class ComponentNode : public Node {
    public:
        DJINN_COM_CLASSID();

        ComponentNode(const Span &span, const Core::String &id, Node *body, const Core::StackArray4<ElementProperty> &properties) : Node(span, kCID), id(id), body(body), properties(properties) {

        }

        Core::String id;
        Core::StackArray4<ElementProperty> properties;
        Node *body;
    };

    class GroupNode : public Node {
    public:
        DJINN_COM_CLASSID();

        GroupNode(const Span &span, Node *body, const Core::StackArray4<ElementProperty> &properties) : Node(span, kCID), body(body), properties(properties) {

        }

        Core::StackArray4<ElementProperty> properties;
        Node *body;
    };

    class ModelNode : public Node {
    public:
        DJINN_COM_CLASSID();

        ModelNode(const Span &span, Node *body, const Core::StackArray4<ElementProperty> &properties) : Node(span, kCID), body(body), properties(properties) {

        }

        Core::StackArray4<ElementProperty> properties;
        Node *body;
    };

    class CategoryNode : public Node {
    public:
        DJINN_COM_CLASSID();

        CategoryNode(const Span &span, const Core::StackArray4<ElementProperty> &properties) : Node(span, kCID), properties(properties) {

        }

        Core::StackArray4<ElementProperty> properties;
    };

    class DataNode : public Node {
    public:
        DJINN_COM_CLASSID();

        DataNode(const Span &span, const Core::StackArray4<ElementProperty> &properties) : Node(span, kCID), properties(properties) {

        }

        Core::StackArray4<ElementProperty> properties;
    };

    class StyleElementNode : public Node {
    public:
        DJINN_COM_CLASSID();

        StyleElementNode(const Span &span, const Core::StackArray4<StyleBind> &binds, Node *body) : Node(span, kCID), binds(binds), body(body) {

        }

        Core::StackArray4<StyleBind> binds;
        Node *body;
    };

    class StyleAttributeNode : public Node {
    public:
        DJINN_COM_CLASSID();

        StyleAttributeNode(const Span &span, const Core::String &name, Node *value) : Node(span, kCID), name(name), value(value) {

        }

        Node *value;
        Core::String name;
    };

    class StyleUsingNode : public Node {
    public:
        DJINN_COM_CLASSID();

        StyleUsingNode(const Span &span, const StyleBind &bind) : Node(span, kCID), bind(bind) {

        }

        StyleBind bind;
        StyleMode mode;
    };

    class ValueNode : public Node {
    public:
        DJINN_COM_CLASSID();

        ValueNode(const Token &tok) : Node(tok.span, kCID), tok(tok) {

        }

        Token tok;
    };

    class BindNode : public Node {
    public:
        DJINN_COM_CLASSID();

        BindNode(const Token& mode, const Token &tok) : Node(tok.span, kCID), mode(mode), tok(tok) {

        }

        Token mode;
        Token tok;
    };

    class RGBANode : public Node {
    public:
        DJINN_COM_CLASSID();

        RGBANode(const Span &span, Node *r, Node *g, Node *b, Node *a) : Node(span, kCID) {
            colors[0] = r;
            colors[1] = g;
            colors[2] = b;
            colors[3] = a;
        }

        Node *colors[4];
    };
}