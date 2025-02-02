//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Native/Extensions/Resolve/SemanticResolve.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>

namespace NyLang {
    class UtilExtension;

    struct MatchResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // Operands
        Symbol* lhs = nullptr;
        Symbol* rhs = nullptr;

        // Resulting value
        Symbol* result = nullptr;
    };

    class MatchExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.MatchExtension");
        LANG_ACTIONID(Action, "Lang.MatchExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static MatchExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        MatchExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Resolve match
        /// \param context
        /// \param node
        /// \param scopes
        /// \param lhs
        /// \param rhs
        /// \return
        MatchResolve ResolveMatch(SemanticContext*context, Node *node, SemanticScopeStack scopes, const Token& tokLhs, const std::list<Symbol*>& lhs, const Token& tokRhs, const std::list<Symbol*>& rhs);

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
    };
}
