//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Frontend/Token/Token.h>
#include <NyLang/Core/Shared.h>
#include <list>

namespace NyLang {
    class ParserContext;

    /// Parser snapshot
    struct ParserSnapshot {
        /// Construct
        /// \param context
        ParserSnapshot(ParserContext*context);

        /// Merge with other branch
        /// \param other
        /// \return
        ParserSnapshot &operator=(const ParserSnapshot &other);

        /// Move to next token
        /// \return
        bool Next();

        /// Is end?
        /// \return
        bool IsEOS();

        /// Peek token
        /// \param offset
        /// \return
        Token Peek(int offset = 1);

        /// Get context
        /// \return
        inline ParserContext*GetContext() {
            return context;
        }

        // Current token
        Token tok;

    private:
        // Parser context
        ParserContext* context = nullptr;

        // Current location
        std::list<Token>::const_iterator tokIt;
    };
}
