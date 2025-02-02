//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>
#include <list>
#include <NyLang/Native/Extensions/Resolve/SemanticResolve.h>
#include <functional>

namespace NyLang {
    class Symbol;
    struct SemanticContext;
    class Node;
    struct SymbolTypeInfo;
    class SymbolValueInfo;
    struct SymbolNamed;
    class AutoExtension;
    class CastExtension;
    class SymbolType;

    struct AssignResolve : public SemanticResolve {
        bool Apply(SemanticResolve *resolve) override;
        bool Apply(SemanticContext*context, Node *node) override;

        // Resulting value
        Symbol* result = nullptr;
    };

    /// Delegates
    using AssignOpDelegate = std::function<SymbolValueInfo*(SemanticContext*, Node *, const SemanticScopeStack&, const Token& op, const Token& targetTok, SymbolValueInfo* target, const Token& valueTok, SymbolValueInfo* value)>;

    class UtilExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.UtilExtension");

        /// Construct and install instance into registry
        /// \return
        static UtilExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        UtilExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Add a unary operation
        /// \param lhs
        /// \param rhs
        /// \param op
        void AddAssignOp(ClassID lhs, ClassID rhs, const AssignOpDelegate &delegate);

        /// Resolve a binary operation
        /// \param context
        /// \param node
        /// \param scopes
        /// \param functions - Functions to attempt calling
        /// \param arguments - Supplied arguments
        /// \return
        AssignResolve ResolveAssign(
                SemanticContext*context, Node *node, SemanticScopeStack scopes,
                const Token& op, const Token& targetTok, SymbolValueInfo* target, const Token& valueTok, SymbolValueInfo* value
        );

        /// Resolve type information, with arguments may result in value
        /// \param context
        /// \param node
        /// \return
        Symbol* ResolveTypeInfo(SemanticContext*context, Node *node, SemanticScopeStack scopes, const std::list<Symbol*>& symbols, const std::list<Symbol*> &arguments = {});

        /// Resolve explicit type information
        /// \param context
        /// \param node
        /// \return
        SymbolPair <SymbolTypeInfo> ResolveExplicitTypeInfo(SemanticContext*context, Node *node, SemanticScopeStack scopes, const std::list<Symbol*>& symbols, const std::list<Symbol*> &arguments = {});

        /// Resolve value information
        /// \param context
        /// \param node
        /// \return
        SymbolPair <SymbolValueInfo> ResolveValueInfo(SemanticContext*context, Node *node, SemanticScopeStack scopes, const std::list<Symbol*>& symbols, bool isInitialization = false);

        /// Resolve value information
        /// \param context
        /// \param node
        /// \return
        SymbolPair <SymbolValueInfo> ResolveValueInfo(SemanticContext*context, Node* node, SemanticScopeStack scopes, Symbol* symbol, bool isInitialization = false);

        /// Resolve type or value type information
        /// \param context
        /// \param node
        /// \return
        SymbolPair <SymbolTypeInfo> ResolveTypeOrValueTypeInfo(SemanticContext*context, Node *node, SemanticScopeStack scopes, const std::list<Symbol*>& symbols, bool isInitialization = false, const std::list<Symbol*> &arguments = {});

        /// Resolve type or value information
        /// \param context
        /// \param node
        /// \return
        Symbol* ResolveTypeOrValueInfo(SemanticContext*context, Node *node, SemanticScopeStack scopes, const std::list<Symbol*>& symbols, bool isInitialization = false, const std::list<Symbol*> &arguments = {});

        /// Get default primitive value of type
        /// \param type
        /// \return
        Symbol* DefaultPrimitiveValue(const SymbolAnchor& anchor, SymbolType* type);

    private:
        Registry* registry = nullptr;
        AutoExtension* autoExtension = nullptr;
        CastExtension* castExtension = nullptr;
        Map<std::pair<ClassID::Type, ClassID::Type>, AssignOpDelegate> assignDelegates;
    };
}
