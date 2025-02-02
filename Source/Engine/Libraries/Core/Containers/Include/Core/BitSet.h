#pragma once

namespace Djinn::Core {
    /// Bit set, represents a collection of values
    /// \tparam T - Underlying enum value, must not have bitwise collisions
    template<class T>
    class BitSet {
    public:
        /// Construct from default
        /// \param value
        BitSet(T value = T()) : value(value) {

        }

        /// Get value
        /// \return
        operator const T &() const {
            return value;
        }

        /// Get value
        /// \return
        operator T &() {
            return value;
        }

        /// Compare with other value
        /// \param v
        /// \return
        bool operator==(const T &v) {
            return value == v;
        }

        /// Compare with other value
        /// \param v
        /// \return
        bool operator!=(const T &v) {
            return value != v;
        }

        /// Add a bit value
        /// \param v
        /// \return
        BitSet &operator+=(const T &v) {
            value = static_cast<T>(static_cast<int>(v) | static_cast<int>(value));
            return *this;
        }

        /// Add a bit value to a new set
        /// \param v
        /// \return
        BitSet operator+(const T &v) {
            return BitSet(static_cast<T>(static_cast<int>(v) | static_cast<int>(value)));
        }

        /// Remove a bit value
        /// \param v
        /// \return
        BitSet &operator-=(const T &v) {
            value = static_cast<T>(static_cast<int>(v) & ~static_cast<int>(value));
            return *this;
        }

        /// Remove a bit value to a new set
        /// \param v
        /// \return
        BitSet operator-(const T &v) {
            return BitSet(static_cast<T>(static_cast<int>(v) & ~static_cast<int>(value)));
        }

        /// Add a bit set
        /// \param v
        /// \return
        BitSet &operator+=(const BitSet<T> &v) {
            value = static_cast<T>(static_cast<int>(v.value) | static_cast<int>(value));
            return *this;
        }

        /// Add a bit set to a new set
        /// \param v
        /// \return
        BitSet operator+(const BitSet<T> &v) {
            return BitSet(static_cast<T>(static_cast<int>(v.value) | static_cast<int>(value)));
        }

        /// Remove a bit set
        /// \param v
        /// \return
        BitSet &operator-=(const BitSet<T> &v) {
            value = static_cast<T>(static_cast<int>(v.value) & ~static_cast<int>(value));
            return *this;
        }

        /// Remove a bit set to a new set
        /// \param v
        /// \return
        BitSet operator-(const BitSet<T> &v) {
            return BitSet(static_cast<T>(static_cast<int>(v.value) & ~static_cast<int>(value)));
        }

        /// Test if a bit value is present
        /// \param v
        /// \return
        bool operator&(const T &v) const {
            return (static_cast<int>(value) & static_cast<int>(v)) != 0;
        }

        /// Is this bit set empty?
        /// \return
        bool IsEmpty() {
            return static_cast<int>(value) == 0;
        }

        /// Get set
        /// \return
        T Get() const {
            return value;
        }

    private:
        T value;
    };

    /// Declare bit set operations for enum
#define DJINN_CORE_BITSET(NAME) \
    using NAME##Set = Djinn::Core::BitSet<NAME>; \
    static NAME##Set operator|(const NAME& l, const NAME& r) { return NAME##Set(l) + r; }
}