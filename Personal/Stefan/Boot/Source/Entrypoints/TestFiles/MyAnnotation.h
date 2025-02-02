#pragma once

#include <COM/IUnknown.h>
#include <Core/Annotation.h>
#include <Reflection/IAttribute.h>

DJINN_NS();

// Custom attributes must be under Djinn::Attributes
namespace Djinn::Attributes {
    // This is our custom attribute
    // Can be applied anywhere
    struct MyAttribute : public Reflection::IAttribute {
        // A helper which creates a unique class id (uint64) of this type
        DJINN_COM_CLASSID();

        MyAttribute(const char *name) : name(name) {
            /**/
        }

        // All reflection attributes must be able to identify themselves
        COM::ClassID GetClassID() override {
            return kCID;
        }

        const char *name;
    };
}

ClassAttr(MyAttribute("NameIsA"))
struct MyStructA {
    int a{0};

    Attr(MyAttribute("Tohru"))
    int b{0};

    Attr(MyAttribute("Kanna"))
    int c{0};
};

ClassAttr(MyAttribute("NameIsB"))
struct MyStructB {
    Attr(MyAttribute("HS DxD"))
    int a{0};
};
