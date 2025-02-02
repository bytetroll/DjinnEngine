//
// Created by Dev on 9/5/2018.
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