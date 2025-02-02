#pragma once

#include <cassert>
#include <cstring>
#include <cstdio>
#include <cctype>
#include <Core/Common.h>
#include <Core/Hash.h>
#include <Core/Assert.h>
#include <cinttypes>

DJINN_BEGIN_NS2(Djinn, Core)

        enum class StringEncodingScheme : unsigned int {
            eASCII,
            eUTF8_BOM,
            eUTF8_No_BOM,
            eUTF8_Invalid,
            eUTF8_Invalid_BOM
        };

#define DJINN_STRING_ALLOCATION_BASE 20
#define DJINN_STRING_ALLOCATION_GRAIN 32

        inline bool IsCharacterUppercase(int c) {
            return std::isupper(c);
        }

        inline bool IsCharacterLowercase(int c) {
            return std::islower(c);
        }

        class String {
        public:
            static constexpr int NPOS = -1;

        private:
            static constexpr unsigned int STATIC_BIT = 31;
            static constexpr unsigned int STATIC_MASK = 1u << STATIC_BIT;
            static constexpr unsigned int ALLOCATION_MASK = STATIC_MASK - 1;

        public:
            String();
            String(const String &string);
            String(const String &string, const int start, const int end);
            String(const char *charArray);
            String(const char *charArray, const int start, const int end);

            String(const char *charArray, bool _static) {
                InternalConstruct();

                EncodeStaticBit(_static);

                if (charArray != nullptr) {
                    const unsigned int length = std::strlen(charArray);

                    VerifyAllocation(length + 1);
                    std::strcpy(backingArray, charArray);

                    backingArrayLength = length;
                }
            }

            explicit String(const bool value);
            explicit String(const char value);
            explicit String(const Int16 value);
            explicit String(const Int32 value);
            explicit String(const Int64 value);
            explicit String(const UInt16 value);
            explicit String(const UInt32 value);
            explicit String(const UInt64 value);
            explicit String(const ULong value);
            explicit String(const float value);
            explicit String(const double value);

            ~String();

            inline unsigned int Length() const {
                return static_cast<unsigned>(backingArrayLength);
            }

            const inline char *AsCharArray() const {
                return backingArray;
            }

            inline char *Ptr() {
                return backingArray;
            }

            inline bool Empty() const {
                return backingArrayLength == 0;
            }

            void Flush();
            void FlushStatic();

            /// Compute hash of this string
            /// \return
            USize Hash() const {
                return Core::ComputeCRC64(AsCharArray());
            }

            // Reserves n + 1 for null terminator.
            void Reserve(const unsigned int amount) {
                VerifyAllocation(amount + 1);

                EncodeStaticBit(false);
                EncodeAllocationBit(DJINN_STRING_ALLOCATION_BASE);

                backingArray = new char[amount + 1];
                backingArrayLength = amount;
                backingArray[amount] = '\0';
            }

            Int32 IndexOf(char c) const {
                for (auto i = 0U; i < Length(); i++) {
                    if (backingArray[i] == c) {
                        return i;
                    }
                }
                return -1;
            }

            Int32 LastIndexOf(char c) const {
                for (Int32 i = Length() - 1; i >= 0; i--) {
                    if (backingArray[i] == c) {
                        return i;
                    }
                }
                return -1;
            }

            Int32 LastIndexOf(const char *c) const {
                auto n = std::strlen(c);
                for (Int32 i = Length() - 1; i >= 0; i--) {
                    for (USize j = 0; j < n; j++) {
                        if (backingArray[i] == c[j]) {
                            return i;
                        }
                    }
                }
                return -1;
            }

            template<typename F>
            bool Any(F&& functor) const {
                for (auto Index = 0; Index < backingArrayLength; Index++) {
                    if (functor(backingArray[Index])) {
                        return true;
                    }
                }

                return false;
            }

            template<typename F>
            bool All(F&& functor) const {
                for (auto Index = 0; Index < backingArrayLength; Index++) {
                    if (!functor(backingArray[Index])) {
                        return false;
                    }
                }

                return true;
            }

            bool EmptyWhitespace() const {
                return All([](char c) { return isspace(c); });
            }

            bool Contains(const char Token) const {
                for (auto Index = 0; Index < backingArrayLength; Index++) {
                    if (backingArray[Index] == Token) {
                        return true;
                    }
                }

                return false;
            }

            bool Contains(const String &Pattern) const {
                UInt32 Count = 0;

                for (auto PatternTokenIndex = 0U; PatternTokenIndex != Pattern.Length(); ++PatternTokenIndex) {
                    for (auto Index = 0; Index != backingArrayLength; ++Index) {
                        if (backingArray[Index] == Pattern[PatternTokenIndex]) {
                            ++Count;
                        }
                    }
                }

                return Count == Pattern.Length();
            }

            String Substring(const char &Start, const char &End) const {
                String Buffer;
                Int32 Index = 0;

                while (true) {
                    if (backingArray[Index] == Start) {
                        ++Index; // Consume the first character so that it is not present in the substring.

                        while (true) {
                            if (backingArray[Index] != End) {
                                Buffer.Append(backingArray[Index]);
                            } else {
                                goto RETURN;
                            }

                            ++Index;
                        }

                        ++Index;
                    }
                }

                RETURN:
                return Buffer;
            }

            String Substring(const UInt32 Start, const UInt32 End) const {
                String Buffer;
                UInt32 Index = 0;

                while (true) {
                    if (Index == Start) {
                        ++Index; // Consume the first character so that it is not present in the substring.

                        while (true) {
                            if (Index != End) {
                                Buffer.Append(backingArray[Index]);
                            } else {
                                goto RETURN;
                            }

                            ++Index;
                        }

                        ++Index;
                    }
                }

                RETURN:
                return Buffer;
            }

            Int32 CountOf(const char Token) const {
                Int32 Count = 0;

                for (auto Index = 0; Index < backingArrayLength; ++Index) {
                    if (backingArray[Index] == Token) {
                        ++Count;
                    }
                }

                return Count;
            }

            // @NOTE: This function expects that the returned array will be deleted when done.
            //        This function allocations on the heap!!!
            String *Splice(const char Token) {
                // Determine how many buffers are needed for the splice.
                // The first splice of the string given the Token will result in two buffers.  One for the LHS string and
                // One for the RHS string.  Each sequential splice (in the case of a string with more than one instance
                // of the specified token) will result in one additional buffer for the RHS of the splice (the LHS of the
                // splice should be the previously created buffer, so it is not recounted). -- NCY, 5.7.2018
                const UInt32 PossibleSplices = CountOf(Token);
                UInt32 NeededBuffers = 2;

                if (PossibleSplices > 1) {
                    for (auto Count = 1U; Count != PossibleSplices; ++Count) {
                        ++NeededBuffers;
                    }
                }

                // Create the needed buffers.
                UInt32 CurrentBuffer = 0;
                String *const Buffers = new String[NeededBuffers];

                UInt32 PreviousEndIndex = 0;
                for (auto Index = 0; Index != backingArrayLength; ++Index) {
                    if (backingArray[Index] == Token) {
                        //PreviousEndIndex = Index;
                        ++CurrentBuffer;
                    } else {
                        Buffers[CurrentBuffer].Append(backingArray[Index]);
                    }
                }

                return Buffers;
            }

            void Append(const char value);
            void Append(const String &value);
            void Append(const char *value);
            void Append(const char *value, const unsigned int length);
            void Insert(const char value, const unsigned int index);
            void Insert(const char *value, const unsigned int index);

            bool Replace(const char *old, const char *nue) {
                int oldLength = std::strlen(old);
                int newLength = std::strlen(nue);

                int count = 0;
                for (auto i = 0U; i < Length(); i++) {
                    if (std::strncmp(&backingArray[i], old, oldLength) == 0) {
                        count++;
                        i += oldLength + 1;
                    }
                }

                if (count) {
                    String oldBuffer(backingArray);

                    VerifyAllocation((*backingArray + (((newLength - oldLength) * count) + 2)), false);

                    int j = 0;
                    for (auto i = 0U; i < oldBuffer.Length(); i++) {
                        if (std::strncmp(&oldBuffer[i], old, oldLength) == 0) {
                            std::memcpy((backingArray + j), nue, newLength);
                            i += oldLength - 1;
                            j += newLength;
                        } else {
                            backingArray[j] = oldBuffer[i];
                            j++;
                        }

                        backingArray[j] = 0;
                        backingArrayLength = std::strlen(backingArray);

                        return true;
                    }

                    return false;
                }
                return true;
            }

            bool Replace(const char old, const char nue) {
                bool wasReplaced = false;

                for (auto i = 0U; i < Length(); i++) {
                    if (backingArray[i] == old) {
                        backingArray[i] = nue;
                        wasReplaced = true;
                    }
                }

                return wasReplaced;
            }

            bool EndsWith(const char *str) const {
                auto len = std::strlen(str);
                if (len > Length()) {
                    return false;
                }

                const char *ptr = &backingArray[Length() - len];

                for (UInt32 i = 0; i < len; i++, ptr++, str++) {
                    if (*ptr != *str) {
                        return false;
                    }
                }

                return true;
            }

            bool StartsWith(const char *str) const {
                auto len = std::strlen(str);
                if (len > Length()) {
                    return false;
                }

                const char *ptr = &backingArray[0];

                for (UInt32 i = 0; i < len; i++, ptr++, str++) {
                    if (*ptr != *str) {
                        return false;
                    }
                }

                return true;
            }

            void ToLowercase();
            void ToUppercase();

            // These could be private?
            inline void CopyRange(const char *str, const unsigned int start, const unsigned int end) {
                int copyRange = end - start;
                if (copyRange < 0) {
                    copyRange = 0;
                }

                VerifyAllocation(copyRange + 1);

                for (auto i = 0; i < copyRange; i++) {
                    backingArray[i] = str[start + i];
                }

                backingArray[copyRange] = '\0';
                backingArrayLength = '\0';
            }

            void ReallocateBackingArray(const unsigned int amount, const bool keepPreviousBuffer) {
                DJINN_ASSERT(amount > 0);

                int newSize = 0;
                int allocMod = amount % DJINN_STRING_ALLOCATION_GRAIN;
                if (!allocMod) {
                    newSize = amount;
                } else {
                    newSize = (amount + DJINN_STRING_ALLOCATION_GRAIN) - allocMod;
                }

                EncodeAllocationBit(newSize);

                // @TODO: Implement Djinn allocator support here once finished.
                auto check = DecodeAllocationBit();
                (void) check; // TODO: Check it?

                char *newBuffer = new char[DecodeAllocationBit()];

                if (keepPreviousBuffer && backingArray != nullptr) {
                    backingArray[backingArrayLength] = '\0';
                    std::strcpy(newBuffer, backingArray);
                }

                if ((backingArray != nullptr) && (backingArray != backingArrayBaseBuffer)) {
                    // @TODO: Implement Djinn allocator support here once finished.
                    delete[] backingArray;
                }

                backingArray = newBuffer;
            }

            char &operator[](int index) const;
            char &operator[](int index);

            void operator=(const String &string);
            void operator=(const char *charArray);

            String &operator+=(const String &rhs);
            String &operator+=(const char *rhs);
            String &operator+=(const float rhs);
            String &operator+=(const char rhs);
            String &operator+=(const Int16 rhs);
            String &operator+=(const Int32 rhs);
            String &operator+=(const Int64 rhs);
            String &operator+=(const UInt16 rhs);
            String &operator+=(const UInt32 rhs);
            String &operator+=(const UInt64 rhs);
            String &operator+=(const bool rhs);

            // Friend operators.
            friend String operator+(const String &lhs, const String &rhs);
            friend String operator+(const String &lhs, const char *rhs);
            friend String operator+(const char *lhs, const String &rhs);
            friend String operator+(const String &lhs, const float rhs);
            friend String operator+(const String &lhs, const int rhs);
            friend String operator+(const String &lhs, const Int16 rhs);
            friend String operator+(const String &lhs, const Int32 rhs);
            friend String operator+(const String &lhs, const Int64 rhs);
            friend String operator+(const String &lhs, const UInt16 rhs);
            friend String operator+(const String &lhs, const UInt32 rhs);
            friend String operator+(const String &lhs, const UInt64 rhs);
            friend String operator+(const String &lhs, const bool rhs);
            friend String operator+(const String &lhs, const char rhs);

            friend bool operator==(const String &lhs, const String &rhs);
            friend bool operator==(const String &lhs, const char *rhs);
            friend bool operator==(const char *lhs, const String &rhs);

            friend bool operator!=(const String &lhs, const String &rhs);
            friend bool operator!=(const String &lhs, const char *rhs);
            friend bool operator!=(const char *lhs, const String &rhs);

            // UTF-8 interface.
            inline unsigned int UTF8_Length() {
                return UTF8_Length(reinterpret_cast< unsigned char * >( backingArray ));
            }

            static unsigned int UTF8_Length(const unsigned char *s) {
                int byteLength = 0;
                int characterLength = 0;

                while (s[byteLength] != '\0') {
                    unsigned int characterIndex;
                    characterIndex = s[byteLength];

                    if (characterIndex < 0x80) {
                        byteLength++;
                    } else {
                        int trailingByte = 0;

                        if (characterIndex >= 0xc0) {
                            static const unsigned char trailingBytes[64] = {
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
                            };

                            trailingByte = trailingBytes[characterIndex - 0xc0];
                        }

                        byteLength += trailingByte + 1;
                    }

                    characterLength++;
                }

                return characterLength;
            }

            inline void UTF8_AppendChar(unsigned int ch) {
                if (ch < 0x80) {
                    Append(static_cast<char>(ch));
                } else if (ch < 0x800) { // 11 bits
                    Append(static_cast<char>( 0xC0 | (ch >> 6)));
                    Append(static_cast<char>( 0x80 | (ch & 0x3F)));
                } else if (ch < 0x10000) { // 16 bits
                    Append(static_cast<char>( 0xE0 | (ch >> 12)));
                    Append(static_cast<char>( 0x80 | ((ch >> 6) & 0x3F)));
                    Append(static_cast<char>( 0x80 | (ch & 0x3F)));
                } else if (ch < 0x200000) {    // 21 bits
                    Append(static_cast<char>( 0xF0 | (ch >> 18)));
                    Append(static_cast<char>( 0x80 | ((ch >> 12) & 0x3F)));
                    Append(static_cast<char>( 0x80 | ((ch >> 6) & 0x3F)));
                    Append(static_cast<char>( 0x80 | (ch & 0x3F)));
                } else {
                    // UTF-8 can encode up to 6 bytes. Why don't we support that?
                    // This is an invalid Unicode character
                    Append('?');
                }
            }

            inline unsigned int UTF8_CharAt(int &index) {
                return UTF8_CharAt(reinterpret_cast< unsigned char * >( backingArray ), index);
            }

            static inline unsigned int UTF8_CharAt(const char *str, int &index) {
                return UTF8_CharAt(reinterpret_cast<const unsigned char *>(str), index);
            }

            static unsigned int UTF8_CharAt(const unsigned char *byte, int &index) {
                if (index >= 0) {
                    while (byte[index] != '\0') {
                        unsigned int characterIndex = byte[index];
                        if (characterIndex < 0x80) {
                            index++;
                            return characterIndex;
                        }

                        int trailing = 0;
                        if (characterIndex >= 0xc0) {
                            static const unsigned char trailingBytes[64] = {
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5
                            };
                            trailing = trailingBytes[characterIndex - 0xc0];
                        }

                        static const unsigned int trailingMask[6] = {0x0000007f, 0x0000001f, 0x0000000f, 0x00000007, 0x00000003, 0x00000001};
                        characterIndex &= trailingMask[trailing];

                        while (trailing-- > 0) {
                            characterIndex <<= 6;
                            characterIndex += byte[++index] & 0x0000003f;
                        }

                        index++;

                        return characterIndex;
                    }
                }

                index++;
                return 0;    // return a null terminator if out of range
            }

            inline void UTF8_EncodeToUTF8() {
                String buffer(*this);

                UTF8_Clear();

                for (auto i = 0U; i < buffer.Length(); ++i) {
                    UTF8_AppendChar(buffer[i]);
                }
            }

            static inline bool UTF8_CheckValidity(const char *s, const int maxLen, StringEncodingScheme &encoding) {
                struct LocalEncoding {
                    static int UTF8_EncodedBytesCount(const unsigned char c) {
                        if (c < 0x80) {
                            return 1;
                        } else if ((c >> 5) == 0x06) {
                            // 2 byte encoding - the next byte must begin with
                            return 2;
                        } else if ((c >> 4) == 0x0E) {
                            // 3 byte encoding
                            return 3;
                        } else if ((c >> 5) == 0x1E) {
                            // 4 byte encoding
                            return 4;
                        }

                        // This isnt' a valid UTF-8 precursor character
                        return 0;
                    }

                    static bool UTF8_DetermineRemainingBytes(const char *s, const int curChar, const int maxLen,
                                                             const int num) {
                        if ((maxLen - curChar) < num) {
                            return false;
                        }

                        for (auto i = (curChar + 1); i <= (curChar + num); i++) {
                            if (s[i] == '\0') {
                                return false;
                            }

                            if ((s[i] >> 6) != 0x02) {
                                return false;
                            }
                        }
                        return true;
                    }
                };

                // Check for byte-order-marker
                encoding = StringEncodingScheme::eASCII;
                StringEncodingScheme utf8Type = StringEncodingScheme::eUTF8_No_BOM;

                if ((maxLen > 3) && (s[0] == static_cast<char>(0xEF)) && (s[1] == static_cast<char>(0xBB)) && (s[2] == static_cast<char>(0xBF))) {
                    utf8Type = StringEncodingScheme::eUTF8_BOM;
                }

                for (int i = 0; ((s[i] != '\0') && (i < maxLen)); i++) {
                    int numBytes = LocalEncoding::UTF8_EncodedBytesCount(s[i]);

                    if (numBytes == 1) {
                        continue;    // just low ASCII
                    } else if (numBytes == 2) {
                        // 2 byte encoding - the next byte must begin with bit pattern 10
                        if (!LocalEncoding::UTF8_DetermineRemainingBytes(s, i, maxLen, 1)) {
                            return false;
                        }

                        // Skip over UTF-8 character
                        i += 1;
                        encoding = utf8Type;
                    } else if (numBytes == 3) {
                        // 3 byte encoding - the next 2 bytes must begin with bit pattern 10
                        if (!LocalEncoding::UTF8_DetermineRemainingBytes(s, i, maxLen, 2)) {
                            return false;
                        }
                        // Skip over UTF-8 character
                        i += 2;
                        encoding = utf8Type;
                    } else if (numBytes == 4) {
                        // 4 byte encoding - the next 3 bytes must begin with bit pattern 10
                        if (!LocalEncoding::UTF8_DetermineRemainingBytes(s, i, maxLen, 3)) {
                            return false;
                        }
                        // Skip over UTF-8 character
                        i += 3;
                        encoding = utf8Type;
                    } else {
                        // This isnt' a valid UTF-8 character
                        if (utf8Type == StringEncodingScheme::eUTF8_BOM) {
                            encoding = StringEncodingScheme::eUTF8_Invalid_BOM;
                        } else {
                            encoding = StringEncodingScheme::eUTF8_Invalid;
                        }

                        return false;
                    }
                }

                return true;
            }

            static inline bool UTF8_CheckValidity(const char *str, const unsigned int maxLength) {
                StringEncodingScheme encoding;

                return UTF8_CheckValidity(str, maxLength, encoding);
            }

        protected:
            void VerifyAllocation(const unsigned int amount, bool keepPreviousBuffer = true);
            void SetStaticBuffer(char *buffer, const unsigned int bufferLength);

        private:
            // Only ever call from a constructor.
            inline void InternalConstruct() {
                EncodeStaticBit(false);
                EncodeAllocationBit(DJINN_STRING_ALLOCATION_BASE);

                backingArray = backingArrayBaseBuffer;
                backingArrayLength = 0;
                backingArray[0] = '\0';
            }

            void ReleaseBackingArray() {
                // DO NOT RELEASE A FUCKING COMPILE TIME STATIC STRING!!!
                if (DecodeStaticBit()) {
                    return;
                }

                if ((backingArray != nullptr) && (backingArray != backingArrayBaseBuffer)) {
                    delete[] backingArray;
                }

                backingArray = backingArrayBaseBuffer;
            }

            void UTF8_Clear() {

            }

            inline int DecodeAllocationBit() const {
                return maskFlag & ALLOCATION_MASK;
            }

            inline void EncodeAllocationBit(const int value) {
                maskFlag = (maskFlag & STATIC_MASK) | (value & ALLOCATION_MASK);
            }

            inline bool DecodeStaticBit() const {
                return (maskFlag & STATIC_MASK) != 0;
            }

            inline void EncodeStaticBit(const bool isStatic) {
                maskFlag = (maskFlag & ALLOCATION_MASK) | (isStatic << STATIC_BIT);
            }

        protected:
            char *backingArray;
            char backingArrayBaseBuffer[DJINN_STRING_ALLOCATION_BASE];
            int backingArrayLength;

            int maskFlag = 0; // Top bit stores static state.
        };

        inline void String::VerifyAllocation(const unsigned int amount, bool keepPreviousBuffer) {
            // @Note: Static string buffers can't be reallocated.
            //if( DecodeStaticBit() ) {
            // Release and assert.
            //    return;
            //}

            if (amount > static_cast<unsigned>(DecodeAllocationBit())) {
                ReallocateBackingArray(amount, keepPreviousBuffer);
            }
        }

