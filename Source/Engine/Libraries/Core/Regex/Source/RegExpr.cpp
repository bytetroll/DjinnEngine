// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

// Core.
#include <Core/Types.h>
#include <Core/String.h>

// Regex.
#include <Core/RegExpr.h>

// Debug.
#include <Core/RuntimeException.h>
#include <Core/ExceptionFormatter.h>

namespace Djinn {
    namespace Regex {
        RegExpr::RegExpr() : NextStateID( 0 ) {
        }

        RegExpr::~RegExpr() {
            Free();
        }

        bool RegExpr::Compile( String& RegexString ) {
            Free();

            if( CreateNFA( RegexString ) ) {
                NfaToDfa();
                ReduceDFA();

                return true;
            }

            return false;
        }

        bool RegExpr::FindFirstPattern( const String& In, Int32& Position,
            Djinn::Core::String& Pattern ) {
            // Cleanup all pattern states before finding the next pattern.
            for( auto Iter = PatternList.begin(); Iter != PatternList.end(); ++Iter ) {
                delete* Iter;
            }
            PatternList.clear();

            Input = In;

            if( Find() ) {
                Position = RecognizedPatternPositions[ 0 ];
                Pattern = RecognziedPatterns[ 0 ];
                PatternIndex = 0;

                return true;
            }

            return false;
        }

        bool RegExpr::FindNextPattern( Int32& Position, String& Pattern ) {
            ++PatternIndex;

            if( PatternIndex < static_cast<Int32>(RecognizedPatternPositions.Size()) ) {
                Position = RecognizedPatternPositions[ PatternIndex ];
                Pattern = RecognziedPatterns[ PatternIndex ];

                return true;
            }

            return false;
        }

        bool RegExpr::Find() {
            bool Ret = false;

            // Clean up our queues for the new search.
            RecognziedPatterns.Clear();
            RecognizedPatternPositions.Clear();

            if( DfaTable.empty() ) {
                return Ret;
            }

            for( USize CharIndex = 0; CharIndex < Input.Length(); ++CharIndex ) {
                const char Char = Input[ CharIndex ];

                for( auto Iter = PatternList.begin(); Iter != PatternList.end(); ++Iter ) {
                    PatternRecognizer* const Recognizer = ( *Iter );
                    Array< State* > Transisitions;

                    Recognizer->CurrentState->GetTransisitions( Char, Transisitions );
                    if( !Transisitions.Empty() ) {
                        Recognizer->CurrentState = Transisitions[ 0 ];

                        // Check to see if we're at the final state.
                        if( Transisitions[ 0 ]->IsAcceptingState() ) {
                            const Int32 SubStart = Recognizer->StartPosition;
                            const Int32 SubEnd = ( CharIndex - SubStart ) + 1;

                            RecognziedPatterns.Add( Input.Substring( static_cast<UInt32>(SubStart), static_cast<UInt32>(SubEnd )) );
                            RecognizedPatternPositions.Add( Recognizer->StartPosition );
                        }
                    } else {
                        Iter = PatternList.erase( Iter );
                        --Iter;
                    }
                }

                State* const S = DfaTable[ 0 ];

                // Check against the first state of the Dfa table.
                Array< State* > Transisitions;
                S->GetTransisitions( Char, Transisitions );
                if( !Transisitions.Empty() ) {
                    PatternRecognizer* const Recognizer = new PatternRecognizer;
                    Recognizer->StartPosition = CharIndex;
                    Recognizer->CurrentState = Transisitions[ 0 ];

                    PatternList.push_back( Recognizer );

                    if( Transisitions[ 0 ]->IsAcceptingState() ) {
                        RecognizedPatternPositions.Add( CharIndex );

                        String Buffer;
                        Buffer += Char;

                        RecognziedPatterns.Add( Buffer );
                    }
                } else {
                    // Check if entry state is already accepting, i.e., a* accepts 0 or many a's,
                    // so any character is accepting.
                    if( S->IsAcceptingState() ) {
                        RecognizedPatternPositions.Add( CharIndex );
                        String Buffer;
                        Buffer += Char;
                        RecognziedPatterns.Add( Buffer );
                    }
                }
            }

            return RecognizedPatternPositions.Size() > 0;
        }

        bool RegExpr::EvaluateNext() {
            if( OperatorStack.size() > 0 ) {
                const char Operator = OperatorStack.top();
                OperatorStack.pop();

                switch( Operator ) {
                    case REGEX_OP_STAR: return Operation_KleenClosure();
                    //case REGEX_OP_BAR: return Operation_Union();
                    case REGEX_OP_BACKSPACE: return Operation_Concatenation();

                    default:
                        return false;
                        //throw Debug::RuntimeException( Debug::ExceptionFormatter() << "Regex operator not implemented" );
                }
            }

            return false;
        }

