//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/PrettyStream.h>

namespace NyLang::IL {
    class Container;

    class Program {
    public:
        /// Pretty print
        /// \param stream
        void PrettyPrint(PrettyStream& stream);

        /// Get containers
        /// \return
        PtrVector <Container>& GetContainers() {
            return containers;
        }

        /// Add container
        /// \param container
        void AddContainer(Container* container) {
            containers.push_back(container);
        }

    private:
        PtrVector<Container> containers;
    };
}