// Static buffer interface.
        inline void String::SetStaticBuffer(char *buffer, const unsigned int bufferLength) {
            DJINN_ASSERT(buffer == backingArrayBaseBuffer);

            backingArray = buffer;
            backingArrayLength = 0;

            EncodeAllocationBit(bufferLength);
            EncodeStaticBit(true);
        }

        inline String::String() {
            InternalConstruct();
        }

        inline String::String(const String &string) {
            InternalConstruct();
            const int stringLength = string.Length();

            VerifyAllocation(stringLength + 1);
            std::strcpy(backingArray, string.backingArray);

            backingArrayLength = stringLength;
        }

        inline String::String(const String &string, int start, int end) {
            InternalConstruct();

            // Handle out of range bullshit.
            if (start > static_cast<int>(string.Length())) {
                start = static_cast<int>(string.Length());
            } else if (start < 0) {
                start = 0;
            }

            if (end > static_cast<int>(string.Length()) || end < start) {
                end = static_cast<int>(string.Length());
            }

            unsigned int copyRange = static_cast<unsigned>(end - start);
            /*if( copyRange < 0 ) {
                copyRange = 0;
            }*/

            VerifyAllocation(copyRange + 1);

            for (auto index = 0U; index < copyRange; index++) {
                backingArray[index] = string[start + index];
            }

            backingArray[copyRange] = '\0';
            backingArrayLength = copyRange;
        }

        inline String::String(const char *charArray) {
            InternalConstruct();

            if (charArray != nullptr) {
                const unsigned int length = static_cast<unsigned>(std::strlen(charArray));

                VerifyAllocation(length + 1);
                std::strcpy(backingArray, charArray);

                backingArrayLength = length;
            }
        }

        inline String::String(const char *charArray, int start, int end) {
            InternalConstruct();

            unsigned int length = static_cast<unsigned>(std::strlen(charArray));

            if (start < 0) {
                start = 0;
            }

            length = static_cast<unsigned>(end - start);

            /*if( length < 0 ) {
                length = 0;
            }*/

            VerifyAllocation(length + 1);

            for (auto index = 0U; index < length; index++) {
                backingArray[index] = charArray[start + index];
            }

            backingArray[length] = '\0';
            backingArrayLength = length;
        }

        inline String::String(const bool b) {
            InternalConstruct();
            VerifyAllocation(2);

            backingArray[0] = b ? '1' : '0';
            backingArray[1] = '\0';

            backingArrayLength = 1;
        }

        inline String::String(const char c) {
            InternalConstruct();
            VerifyAllocation(2);

            backingArray[0] = c;
            backingArray[1] = '\0';

            backingArrayLength = 1;
        }

        inline String::String(const Int16 i) {
            InternalConstruct();

            char buffer[64];
            const unsigned int length = std::sprintf(buffer, "%d", i);

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const Int32 i) {
            InternalConstruct();

            char buffer[64];
            const unsigned int length = std::sprintf(buffer, "%d", i);

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const Int64 i) {
            InternalConstruct();

            char buffer[64];
            const unsigned int length = std::sprintf(buffer, "%lld", i);

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const ULong i) {
            InternalConstruct();

            char buffer[64];
            const unsigned int length = std::sprintf(buffer, "%lu", i);

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const UInt16 i) {
            InternalConstruct();

            char buffer[64];
            const unsigned int length = std::sprintf(buffer, "%u", i);

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const UInt32 i) {
            InternalConstruct();

            char buffer[64];
            const unsigned int length = std::sprintf(buffer, "%u", i);

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const UInt64 i) {
            InternalConstruct();

            char buffer[64];
            const unsigned int length = std::sprintf(buffer, "%llu", i);

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const float f) {
            InternalConstruct();

            char buffer[64];
            unsigned int length = std::sprintf(buffer/*, sizeof( buffer )*/, "%f", f);

            while ((length > 0) && (buffer[length - 1] == '0')) {
                buffer[--length] = '\0';
            }

            while ((length > 0) && (buffer[length - 1] == '.')) {
                buffer[--length] = '\0';
            }

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::String(const double f) {
            InternalConstruct();

            char buffer[64];
            unsigned int length = std::sprintf(buffer/*, sizeof( buffer )*/, "%f", f);

            while ((length > 0) && (buffer[length - 1] == '0')) {
                buffer[--length] = '\0';
            }

            while ((length > 0) && (buffer[length - 1] == '.')) {
                buffer[--length] = '\0';
            }

            VerifyAllocation(length + 1);

            std::strcpy(backingArray, buffer);
            backingArrayLength = length;
        }

        inline String::~String() {
            ReleaseBackingArray();
        }

        inline void String::Flush() {
            VerifyAllocation(1);

            backingArray[0] = '\0';
            backingArrayLength = 0;
        }

        inline void String::FlushStatic() {
            backingArrayLength = 0;
            backingArray[0] = '\0';
        }

        inline void String::Append(const char c) {
            VerifyAllocation(backingArrayLength + 2);

            backingArray[backingArrayLength] = c;
            backingArrayLength++;
            backingArray[backingArrayLength] = '\0';
        }

        inline void String::Append(const String &s) {
            unsigned int newLength = backingArrayLength + s.Length();

            VerifyAllocation(newLength + 1);

            for (auto index = 0U; index < s.Length(); index++) {
                backingArray[backingArrayLength + index] = s[index];
            }

            backingArrayLength = newLength;
            backingArray[backingArrayLength] = '\0';
        };

        inline void String::Append(const char *charArray) {
            int newLength = 0;

            if (charArray != nullptr) {
                newLength = backingArrayLength + std::strlen(charArray);

                VerifyAllocation(newLength + 1);

                for (auto index = 0; charArray[index]; index++) {
                    backingArray[backingArrayLength + index] = charArray[index];
                }

                backingArrayLength = newLength;
                backingArray[backingArrayLength] = '\0';
            }
        }

        inline void String::Append(const char *charArray, unsigned int length) {
            unsigned int newLength = 0;

            if ((charArray != nullptr) && length) {
                newLength = backingArrayLength + length;

                VerifyAllocation(newLength + 1);

                for (auto index = 0; ((charArray[index]) && (index < 1)); index++) {
                    backingArray[length + index] = charArray[index];
                }

                backingArrayLength = newLength;
                backingArray[backingArrayLength] = '\0';
            }
        }

        inline void String::Insert(const char a, unsigned int index) {
            // Patch (Miguel): Case always false
            /*if( index < 0 ) {
                index = 0;
            } else */if (static_cast<int>(index) > backingArrayLength) {
                index = backingArrayLength;
            }

            int l = 1;

            VerifyAllocation((backingArrayLength + l) + 1);

            for (auto i = backingArrayLength; i >= static_cast<int>(index); i--) {
                backingArray[i + l] = backingArray[i];
            }

            backingArray[index] = a;
            backingArrayLength++;
        }

        inline void String::Insert(const char *charArray, unsigned int index) {
            // Patch (Miguel): Case always false
            /*if( index < 0 ) {
                index = 0;
            } else*/ if (static_cast<int>(index) > backingArrayLength) {
                index = backingArrayLength;
            }

            int l = std::strlen(charArray);

            VerifyAllocation((backingArrayLength + l) + 1);

            for (auto i = backingArrayLength; i >= static_cast<int>(index); i--) {
                backingArray[i + l] = backingArray[i];
            }

            for (auto i = 0; i < l; i++) {
                backingArray[index + i] = charArray[i];
            }

            backingArrayLength += l;
        }

        inline void String::ToLowercase() {
            for (auto i = 0; backingArray[i]; i++) {
                if (IsCharacterUppercase(backingArray[i])) {
                    backingArray[i] += 'a' - 'A';
                }
            }
        }

        inline void String::ToUppercase() {
            for (auto i = 0; backingArray[i]; i++) {
                if (IsCharacterLowercase(backingArray[i])) {
                    backingArray[i] -= 'a' - 'A';
                }
            }
        }

// Inline operators.
        inline char &String::operator[](int index) const {
            DJINN_ASSERT((index >= 0) && (index <= backingArrayLength));
            return backingArray[index];
        }

        inline char &String::operator[](int index) {
            DJINN_ASSERT((index >= 0) && (index <= backingArrayLength));
            return backingArray[index];
        }

        inline void String::operator=(const String &string) {
            const unsigned int length = string.Length();

            VerifyAllocation((length + 1), false);

            std::memcpy(backingArray, string.backingArray, length);
            backingArray[length] = '\0';
            backingArrayLength = length;
        }

        inline String &String::operator+=(const float f) {
            char buffer[64];

            std::sprintf(buffer, "%f", f);
            Append(buffer);

            return *this;
        }

        inline String &String::operator+=(const Int16 i) {
            char buffer[64];

            std::sprintf(buffer, "%d", i);
            Append(buffer);

            return *this;
        }

        inline String &String::operator+=(const Int32 i) {
            char buffer[64];

            std::sprintf(buffer, "%d", i);
            Append(buffer);

            return *this;
        }

        inline String &String::operator+=(const Int64 i) {
            char buffer[64];

            std::sprintf(buffer, "%lld", i);
            Append(buffer);

            return *this;
        }

        inline String &String::operator+=(const UInt16 i) {
            char buffer[64];

            std::sprintf(buffer, "%d", i);
            Append(buffer);

            return *this;
        }

        inline String &String::operator+=(const UInt32 i) {
            char buffer[64];

            std::sprintf(buffer, "%d", i);
            Append(buffer);

            return *this;
        }

        inline String &String::operator+=(const UInt64 i) {
            char buffer[64];

            std::sprintf(buffer, "%llu", i);
            Append(buffer);

            return *this;
        }

        inline String &String::operator+=(const String &s) {
            Append(s);

            return *this;
        }

        inline String &String::operator+=(const char *charArray) {
            Append(charArray);

            return *this;
        }

        inline String &String::operator+=(const char c) {
            Append(c);

            return *this;
        }

        inline String &String::operator+=(const bool b) {
            Append(b ? "true" : "false");

            return *this;
        }

        inline void String::operator=(const char *rhs) {
            // Define safe behaviour for when the rhs if the operator is null.
            if (rhs == nullptr) {
                VerifyAllocation(1, false);

                backingArray[0] = '\0';
                backingArrayLength = 0;

                return;
            }

            if (backingArray == rhs) {
                return; // We're checking against the same string.
            }

            // Check UTF8 aliasing.
            if ((rhs >= backingArray) && (rhs <= (backingArray + backingArrayLength))) {
                int difference = rhs - backingArray;

                //DJINN_ASSERT( std::strlen( rhs ) <= ( backingArray + backingArrayLength ) );

                int i = 0;
                for (i = 0; rhs[i]; i++) {
                    backingArray[i] = rhs[i];
                }

                backingArray[i] = '\0';

                backingArrayLength -= difference;

                return;
            }

            int length = std::strlen(rhs);

            VerifyAllocation((length + 1), false);

            std::strcpy(backingArray, rhs);
            backingArrayLength = length;
        }

// Global operators.
        inline String operator+(const String &lhs, const String &rhs) {
            String result(lhs);
            result.Append(rhs);

            return result;
        }

        inline String operator+(const String &string, const char *b) {
            String result(string);
            result.Append(b);

            return result;
        }

        inline String operator+(const char *b, const String &string) {
            String result(b);
            result.Append(string);

            return result;
        }

        inline String operator+(const String &string, const float f) {
            char buffer[64];
            String result(string);

            std::sprintf(buffer, "%f", f);
            result.Append(buffer);

            return result;
        }

        inline String operator+(const String &string, const Int16 i) {
            char buffer[64];
            String result(string);

            std::sprintf(buffer, "%d", i);
            result.Append(buffer);

            return result;
        }

        inline String operator+(const String &string, const Int32 i) {
            char buffer[64];
            String result(string);

            std::sprintf(buffer, "%d", i);
            result.Append(buffer);

            return result;
        }

        inline String operator+(const String &string, const Int64 i) {
            char buffer[64];
            String result(string);

            std::sprintf(buffer, "%lld", i);
            result.Append(buffer);

            return result;
        }

        inline String operator+(const String &string, const UInt16 i) {
            char buffer[64];
            String result(string);

            std::sprintf(buffer, "%d", i);
            result.Append(buffer);

            return result;
        }

        inline String operator+(const String &string, const UInt32 i) {
            char buffer[64];
            String result(string);

            std::sprintf(buffer, "%d", i);
            result.Append(buffer);

            return result;
        }

        inline String operator+(const String &string, const UInt64 i) {
            char buffer[64];
            String result(string);

            std::sprintf(buffer, "%llu", i);
            result.Append(buffer);

            return result;
        }

        inline String operator+(const String &string, const bool b) {
            String result(string);
            result.Append(b ? "true" : "false");

            return result;
        }

        inline String operator+(const String &string, const char ch) {
            String result(string);
            result.Append(ch);

            return result;
        }

        inline bool operator==(const String &lhs, const String &rhs) {
            return !std::strcmp(lhs.backingArray, rhs.backingArray);
        }

        inline bool operator==(const String &lhs, const char *rhs) {
            DJINN_ASSERT(rhs, "Bad operand");
            return !std::strcmp(lhs.backingArray, rhs);
        }

        inline bool operator==(const char *lhs, const String &rhs) {
            DJINN_ASSERT(lhs, "Bad operand");
            return !std::strcmp(lhs, rhs.backingArray);
        }

        inline bool operator!=(const String &lhs, const String &rhs) {
            return !(lhs == rhs);
        }

        inline bool operator!=(const String &lhs, const char *rhs) {
            return !(lhs == rhs);
        }

        inline bool operator!=(const char *rhs, const String &lhs) {
            return !(rhs == lhs);
        }

DJINN_END_NS2(Djinn, Core)

namespace Djinn {
    template<>
    struct Hasher<Core::String> {
        USize operator()(const Core::String &str) const { return Hasher<const char *>{}(str.AsCharArray()); }
    };
}