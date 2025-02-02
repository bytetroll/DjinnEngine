//
// Created by Dev on 5/5/2018.
//
#pragma once

#include <COM/Result.h>
#include <Core/HashSet.h>
#include <Core/HashMap.h>
#include <Core/String.h>
#include "Token.h"

namespace Djinn::Memory {
    class Context;
}

namespace Djinn::Data::UIML {
    class Diagnostic;
    struct TokenBlob;

    class Tokenizer {
    public:
        /// Initialize this tokenizer
        /// \param context
        /// \return
        COM::Result Initialize(Memory::Context* context, Diagnostic* diag);

        /// Compile from a source
        /// \param source
        /// \param out
        /// \return
        COM::Result Compile(const char* source, TokenBlob* out);

    private:
        Memory::Context* context;
        Diagnostic* diag;

        // Segment set
        Core::HashSet<char> segmentHeads;
        Core::HashSet<Core::String> segments;

        // Token map
        Core::HashMap<Core::String, TokenID> tokens;
    };
}
