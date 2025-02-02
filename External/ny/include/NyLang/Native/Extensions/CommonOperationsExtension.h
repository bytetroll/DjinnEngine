//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>
#include <list>

namespace NyLang {
    class Symbol;
    struct SemanticContext;
    class Node;
    struct SymbolTypeInfo;
    class SymbolValueInfo;
    struct SymbolNamed;
    class BinaryExtension;
    class UnaryExtension;
    class SymbolType;
    class UtilExtension;
    class CastExtension;

    class CommonOperationsExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.CommonOperationsExtension");

        /// Construct and install instance into registry
        /// \return
        static CommonOperationsExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        CommonOperationsExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
        BinaryExtension* binaryExtension = nullptr;
        UnaryExtension* unaryExtension = nullptr;
        CastExtension* castExtension = nullptr;
    };
}
