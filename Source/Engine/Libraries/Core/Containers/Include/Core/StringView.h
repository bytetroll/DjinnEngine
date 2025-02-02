#pragma once

#include <Core/Template/Limits.h>
#include "String.h"

namespace Djinn::Core {
    class StringView {
    public:
        /// Construct
        StringView() : data(nullptr), length(0) {

        }

        StringView(const StringView& other) : data(other.data), length(other.length) {

        }

        /// Construct
        /// \param array
        StringView(const String &str) : data(str.AsCharArray()), length(str.Length()) {

        }

        /// Construct
        /// \param array
        /// \param size
        StringView(const char *array, USize size) : data(array), length(size) {

        }

        /// Construct, calculates length of string
        /// \param array
        /// \param size
        StringView(const char *array) : data(array), length(array ? std::strlen(array) : 0) {

        }

        /// To string
        /// \return
        explicit operator Core::String() const {
            return ToString();

        }

        /// To string
        /// \return
        Core::String ToString() const {
            return Core::String(data, 0, length);
        }

        /// Length of this string
        /// \return
        DJINN_INLINE USize Length() const {
            return length;
        }

        /// Get character at index
        /// \param index
        /// \return
        DJINN_INLINE char Get(USize index) const {
            DJINN_ASSERT(index < length, "Index out of bounds");
            return data[index];
        }

        /// Get character at index
        /// \param index
        /// \return
        DJINN_INLINE char operator[](USize index) const {
            DJINN_ASSERT(index < length, "Index out of bounds");
            return data[index];
        }

        /// Get the index of
        /// \param c
        /// \return
        Int32 IndexOf(char c) const {
            for (auto i = 0U; i < length; i++) {
                if (data[i] == c) {
                    return i;
                }
            }
            return -1;
        }

        /// Get last index of
        /// \param c
        /// \return
        Int32 LastIndexOf(char c) const {
            for (Int32 i = length - 1; i >= 0; i--) {
                if (data[i] == c) {
                    return i;
                }
            }
            return -1;
        }

        /// Get last index of
        /// \param c
        /// \return
        Int32 LastIndexOf(const char *c) const {
            auto n = std::strlen(c);
            for (Int32 i = length - 1; i >= 0; i--) {
                for (USize j = 0; j < n; j++) {
                    if (data[i] == c[j]) {
                        return i;
                    }
                }
            }
            return -1;
        }

        /// Check if this string starts with
        /// \param str
        /// \return
        bool StartsWith(const char *str) const {
            auto len = std::strlen(str);
            if (len > length) {
                return false;
            }

            const char *ptr = &data[0];

            for (UInt32 i = 0; i < len; i++, ptr++, str++) {
                if (*ptr != *str) {
                    return false;
                }
            }

            return true;
        }

        /// Check if this string ends with
        /// \param str
        /// \return
        bool EndsWith(const char *str) const {
            auto len = std::strlen(str);
            if (len > length) {
                return false;
            }

            const char *ptr = &data[length - len];

            for (UInt32 i = 0; i < len; i++, ptr++, str++) {
                if (*ptr != *str) {
                    return false;
                }
            }

            return true;
        }

        /// Create a sub string view from this view
        /// \param begin
        /// \param length
        /// \return
        StringView SubStr(USize begin, USize length) const {
            DJINN_ASSERT(begin + length <= length, "Invalid sub string range");
            return StringView(data + begin, length);
        }

        /// Create a sub string view from this view
        /// \param begin
        /// \param length
        /// \return
        StringView SubStr(USize begin) const {
            DJINN_ASSERT(begin <= length, "Invalid sub string range");
            return StringView(data + begin, length - begin);
        }

        /// Compare
        /// \param other
        /// \return
        bool operator==(const StringView& other) const {
            if (length != other.length) {
                return false;
            }

            for (USize i = 0; i < length; i++) {
                if (data[i] != other.data[i]) {
                    return false;
                }
            }

            return true;
        }

        /// Compare
        /// \param other
        /// \return
        bool operator==(const char* other) const {
            if (length != std::strlen(other)) {
                return false;
            }

            for (USize i = 0; i < length; i++) {
                if (data[i] != other[i]) {
                    return false;
                }
            }

            return true;
        }

        /// Compare
        /// \param other
        /// \return
        bool operator==(const Core::String& other) const {
            if (length != other.Length()) {
                return false;
            }

            for (USize i = 0; i < length; i++) {
                if (data[i] != other[i]) {
                    return false;
                }
            }

            return true;
        }

        /// Compare
        /// \param other
        /// \return
        bool operator!=(const char* other) const {
            return !(*this == other);
        }

        /// Compare
        /// \param other
        /// \return
        bool operator!=(const Core::String& other) const {
            return !(*this == other);
        }

        /// Get data pointer
        /// \return
        const char *Ptr() const {
            return data;
        }

        /// Get end pointer
        /// \return
        const char *End() const {
            return data + length;
        }

        /// Compute hash of this string view
        /// \return
        USize Hash() const {
            return Core::ComputeCRC64(data, data + length);
        }
        
        // ...
        const char *data;
        USize length;
    };

    class HashStringView {
    public:
        HashStringView() : hash(0) {

        }

        HashStringView(const StringView& view) : view(view) {
            hash = view.Hash();
        }

        /// To string
        /// \return
        Core::String ToString() const {
            return view.ToString();
        }

        /// Get the view
        /// \return
        const StringView& GetView() const {
            return view;
        }

        /// Get the hash of this view
        /// \return
        USize GetHash() const {
            return hash;
        }

    private:
        StringView view;
        USize hash;
    };
}

namespace Djinn {
    template<>
    struct Hasher<Core::StringView> {
        USize operator()(const Core::StringView &str) const { return Core::ComputeCRC64(str.Ptr(), str.Ptr() + str.Length()); }
    };
}