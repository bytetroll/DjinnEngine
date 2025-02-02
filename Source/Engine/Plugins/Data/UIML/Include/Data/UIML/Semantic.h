//
// Created by Dev on 5/5/2018.
//
#pragma once

#include <COM/Result.h>
#include "SemanticMode.h"
#include "AST.h"
#include "SemanticScope.h"

namespace Djinn::Memory {
    class Context;
}

namespace Djinn::Data::UIML {
    class Diagnostic;
    struct ParserBlob;
    struct SemanticBlob;

    class Semantic {
    public:
        /// Initialize this semantic verifier
        /// \param context
        /// \return
        COM::Result Initialize(Memory::Context *context, Diagnostic *diag);

        /// Compile from a parsed
        /// \param source
        /// \param out
        /// \return
        COM::Result Compile(ParserBlob *blob, SemanticBlob *out);

    protected:
        COM::Result Visit(Node* node, SemanticScope scope);
        COM::Result Visit(BodyNode* node, SemanticScope scope);
        COM::Result Visit(StyleNode* node, SemanticScope scope);
        COM::Result Visit(ElementNode* node, SemanticScope scope);
        COM::Result Visit(StyleElementNode* node, SemanticScope scope);
        COM::Result Visit(StyleAttributeNode* node, SemanticScope scope);
        COM::Result Visit(StyleUsingNode* node, SemanticScope scope);
        COM::Result Visit(ValueNode* node, SemanticScope scope);
        COM::Result Visit(RGBANode* node, SemanticScope scope);
        COM::Result Visit(ArrayNode* node, SemanticScope scope);
        COM::Result Visit(ComponentNode* node, SemanticScope scope);
        COM::Result Visit(GroupNode* node, SemanticScope scope);
        COM::Result Visit(CategoryNode* node, SemanticScope scope);
        COM::Result Visit(BindNode* node, SemanticScope scope);
        COM::Result Visit(DataNode* node, SemanticScope scope);

    private:
        Memory::Context *context = nullptr;
        Diagnostic *diag = nullptr;
        SemanticBlob* out = nullptr;
    };
}
