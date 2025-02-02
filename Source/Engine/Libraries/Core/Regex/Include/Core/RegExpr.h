// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

// Core.
#include <Core/String.h>
#include <Core/Array.h>

// Regex.
#include "Machine/Internal.h"
#include "Machine/State.h"
#include "Machine/PatternRecognizer.h"

// Defines a regular expression string.

namespace Djinn {
    namespace Regex {
        class RegExpr {
        public:
            // @TODO: These should be renamed to match their operations. NCY, 5.18.2018
            static constexpr Int32 REGEX_OP_STAR = 0x0000002A; // '*'
            static constexpr Int32 REGEX_OP_BAR = 0x00000008E; // '|'
            static constexpr Int32 REGEX_OP_LEFT_PAREN = 0x00000028; // '('
            static constexpr Int32 REGEX_OP_RIGHT_PAREN = 0x00000029; // ')'
            static constexpr Int32 REGEX_OP_BACKSPACE = 0x00000008; // '<backspace>'

            using FsaTable = Deque< State* >;

        public:
            RegExpr();
            virtual ~RegExpr();

            bool Compile( char* const Array ) {
                String Buffer( Array );
                return Compile( Buffer );
            }

            bool Compile( String& RegexString );
            bool FindFirstPattern( const String& In, Int32& Position, String& Pattern );
            bool FindNextPattern( Int32& Position, String& Pattern );

            void Push( const char Token );
            bool Pop( FsaTable& NfaTable );

            // Finds all occurrences of the pattern in input.
            bool Find();
            
            bool IsInput( const char Token ) const;

            bool IsLeftParanthesis( const char Token ) const {
                return Token == REGEX_OP_LEFT_PAREN;
            }

            bool IsRightParanthesis( const char Token ) const {
                return Token == REGEX_OP_RIGHT_PAREN;
            }

            bool IsOperator( const char Token ) const {
                switch( Token ) {
                    case REGEX_OP_STAR:
                    case REGEX_OP_RIGHT_PAREN:
                    case REGEX_OP_LEFT_PAREN:
                    //case REGEX_OP_BAR:
                    case REGEX_OP_BACKSPACE:
                        return true;

                    default:
                        return false;
                }
            }

            // Returns true is LHS <= RHS.
            //
            // Kleen's Closure -- highest.
            // Concatentation -- middle.
            // Union - lowest.
            bool HasPresedence( const char LeftOp, const char RightOp ) const {
                if( LeftOp == RightOp ) {
                    return true;
                } else if( LeftOp == REGEX_OP_STAR ) {
                    return false;
                } else if( RightOp == REGEX_OP_STAR ) {
                    return true;
                } else if( LeftOp == REGEX_OP_BACKSPACE ) {
                    return false;
                } else if( RightOp == REGEX_OP_BACKSPACE ) {
                    return true;
                } /*else if( LeftOp == REGEX_OP_BAR ) {
                    return false;
                }*/

                return true;
            }

        private:
            bool CreateNFA( String& RegexString );

            // Compute the epsilon closure for the state.
            void ComputeEpsilonClosure( const Set< State* >& Transisitions, Set< State*>& Results );

            // Computes the movements for a given input token.
            void ComputeMovements( const char Token, const Set< State* >& Transisitions, Set< State* >& Results );

            // Convert NFA structures to DFA structures using the subset construction algorithm.
            void NfaToDfa();
            void ReduceDFA();

            // Evaluates the next token from the stack.
            bool EvaluateNext();

            // Operations.
            bool Operation_Concatenation();
            bool Operation_KleenClosure();
            bool Operation_Union();

            // Inserts the backspace token where concatenation is to occure.
            String ExpandForConcatenation( const String& RegexString ) const;

            void Free() {
                // NFA table cleanup.
                for( USize Index = 0; Index < NfaTable.size(); ++Index ) {
                    delete NfaTable[ Index ];
                    NfaTable[ Index ] = nullptr;
                }
                NfaTable.clear();

                // DFA table cleanup.
                for( USize Index = 0; Index < DfaTable.size(); ++Index ) {
                    delete DfaTable[ Index ];
                    DfaTable[ Index ] = nullptr;
                }
                DfaTable.clear();

                NextStateID = 0;

                // Stacks.
                while( !OperatorStack.empty() ) {
                    OperatorStack.pop();
                }
                while( !OperandStack.empty() ) {
                    OperatorStack.pop();
                }

                // Set.
                InputSet.clear();

                // Pattern stuff.
                List< PatternRecognizer* >::iterator Iter;
                for( Iter = PatternList.begin(); Iter != PatternList.end(); ++Iter ) {
                    delete* Iter;
                }
                PatternList.clear();
            }

        private:
            // Using Thompson's algorithm, each operand is an NFA automata.
            Stack< FsaTable > OperandStack;
            Stack< char > OperatorStack;

            FsaTable NfaTable;
            FsaTable DfaTable;

            Int32 NextStateID;

            // Input.
            String Input;
            Set< char > InputSet;

            // Pattern matching.
            Array< String > RecognziedPatterns;
            Array< Int32 > RecognizedPatternPositions;
            List< PatternRecognizer* > PatternList;
            Int32 PatternIndex;
        };
    }
}