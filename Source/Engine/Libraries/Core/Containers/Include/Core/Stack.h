// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
#pragma once

#include <Core/Types.h>

namespace Djinn {
    namespace Core {
        #define DJINN_DEFAULT_STACK_ALLOC 25
        #define DJINN_DEFAULT_STACK_REALLOC 25

        template< class Type >
        class Stack {
        public:
            Stack() {
                Realloc( DJINN_DEFAULT_STACK_ALLOC );
            }

            Stack( const Stack< Type >& Copy ) {
            }

            ~Stack() {
            }

            void Push( const Type& Element ) {
            }

            Type& Pop() const {
            }

            Type& Peek() const {
            }

            Stack& operator=( const Stack< Type >& Rhs ) {
            }

            const bool Size() const {
                return BackingArrayLength;
            }

            void Reserve( const UInt32 Amount ) {
            }

            // Pushes a new element to the top of the stack without copy or move on the element.
            void Emplace( const Type&& Element ) {
            }

        private:
            void Realloc( const UInt32 Amount ) {

            }

            const bool NeedsResize() const {
            }

            void Resize() {
            }

        private:
            Type* BackingArray;
            UInt32 BackingArrayLength;
        };
    }
}