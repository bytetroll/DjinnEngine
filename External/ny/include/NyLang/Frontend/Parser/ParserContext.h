//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/COM/ID.h>
#include <NyLang/Core/Shared.h>

namespace NyLang {
    class Registry;
    class IDiagnostic;
    class TokenizerContext;
    class IParser;
    class Node;

    /// Parsing context
    struct ParserContext {
        ParserContext(Registry*registry, IParser* parser, TokenizerContext* tokenizer);

        // Parser
        IParser* parser = nullptr;

        // Tokenizer
        TokenizerContext* tokenizer = nullptr;

        // Diagnostic
        IDiagnostic* diagnostic = nullptr;

        // Root Node
        Node* root = nullptr;

        // Contained registry
        Registry* registry = nullptr;
    };
}