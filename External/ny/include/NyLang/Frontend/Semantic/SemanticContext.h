//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Core/COM/ID.h>
#include <NyLang/Core/Shared.h>
#include "SemanticScopeStack.h"

namespace NyLang {
    class Registry;
    class IDiagnostic;
    struct ParserContext;
    class ISemantic;
    class Node;
    namespace IL {
        class Program;
        class Emitter;
    }

    /// Parsing context
    struct SemanticContext {
        SemanticContext(Registry*registry, ISemantic* semantic, ParserContext* parser);

        // Semantic
        ISemantic* semantic = nullptr;

        // Parser
        ParserContext* parser = nullptr;

        // Diagnostic
        IDiagnostic* diagnostic = nullptr;

        // IL Program
        IL::Program* ilProgram = nullptr;

        // IL Emitter
        IL::Emitter* ilEmitter = nullptr;

        // Contained registry
        Registry* registry = nullptr;
    };
}