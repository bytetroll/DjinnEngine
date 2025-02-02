//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>

namespace NyLang::IL {
    class Emitter;
    class Container;

    class ContainerScope {
    public:
        ContainerScope(Emitter* emitter, Container* Container);
        ~ContainerScope();

    private:
        Emitter* emitter = nullptr;
        Container* previous = nullptr;
    };
}
