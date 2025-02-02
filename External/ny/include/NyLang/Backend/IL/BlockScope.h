//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>

namespace NyLang::IL {
    class Emitter;
    class Block;

    class BlockScope {
    public:
        BlockScope(Emitter* emitter, Block* block);
        ~BlockScope();

    private:
        Emitter* emitter = nullptr;
        Block* previous = nullptr;
    };
}
