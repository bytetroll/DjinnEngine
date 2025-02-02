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
 on 9/5/2018.
//
#pragma once

#include <Reflection/IAttribute.h>
#include <Core/Annotation.h>

namespace Djinn::Attributes::UIML {
    struct Class : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Class(const char *name) : name(name) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *name;
    };

    struct Setter : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Setter(const char *property) : property(property) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *property;
    };

    struct Getter : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Getter(const char *property) : property(property) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *property;
    };

    struct Event : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Event(const char *name) : name(name) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *name;
    };

    struct Property : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Property(const char *name) : name(name) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *name;
    };

    struct SocketAttach : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        SocketAttach(const char *name = nullptr) : name(name) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *name;
    };

    struct SocketDetach : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        SocketDetach(const char *name = nullptr) : name(name) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *name;
    };

    struct Bind : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Bind(const char *name = nullptr) : name(name) {
            /**/
        }

        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *name;
    };
}