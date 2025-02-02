//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Semantic/ISemantic.h>
#include <NyLang/Native/Extensions/Resolve/SemanticResolve.h>

namespace NyLang {
    class UtilExtension;
    class CastExtension;
    class NodeBinary;
    class SymbolValueInfo;
    struct SymbolTypeInfo;

    struct BinaryResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // Operands
        Symbol* lhs = nullptr;
        Symbol* rhs = nullptr;

        // Scope level
        int scopeLevel = -1;

        // Resulting value
        Symbol* result = nullptr;

        // ...
        bool isInitialization = false;
    };

    struct MultiBinaryResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // ...
        bool passed;

        // Candidates
        std::list<BinaryResolve> passedCandidates;
        std::list<BinaryResolve> ignoredCandidates;
        std::list<BinaryResolve> failedCandidates;
    };

    /// Delegates
    using BinaryOpDelegate = std::function<SymbolValueInfo*(SemanticContext*, Node *, const SemanticScopeStack&, const Token& op, const Token& lhsTok, SymbolValueInfo* lhs, const Token& rhsTok, SymbolValueInfo* rhs)>;

    class BinaryExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.BinaryExtension");
        LANG_ACTIONID(Action, "Lang.BinaryExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static BinaryExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        BinaryExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Add a binary operation
        /// \param lhs
        /// \param rhs
        /// \param op
        void AddOp(const Vector<TokenID>& triggers, ClassID lhs, ClassID rhs, const BinaryOpDelegate& delegate);

        /// Resolve a binary operation
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        BinaryResolve ResolveBinary(
                SemanticContext*context, Node *node, SemanticScopeStack scopes,
                const Token& op, const SymbolWrapper& lhs, const SymbolWrapper& rhs
        );

        /// Resolve a binary operation from multiple
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        MultiBinaryResolve ResolveMultiBinary(
                SemanticContext*context, Node *node, SemanticScopeStack scopes,
                const Token& op, const Token& lhsTok, const std::list<Symbol*>& lhs, const Token& rhsTok, const std::list<Symbol*>& rhs
        );

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
        CastExtension* castExtension = nullptr;

        struct BinaryOp {
            ClassID lhsType;
            ClassID rhsType;
            BinaryOpDelegate delegate;
        };
        std::multimap<TokenID::Type, BinaryOp> triggerOps;
        Vector<BinaryOp> ops;
    };
}
