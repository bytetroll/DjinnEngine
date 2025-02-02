// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// C++ STL.
#include <sstream>

// Core.
#include <Core/Types.h>
#include <Core/Assert.h>
#include <Core/Array.h>
#include <Core/String.h>

#include "../Machine/Internal.h"


// Regex.
//#include "Core/Machine/Internal.h"

// State machine state.

namespace Djinn {
    namespace Regex {
        using namespace Djinn;
        using namespace Djinn::Core;

        class State {
        public:
            static constexpr Int32 INVALID_STATE = -1;

        public:
            State() : ID( INVALID_STATE ), AcceptingState( false ) {
            }

            State( const Int32 ID ) : ID( ID ), AcceptingState( false ) {
            }

            // Constructs a new state from a set of existing states.  This is needed for
            // subset construction as a new DFA state will be constructed from one or
            // more existing NFA states.  The final "accepting" state is the last state
            // on the stack.
            State( Set< State* >& NfaStates, const Int32 ID ) {
                this->NfaStates = NfaStates;
                this->ID = ID;

                // DFA state is flagged as the accepting state if it is constructed
                // from an accepting NFA state.
                for( auto Iter = NfaStates.begin(); Iter != NfaStates.end(); ++Iter ) {
                    if( (*Iter)->AcceptingState ) {
                        AcceptingState = true;
                    }
                }
            }

            State( const State& Copy ) {
                *this = Copy;
            }

            virtual ~State() {
            }

            void AddTransition( const char Input, State* const NfaState ) {
                DJINN_ASSERT( NfaState != nullptr );
                Transitions.insert( std::make_pair( Input, NfaState ) );
            }

            void RemoveTransition( State* const NfaState ) {
                for( auto Iter = Transitions.begin(); Iter != Transitions.end(); ) {
                    State* const ToState = Iter->second;
                    if( ToState == NfaState ) {
                        Transitions.erase( Iter++ );
                    } else {
                        ++Iter;
                    }
                }
            }

            void GetTransisitions( const char Input, Array< State* >& States ) {
                States.Clear();

                for( auto Iter = Transitions.lower_bound( Input ); Iter != Transitions.upper_bound( Input ); ++Iter ) {
                    State* const NfaState = Iter->second;
                    DJINN_ASSERT( NfaState != nullptr );
                    States.Add( NfaState );
                }
            }

            // Used for DFA reduction.  Dead end means that this state is not the accepting
            // (final) state and has no transitions to other states.
            bool IsDeadEnd() const {
                if( AcceptingState ) {
                    return false;
                } else if( Transitions.empty() ) {
                    return true;
                }

                for( auto Iter = Transitions.begin(); Iter != Transitions.end(); ++Iter ) {
                    State* const ToState = Iter->second;
                    if( ToState != this ) {
                        return false;
                    }
                }

                return true;
            }

            bool IsAcceptingState() const {
                return AcceptingState;
            }

            void FlagAsAcceptingState() {
                AcceptingState = true;
            }

            String StateID() const {
                std::stringstream Stream;

                if( AcceptingState ) {
                    Stream << "{" << ID << "}";
                } else {
                    Stream << ID;
                }

                return Stream.str().c_str();
            }

            // Get a list of the NFA states used to construct this DFA state.
            Set< State* >& GetNfaStates() {
                return NfaStates;
            }

            State& operator=( const State& Rhs ) {
                Transitions = Rhs.Transitions;
                ID = Rhs.ID;
                NfaStates = Rhs.NfaStates;
                return *this;
            }

            bool operator==( const State& Rhs ) {
                if( NfaStates.empty() ) {
                    return ID == Rhs.ID;
                }

                return NfaStates == Rhs.NfaStates;
            }

        protected:
            Multimap< char, State* > Transitions;
            Set< State* > NfaStates;

            Int32 ID;
            bool AcceptingState;
        };
    }
}