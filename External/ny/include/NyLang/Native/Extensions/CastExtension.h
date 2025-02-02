//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Native/Extensions/Resolve/SemanticResolve.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>
#include <functional>

namespace NyLang {
    class UtilExtension;
    class SymbolValueInfo;
    struct SymbolTypeInfo;

    enum class CastType {
        eImplicit,
        eExplicit,
        eMutate
    };

    struct CastResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // From to
        Symbol* from = nullptr;
        SymbolTypeInfo* to = nullptr;

        // New value
        // - Null on invalid cast
        Symbol* value = nullptr;
    };

    struct MultiCastResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // Attempts
        std::list<CastResolve> passedCasts;
        std::list<CastResolve> failedCasts;
    };

    /// Delegates
    using CastOpDelegate = std::function<Symbol*(SemanticContext*, Node *, const SemanticScopeStack&, CastType castType, const Token& tok, SymbolValueInfo* from, SymbolTypeInfo* to)>;

    class CastExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.CastExtension");
        LANG_ACTIONID(Action, "Lang.CastExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static CastExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        CastExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Resolve a cast
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        CastResolve ResolveCast(
                SemanticContext*context, Node *node, SemanticScopeStack scopes, CastType castType,
                const SymbolWrapper& from, SymbolTypeInfo* to, bool isInitialization
        );

        /// Resolve a multi cast
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        MultiCastResolve ResolveMultiCast(
                SemanticContext*context, Node *node, SemanticScopeStack scopes, CastType castType,
                const std::list<SymbolWrapper>& from, SymbolTypeInfo* to, bool isInitialization
        );

        /// Add a cast operation
        /// \param lhs
        /// \param rhs
        /// \param op
        void AddCastOp(ClassID from, ClassID to, const CastOpDelegate &delegate);

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
        Map<std::pair<ClassID::Type, ClassID::Type>, CastOpDelegate> castDelegates;
    };
}
