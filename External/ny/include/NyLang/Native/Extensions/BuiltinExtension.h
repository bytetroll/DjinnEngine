//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Core/Exception.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>
#include <functional>

namespace NyLang {
    class UtilExtension;
    class Symbol;
    struct SemanticContext;
    class Node;
    class CallExtension;

    /// Common exceptions
    LANG_EXCEPTION(SemanticInvalidBuiltinName);

    /// Delegates
    using BuiltinDelegate = std::function<Symbol*(SemanticContext*, Node *, const SemanticScopeStack&, const Vector<Symbol*>& args)>;

    class BuiltinExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.BuiltinExtension");
        LANG_ACTIONID(Action, "Lang.BuiltinExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static BuiltinExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        BuiltinExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

        void AddBuiltin(const String& name, const BuiltinDelegate& delegate);

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
        CallExtension* callExtension = nullptr;

        UnorderedMap<String, BuiltinDelegate> builtins;
    };
}
