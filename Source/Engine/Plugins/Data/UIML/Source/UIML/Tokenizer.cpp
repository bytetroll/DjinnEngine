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
#include <Data/UIML/Tokenizer.h>
#include <Data/UIML/TokenBlob.h>
#include <Core/Context.h>
#include <cstring>
#include <Data/UIML/Diagnostic.h>

DJINN_NS3(Data, UIML);

COM::Result Tokenizer::Initialize(Memory::Context *context, Diagnostic* diag) {
    this->context = context;
    this->diag = diag;

#define TOKEN(STR) { Core::String str = STR; segmentHeads.Add(str[0]); tokens[str] = STR##_id; for (std::uint32_t i = 0; i < std::strlen(STR); i++) { segments.Add(Core::String(str, 0, i + 1)); } }

#include <Data/UIML/Tables/tokens.tbl>

#undef TOKEN

    // OK
    return COM::kOK;
}

COM::Result Tokenizer::Compile(const char *source, TokenBlob *out) {
    Location offset = {};

    // Length of source
    std::size_t length = std::strlen(source);

    // Perform preallocation guess (1 token per 4 characters)
    out->tokens.Reserve(length / 4);

    // Offset helper
    auto next = [&](Location &offset, int n = 1) {
        for (int i = 0; i < n && source[offset.index] != 0; i++) {
            offset.character++;
            if (source[++offset.index] == '\n') {
                offset.character = 0;
                offset.line++;
            }
        }
        return source[offset.index] != 0;
    };

    // Current helper
    auto current = [&](Location &offset) -> char {
        return source[offset.index];
    };

    // Peek helper
    auto peek = [&](Location &offset, int n = 1) -> char {
        if (offset.index + n >= length) {
            return 0;
        }
        return source[offset.index + n];
    };

    // Temporary bucket
    Core::String bucket;

    // Whilst not end
    while (current(offset)) {
        bool _ignore = true;
        while (isspace(current(offset)) && (_ignore = next(offset)));
        if (!_ignore) {
            break;
        }

        // String segment?
        if (current(offset) == '"') {
            Location shadow = offset;
            next(shadow);

            while (current(shadow) != '"' && current(shadow)) {
                bucket += current(shadow);
                next(shadow);
            }
            next(shadow);

            // Add token
            Token tok;
            tok.span = {offset, shadow};
            tok.type = kTTString;
            tok.value.chp = context->NewArray<char>(bucket.Length() + 1);
            std::strcpy(tok.value.chp, bucket.AsCharArray());
            out->tokens.Add(tok);

            // Clean up
            offset = shadow;
            bucket.Flush();
            continue;
        }

        // One liner comment?
        if (current(offset) == '/' && peek(offset) == '/') {
            next(offset, 2);

            while (current(offset) != '\n' && current(offset)) {
                if (!next(offset)) {
                    break;
                }
            }
            next(offset);
            continue;
        }

        // Multiliner comment
        if (current(offset) == '/' && peek(offset) == '*') {
            next(offset, 2);

            while (current(offset) && !(current(offset) == '*' && peek(offset) == '/')) {
                if (!next(offset)) {
                    break;
                }
            }
            auto c = current(offset);
            next(offset, 2);
            continue;
        }

        // Possible segment?
        if (segmentHeads.Contains(std::tolower(current(offset)))) {
            Location shadow = offset;

            // Fill bucket
            do {
                bucket += static_cast<char>(std::tolower(current(shadow)));
            } while (segments.Contains(bucket) && next(shadow));

            // Erase last
            if (current(shadow)) {
                bucket = Core::String(bucket, 0, bucket.Length() - 1);
            }

            // Find token
            auto it = tokens.Find(bucket);

            // Clean up
            bucket.Flush();

            if (it != tokens.End()) {
                // Add token
                Token tok;
                tok.span = {offset, shadow};
                tok.type = (*it).second;
                out->tokens.Add(tok);
                offset = shadow;
                continue;
            }
        }

        // ID?
        if (isalpha(current(offset)) || current(offset) == '_') {
            Location shadow = offset;

            // Fill bucket
            do {
                bucket += current(shadow);
            } while (next(shadow) && (isalnum(current(shadow)) || current(shadow) == '-' || current(shadow) == '_'));

            // Add token
            Token tok;
            tok.span = {offset, shadow};
            tok.type = kTTID;
            tok.value.chp = context->NewArray<char>(bucket.Length() + 1);
            std::strcpy(tok.value.chp, bucket.AsCharArray());
            out->tokens.Add(tok);

            // Clean up
            offset = shadow;
            bucket.Flush();
            continue;
        }

        // Hex segment?
        if (current(offset) == '0' && peek(offset) == 'x') {
            Location shadow = offset;
            next(shadow, 2);

            // Fill bucket
            while (isdigit(current(shadow))) {
                bucket += current(shadow);
                if (!next(shadow)) {
                    break;
                }
            }

            char *pp;
            long n = strtol(bucket.AsCharArray(), &pp, 16);
            if (*pp != 0) {
                // error report
            }

            // Add token
            Token tok;
            tok.span = {offset, shadow};
            tok.type = kTTInt64;
            tok.value.i64 = static_cast<std::int64_t>(n);
            out->tokens.Add(tok);

            // Clean up
            offset = shadow;
            bucket.Flush();
            continue;
        }

        // Numeric segment?
        if (isdigit(current(offset))) {
            Location shadow = offset;

            // Fill bucket
            do {
                bucket += current(shadow);
            } while (next(shadow) && isdigit(current(shadow)));

            Token tok;
            if (current(shadow) == '.') {
                bucket += '.';
                next(shadow);

                do {
                    bucket += current(shadow);
                } while (next(shadow) && isdigit(current(shadow)));

                if (tolower(current(shadow)) == 'f') {
                    next(shadow);

                    tok.type = kTTFloat;

                    char *pp;
                    tok.value.fp32 = strtof(bucket.AsCharArray(), &pp);
                    if (*pp != 0) {
                        diag->Add(Severity::eError, {offset, shadow}, "Invalid number format");
                    }
                } else {
                    if (tolower(current(shadow)) == 'd') {
                        next(shadow);
                    }
                    tok.type = kTTDouble;

                    char *pp;
                    tok.value.fp64 = strtod(bucket.AsCharArray(), &pp);
                    if (*pp != 0) {
                        diag->Add(Severity::eError, {offset, shadow}, "Invalid number format");
                    }
                }
            } else {
                char *pp;

                #if DJINN_PLATFORM_WINDOWS
                    std::int64_t n = _strtoi64(bucket.AsCharArray(), &pp, 0);
                #else
                    std::int64_t n = std::strtoll( bucket.AsCharArray(), &pp, 0 );
                #endif


                if (*pp != 0) {
                    diag->Add(Severity::eError, {offset, shadow}, "Invalid number format");
                }

                tok.type = kTTInt64;
                tok.value.i64 = static_cast<std::int64_t>(n);
            }
            tok.span = {offset, shadow};
            out->tokens.Add(tok);

            // Clean up
            offset = shadow;
            bucket.Flush();
            continue;
        }

        // Unknown token
        {
            Location shadow = offset;
            next(shadow);

            Token tok;
            tok.span = {offset, shadow};
            tok.type = kTTUnknown;
            tok.value.ch = current(offset);
            out->tokens.Add(tok);

            offset = shadow;
        }
    }

    // OK
    return COM::kOK;
}

