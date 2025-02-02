//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Native/Extensions/Resolve/SemanticResolve.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>

namespace NyLang {
    class UtilExtension;
    class CastExtension;
    class SymbolValueInfo;

    struct CallResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // ...
        bool passed;
        bool isStatic;

        // Target symbol
        Symbol* symbol = nullptr;

        // Resolved delegate
        SymbolValueInfo* delegate = nullptr;

        // Casted arguments
        std::list<Symbol*> arguments;

        // Resulting value
        Symbol* value = nullptr;
    };


    struct MultiCallResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // Candidates
        std::list<CallResolve> passedCandidates;
        std::list<CallResolve> failedCandidates;
    };

    class CallExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.CallExtension");
        LANG_ACTIONID(Action, "Lang.CallExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static CallExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        CallExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Resolve a call
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        CallResolve ResolveCall(
                SemanticContext*context, Node *node, SemanticScopeStack scopes,
                const Token& tok, Symbol* symbol, const std::list<std::list<SymbolWrapper>>& arguments
        );

        /// Resolve a call
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        MultiCallResolve ResolveMultiCall(
                SemanticContext*context, Node *node, SemanticScopeStack scopes,
                const Token& tok, const std::list<Symbol*>& symbols, const std::list<std::list<SymbolWrapper>>& arguments
        );

        /// Expand arguments
        /// \param symbols
        /// \return
        std::list<std::list<SymbolWrapper>> ExpandArguments(SemanticContext*context, Node *node, SemanticScopeStack scopes, const std::list<std::list<SymbolWrapper>>& symbols);

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
        CastExtension* castExtension = nullptr;
    };
}
