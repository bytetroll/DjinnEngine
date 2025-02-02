//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Core/Math.h>
#include <Core/Types.h>

namespace Djinn::Math {
    struct EVector2IIndexOOB {
    };

    class Vector2I {
    public:
        Vector2I() {

        }

        Vector2I(Int32 x, Int32 y) : x(x), y(y) {
        }

        bool operator==(const Vector2I& v) {
            return x == v.x && y == v.y;
        }

        bool operator!=(const Vector2I& v) {
            return x != v.x || y != v.y;
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector2I operator+(const Vector2I &rhs) const {
            return Vector2I(x + rhs.x, y + rhs.y);
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector2I operator+(Int32 rhs) const {
            return Vector2I(x + rhs, y + rhs);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector2I operator-(const Vector2I &rhs) const {
            return Vector2I(x - rhs.x, y - rhs.y);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector2I operator-(Int32 rhs) const {
            return Vector2I(x - rhs, y - rhs);
        }

        /// Negate this vector
        /// \return
        Vector2I operator-() const {
            return Vector2I(-x, -y);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector2I operator/(const Vector2I &rhs) const {
            return Vector2I(x / rhs.x, y / rhs.y);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector2I operator/(Int32 rhs) const {
            return Vector2I(x / rhs, y / rhs);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector2I operator*(const Vector2I &rhs) const {
            return Vector2I(x * rhs.x, y * rhs.y);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector2I operator*(Int32 rhs) const {
            return Vector2I(x * rhs, y * rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2I &operator+=(const Vector2I &rhs) {
            return Add(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2I &operator+=(Int32 rhs) {
            return Add(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2I &operator-=(const Vector2I &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2I &operator-=(Int32 rhs) {
            return Sub(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2I &operator/=(const Vector2I &rhs) {
            return Div(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2I &operator/=(Int32 rhs) {
            return Div(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2I &operator*=(const Vector2I &rhs) {
            return Mul(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2I &operator*=(Int32 rhs) {
            return Mul(rhs);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        Int32 operator|(const Vector2I &rhs) {
            return Dot(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2I &Add(const Vector2I &rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2I &Add(Int32 rhs) {
            x += rhs;
            y += rhs;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2I &Sub(const Vector2I &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2I &Sub(Int32 rhs) {
            x -= rhs;
            y -= rhs;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2I &Div(const Vector2I &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2I &Div(Int32 rhs) {
            x /= rhs;
            y /= rhs;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2I &Mul(const Vector2I &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2I &Mul(Int32 rhs) {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        Int32 Dot(const Vector2I &rhs) {
            return x * rhs.x + y * rhs.y;
        }

        /// Calculate normalized vector
        /// \return
        Vector2I Normalized() {
            return Vector2I(x, y).Normalize();
        }

        /// Normalize this vector
        /// \return
        Vector2I &Normalize() {
            Int32 len = Length();
            x /= len;
            y /= len;
            return *this;
        }

        /// Calculate squared length
        /// \return
        Int32 LengthSqr() {
            return x * x + y * y;
        }

        /// Calculate length
        /// \return
        Int32 Length() {
            return Math::Sqrt(LengthSqr());
        }

        /// Get data pointer
        /// \return
        Int32 *Ptr() {
            return &x;
        }

        /// Get element
        /// \param i
        /// \return
        Int32 &operator[](int i) {
            switch (i) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    DJINN_THROW EVector2IIndexOOB();
            }
        }

        /// Get element
        /// \param i
        /// \return
        Int32 operator[](int i) const {
            switch (i) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    DJINN_THROW EVector2IIndexOOB();
            }
        }

        // Components
        Int32 x;
        Int32 y;
    };
}