        // Operations.
        bool RegExpr::Operation_Concatenation() {
            FsaTable A, B;
            if( !Pop( A ) || !Pop( B ) ) {
                return false;
            }

            // Eval AB.  Last state from A is added to the epsilon transition of the first state of B.
            A[ A.size() - 1 ]->AddTransition( 0, B[ 0 ] );
            A.insert( A.end(), B.begin(), B.end() );

            OperandStack.push( A );

            return true;
        }

        bool RegExpr::Operation_KleenClosure() {
            FsaTable A, B;
            if( !Pop( A ) ) {
                return false;
            }

            // Eval A*.  This will create two new states which will be pushed to the tail of the deque.
            // An epsilon transition from last to first will also be created.  The epsilon tranistion will
            // be between the two new states.
            State* const Start = new State( ++NextStateID );
            State* const End = new State( ++NextStateID );

            Start->AddTransition( 0, A[ 0 ] );
            A[ A.size() - 1 ]->AddTransition( 0, End );
            A[ A.size() - 1 ]->AddTransition( 0, A[ 0 ] );

            // Create the new DFA.
            A.push_back( End );
            A.push_front( Start );

            OperandStack.push( A );

            return true;
        }

        bool RegExpr::Operation_Union() {
            FsaTable A, B;
            if( !Pop( A ) || !Pop( B ) ) {
                return false;
            }

            // Eval AB.  Creates two new states.  An epsilon transition from the start of A and B is created to the
            // end of the new state.
            State* const Start = new State( ++NextStateID );
            Start->AddTransition( 0, A[ 0 ] );

            State* const End = new State( ++NextStateID );
            End->AddTransition( 0, B[ 0 ] );

            A[ A.size() - 1 ]->AddTransition( 0, End );
            B[ B.size() - 1 ]->AddTransition( 0, End );

            // Create the NFA.
            B.push_back( End );
            A.push_front( Start );
            A.insert( A.end(), B.begin(), B.end() );

            OperandStack.push( A );

            return true;
        }

        String RegExpr::ExpandForConcatenation( const String& RegexString ) const {
            String Ret;

            for( USize Index = 0; Index < ( RegexString.Length() - 1 ); ++Index ) {
                const char Left = RegexString[ Index ];
                const char Right = RegexString[ Index + 1 ];

                Ret += Left;

                if( IsInput( Left ) || IsRightParanthesis( Left ) || ( Left == '*' ) ) {
                    if( IsInput( Right ) || IsLeftParanthesis( Right ) ) {
                        Ret += REGEX_OP_BACKSPACE;
                    }
                }

                Ret += RegexString[ RegexString.Length() - 1 ];
            }

            return Ret;
        }

        bool RegExpr::CreateNFA( String& RegexString ) {
            RegexString = ExpandForConcatenation( RegexString );

            for( USize Index = 0; Index < RegexString.Length(); ++Index ) {
                const char Char = RegexString[ Index ];

                if( IsInput( Char ) ) {
                    Push( Char );
                } else if( OperatorStack.empty() ) {
                    OperatorStack.push( Char );
                } else if( IsLeftParanthesis( Char ) ) {
                    OperatorStack.push( Char );
                } else if( IsRightParanthesis( Char ) ) {
                    // Eval everything between the parenthesis.
                    while( !IsLeftParanthesis( OperatorStack.top() ) ) {
                        if( !EvaluateNext() ) {
                            return false;
                        }

                        OperatorStack.pop();
                    }
                } else {
                    while( !OperatorStack.empty() && HasPresedence( Char, OperatorStack.top() ) ) {
                        if( !EvaluateNext() ) {
                            return false;
                        }

                        OperatorStack.push( Char );
                    }
                }

                // Eval rest of operators.
                while( !OperatorStack.empty() ) {
                    if( !EvaluateNext() ) {
                        return false;
                    }
                }

                if( !Pop( NfaTable ) ) {
                    return false;
                }

                // THe last NFA table is always the accepting state.
                NfaTable[ NfaTable.size() - 1 ]->FlagAsAcceptingState();

                return true;
            }

            return false;
        }

        void RegExpr::Push( const char Token ) {
            State* const State0 = new State( ++NextStateID );
            State* const State1 = new State( ++NextStateID );

            State0->AddTransition( Token, State1 );

            // Create the NFA.
            FsaTable NfaTable;
            NfaTable.push_back( State0 );
            NfaTable.push_back( State1 );

            OperandStack.push( NfaTable );

            InputSet.insert( Token );
        }

