//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Symbol/SymbolPair.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>
#include <list>

namespace NyLang {
    struct SymbolNamed;
    struct SemanticContext;
    class Node;
    struct SymbolTypeInfo;
    class UtilExtension;

    class AutoExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.AutoExtension");
        LANG_ACTIONID(Action, "Lang.AutoExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static AutoExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        AutoExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;
        /// Instantiate auto type
        /// \param context
        /// \param instantiator
        /// \param autoType
        /// \param symbols
        /// \return
        SymbolPair <SymbolNamed> InstantiateAuto(SemanticContext*context, Node *instantiator, SemanticScopeStack scopes, SymbolTypeInfo*autoInfo, const std::list<Symbol*> &symbols);

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
    };
}
