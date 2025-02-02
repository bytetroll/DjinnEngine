//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include "ITokenizer.h"

namespace NyLang {
    class Tokenizer : public ITokenizer {
    public:
        LANG_CLASSID("Lang.Tokenizer");

        Tokenizer(Registry*registry);

        /// Overrides
        virtual ClassID GetClassID() override;
        virtual NyLang::TokenizerContext* Tokenize(const char* str) override;
        virtual void AddHandler(ActionID aid, const Vector<TokenizerProperty> &properties, const TokenizerDelegate &delegate) override;
        virtual bool Configure() override;
        virtual void AddDependency(const Dependency &dependency) override;

    protected:
        struct Action {
            ActionID id;
            TokenizerDelegate delegate;
        };

    private:
        bool ResolveBucket(Vector<Action>& bucket);

        Registry* registry = nullptr;

        // Trigger handlers
        UnorderedMap<char, Vector<Action>> triggerHandlers;

        // Handlers
        Vector<Action> handlers;

        // Dependencies
        Vector<Dependency> dependencies;
    };
}
