//
// Created by Dev on 11/2/2017.
//
#pragma once

#include <NyLang/Core/Shared.h>
#include <list>
#include <NyLang/Frontend/Token/Token.h>

namespace NyLang {
    class Node;
    class Symbol;
    struct SemanticContext;
    class DiagnosticBucket;
    namespace IL {
        class Block;
    }

    struct SymbolWrapper {
        Symbol* symbol = nullptr;
        Token tok;
    };

    class SemanticResolve {
    public:
        /// Apply the results of this resolve to another resolve
        /// \param node
        virtual bool Apply(SemanticResolve *resolve) = 0;

        /// Apply the results of this resolve to a node
        /// \param node
        virtual bool Apply(SemanticContext*context, Node *node) = 0;

        // Diagnostic bucket
        DiagnosticBucket* bucket = nullptr;

        // IL Block
        IL::Block* ilBlock = nullptr;
    };
}