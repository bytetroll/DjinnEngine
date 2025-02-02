//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>
#include <functional>
#include <NyLang/Native/Nodes/NodeUnary.h>
#include <NyLang/Native/Extensions/Resolve/SemanticResolve.h>

namespace NyLang {
    class UtilExtension;
    class SymbolValueInfo;
    struct SemanticContext;

    struct UnaryResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // Value
        Symbol* value = nullptr;

        // Scope level
        int scopeLevel = -1;

        // Resulting value
        Symbol* result = nullptr;

        // ...
        bool isInitialization = false;
    };

    struct MultiUnaryResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // ...
        bool passed;

        // Candidates
        std::list<UnaryResolve> passedCandidates;
        std::list<UnaryResolve> ignoredCandidates;
        std::list<UnaryResolve> failedCandidates;
    };

    /// Delegates
    using UnaryOpDelegate = std::function<SymbolValueInfo*(SemanticContext*, Node *, const SemanticScopeStack&, const Token& op, const Token& valueTok, SymbolValueInfo* value)>;

    class UnaryExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.UnaryExtension");
        LANG_ACTIONID(Action, "Lang.UnaryExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static UnaryExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        UnaryExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Add a unary operation
        /// \param lhs
        /// \param rhs
        /// \param op
        void AddOp(const Vector<TokenID>& triggers, NodeUnaryPrecendance precedance, ClassID type, const UnaryOpDelegate& delegate);

        /// Resolve a binary operation
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        UnaryResolve ResolveUnary(
                SemanticContext*context, Node *node, SemanticScopeStack scopes,
                const Token& op, NodeUnaryPrecendance precendance, const SymbolWrapper& lhs
        );

        /// Resolve a binary operation from multiple
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        MultiUnaryResolve ResolveMultiUnary(
                SemanticContext*context, Node *node, SemanticScopeStack scopes,
                const Token& op, NodeUnaryPrecendance precendance, const Token& lhsTok, const std::list<Symbol*>& lhs
        );

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;

        struct UnaryOp {
            ClassID type;
            UnaryOpDelegate delegate;
        };
        std::multimap<TokenID::Type, UnaryOp> triggerOpsL;
        std::multimap<TokenID::Type, UnaryOp> triggerOpsR;
        Vector<UnaryOp> opsL;
        Vector<UnaryOp> opsR;
    };
}
