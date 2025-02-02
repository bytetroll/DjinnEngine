//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include "ISemantic.h"

namespace NyLang {
    class Semantic : public ISemantic {
    public:
        LANG_CLASSID("Lang.Semantic");

        /// Construct
        /// \param registry
        Semantic(Registry* registry);

        /// Overrides
        virtual ClassID GetClassID() override;
        virtual NyLang::SemanticContext* Generate(ParserContext*parser) override;
        virtual SemanticEvent Visit(SemanticContext* context, Node *node, SemanticScopeStack scopes) override;
        virtual SemanticEvent Visit(SemanticContext* context, Node *node, SemanticScopeStack scopes, SemanticMode mode) override;
        virtual void AddHandler(ActionID aid, const Vector<ClassID> &triggers, const SemanticDelegate &delegate) override;

    private:
        Registry* registry = nullptr;

        // Triggers
        UnorderedMap<ClassID::Type, SemanticDelegate> triggerHandlers;
    };
}
