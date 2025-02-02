#pragma once

#include <COM/IUnknown.h>
#include <Core/String.h>

DJINN_NS();

class MyClass : public COM::IUnknown {
public:
    DJINN_COM_CLASS();

    // "Just use the base constructor"
    using IUnknown::IUnknown;

    // My mom always told me I was special as a kid
    Core::String VerySpecialFunction() {
        return GetClassName();
    }
};
