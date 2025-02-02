// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Core.
#include <Core/Types.h>
#include <Core/Assert.h>

// Machine.
#include "State.h"

// Pattern recognition.


namespace Djinn {
    namespace Regex {
        using namespace Djinn;
        using namespace Djinn::Core;

        class PatternRecognizer {
        public:
            PatternRecognizer() : CurrentState( nullptr ), StartPosition( State::INVALID_STATE ) {
            }

            PatternRecognizer( const PatternRecognizer& Copy ) {
                *this = Copy;
            }

            virtual ~PatternRecognizer() {
            }

            PatternRecognizer& operator=( const PatternRecognizer& Rhs ) {
                DJINN_ASSERT( Rhs.CurrentState != nullptr );
                CurrentState = Rhs.CurrentState;
                StartPosition = Rhs.StartPosition;

                return* this;
            }

        public:
            State* CurrentState; // In DFA.
            Int32 StartPosition;
        };
    }
}