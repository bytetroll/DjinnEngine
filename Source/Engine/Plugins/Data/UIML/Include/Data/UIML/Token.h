//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 5/5/2018.
//
#pragma once

#include <Core/Types.h>
#include <Core/Unique.h>

namespace Djinn::Data::UIML {
    DJINN_UNIQUEID(TokenID, UInt64);

    union TokenValue {
        Int64 i64;
        float fp32;
        double fp64;
        char ch;
        char *chp;
    };

    /// Common token types
    static constexpr TokenID kTTNone = "<none>"_id;
    static constexpr TokenID kTTUnknown = "<unknown>"_id;
    static constexpr TokenID kTTInt64 = "<int64>"_id;
    static constexpr TokenID kTTFloat = "<float>"_id;
    static constexpr TokenID kTTDouble = "<double>"_id;
    static constexpr TokenID kTTChar = "<char>"_id;
    static constexpr TokenID kTTID = "<id>"_id;
    static constexpr TokenID kTTString = "<string>"_id;

    struct Location {
        /// Index in the source stream
        std::uint32_t index = 0;

        /// Character of the line, starts as 1
        std::uint32_t character = 1;

        /// Line number, starts at 1
        std::uint32_t line = 1;

        /// Source file index
        std::uint32_t file = 0;
    };

    struct Span {
        template<typename T0, typename T1, typename T2, typename... TX>
        static Span Range(T0 t0, T1 t1, T2 t2, TX... tx) {
            return Range(Range(t0, t1), t2, tx...);
        }

        template<typename T0, typename T1>
        static Span Range(T0 t0, T1 t1) {
            Span span;
            if (t0.begin.index < t1.begin.index) {
                span.begin = t0.begin;
            } else {
                span.begin = t1.begin;
            }
            if (t0.end.index > t1.end.index) {
                span.end = t0.end;
            } else {
                span.end = t1.end;
            }
            return span;
        }

        /// Begin location
        Location begin;

        /// End location
        Location end;
    };

    struct Token {
        /// Type of this token
        TokenID type = kTTNone;

        /// Span of this token
        Span span;

        /// Value of this token (May not be filled)
        TokenValue value = {};
    };
}