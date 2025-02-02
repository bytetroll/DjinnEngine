//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/Shared.h>
#include <NyLang/Core/StdTypes.h>

namespace NyLang::IL {
    class IValue;
    class Block;
    class Container;

    class Emitter {
    public:
        /// Set current block
        /// \param block
        void SetCurrentBlock(Block* block) {
            this->block = block;
        }

        /// Get current block
        Block* GetCurrentBlock() {
            return block;
        }

        /// Set current container
        /// \param block
        void SetCurrentContainer(Container* container) {
            this->container = container;
        }

        /// Get current container
        Container* GetCurrentContainer() {
            return container;
        }

    private:
        // Current block
        Block* block = nullptr;

        // Current container
        Container* container = nullptr;
    };
}
