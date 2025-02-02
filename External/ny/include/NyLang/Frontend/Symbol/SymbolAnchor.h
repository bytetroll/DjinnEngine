//
// Created by Dev on 10/29/2017.
//
#pragma once

#include <NyLang/Frontend/Token/Token.h>

namespace NyLang {
    // Symbols contain backward references
    class SymbolTree;
    class Node;
    class Symbol;

    /// Symbol anchor, represents a location where the symbol was created
    struct SymbolAnchor {
        SymbolAnchor() {

        }

        SymbolAnchor(const Token &tok, Node *node, SymbolTree *tree) : Tok(tok), node(node), tree(tree) {

        }

        // Symbol Token
        Token Tok;

        // Node owning this symbol
        Node *node = nullptr;

        // Symbol tree owning this symbol
        SymbolTree *tree = nullptr;
    };
}