        bool RegExpr::Pop( FsaTable& NfaTable ) {
            if( OperandStack.size() > 0 ) {
                NfaTable = OperandStack.top();
                OperandStack.pop();

                return true;
            }

            return false;
        }

        void RegExpr::ComputeEpsilonClosure( const Set< Djinn::Regex::State* >& Transisitions,
            Set< Djinn::Regex::State* >& Results ) {
            Results.clear();

            // each state has a epsilon closure to itself to result starts with the transisitions.
            Results = Transisitions;

            Stack< State* > Unprocessed;
            for( auto Iter = Transisitions.begin(); Iter != Transisitions.end(); ++Iter ) {
                Unprocessed.push( *Iter );
            }

            while( !Unprocessed.empty() ) {
                State* const T = Unprocessed.top();
                Unprocessed.pop();

                // Get all of the epsilon transitions for this state.
                Array< State* > Epsilons;
                T->GetTransisitions( 0, Epsilons );

                // Each state U with an edge fro T to U is an epsilon.
                for( USize Index = 0; Index < Epsilons.Size(); ++Index ) {
                    State* const U = Epsilons[ Index ];

                    if( Results.find( U ) == Results.end() ) {
                        Results.insert( U );
                        Unprocessed.push( U );
                    }
                }
            }
        }

        void RegExpr::ComputeMovements( const char Token, const Set< State* >& Transisitions, Set< State* >& Results ) {
            Results.clear();

            for( auto Iter = Transisitions.begin(); Iter != Transisitions.end(); ++Iter ) {
                State* const S = *Iter;

                Array< State* > States;
                S->GetTransisitions( Token, States );

                for( USize Index = 0; Index < States.Size(); ++Index ) {
                    Results.insert( States[ Index ] );
                }
            }
        }

        void RegExpr::NfaToDfa() {
            // Clean the DFA table.
            for( USize Index = 0; Index < DfaTable.size(); ++Index ) {
                delete DfaTable[ Index ];
            }
            DfaTable.clear();

            if( NfaTable.empty() ) {
                return;
            }

            NextStateID = 0;

            Array< State* > Unprocessed;
            Set< State* > DfaStart;
            Set< State* > NfaStart;

            NfaStart.insert( NfaTable[ 0 ] );

            ComputeEpsilonClosure( NfaStart, DfaStart );

            State* const DfaStartState = new State( DfaStart, ++NextStateID );
            DfaTable.push_back( DfaStartState );
            Unprocessed.Add( DfaStartState );

            while( !Unprocessed.Empty() ) {
                State* const ProcessingDfaState = Unprocessed[ Unprocessed.Size() - 1 ];
                Unprocessed.PopEnd();

                for( auto Iter = InputSet.begin(); Iter != InputSet.end(); ++Iter ) {
                    Set< State* > Move, Epsilon;
                    ComputeMovements( *Iter, ProcessingDfaState->GetNfaStates(), Move );
                    ComputeEpsilonClosure( Move, Epsilon );

                    bool Found = false;
                    State* S = nullptr;
                    for( USize Iter = 0; Iter < DfaTable.size(); ++Iter ) {
                        S = DfaTable[ Iter ];
                        if( S->GetNfaStates() == Epsilon ) {
                            Found = true;
                            break;
                        }
                    }

                    if( !Found ) {
                        State* const U = new State( Epsilon, ++NextStateID );
                        Unprocessed.Add( U );
                        DfaTable.push_back( U );
                    } else {
                        ProcessingDfaState->AddTransition( *Iter, S );
                    }
                }
            }
        }

        void RegExpr::ReduceDFA() {
            Set< State* > DeadEnds;
            for( USize Index = 0; Index < DfaTable.size(); ++Index ) {
                if( DfaTable[ Index ]->IsDeadEnd() ) {
                    DeadEnds.insert( DfaTable[ Index ] );
                }
            }

            if( DeadEnds.empty() ) {
                return;
            }

            // Remove all transitions to the dead-end states.
            for( auto Iter = DeadEnds.begin(); Iter != DeadEnds.end(); ++Iter ) {
                for( USize Index = 0; Index < DfaTable.size(); ++Index ) {
                    DfaTable[ Index ]->RemoveTransition( *Iter );
                }

                Deque< State* >::iterator Position;
                for( Position = DfaTable.begin(); Position != DfaTable.end(); ++Position ) {
                    if( *Position == *Iter ) {
                        break;
                    }
                }

                DfaTable.erase( Position );
                delete *Iter;
            }
        }

        bool RegExpr::IsInput( const char Token ) const {
            return !IsOperator( Token );
        }
    }
}