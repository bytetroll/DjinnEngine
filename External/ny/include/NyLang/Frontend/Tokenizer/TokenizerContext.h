//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/Shared.h>
#include <NyLang/Frontend/Token/Token.h>
#include <list>

namespace NyLang {
    class Registry;
    class IDiagnostic;

    /// Tokenizer context
    struct TokenizerContext {
        /// Construct
        /// \param registry
        TokenizerContext(Registry*registry);

        // Token list
        std::list<Token> tokens;

        // Diagnostic
        IDiagnostic* diagnostic = nullptr;

        // Contained registry
        Registry* registry = nullptr;
    };
}