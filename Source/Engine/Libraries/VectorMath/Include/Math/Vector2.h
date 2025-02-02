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
 on 10/23/2017.
//
#pragma once

#include <Math/Detail/DirectXMath/Inc/DirectXMath.h>
#include <Core/Math.h>

namespace Djinn::Math {
    struct EVector2IndexOOB {
    };

    class Vector2 {
    public:
        Vector2() : x(0), y(0) {

        }

        Vector2(float x, float y) : x(x), y(y) {
        }

        /// Get the dot product of two vectors
        /// \param a
        /// \param b
        /// \return
        static float Dot(const Vector2 &a, const Vector2 &b) {
            return a.x * b.x + a.y * b.y;
        }

        /// Get the per component minimum of two vectors
        /// \param a
        /// \param b
        /// \return
        static Vector2 Min(const Vector2 &a, const Vector2 &b) {
            return {
                    a.x < b.x ? a.x : b.x,
                    a.y < b.y ? a.y : b.y
            };
        }

        /// Get the per component maximum of two vectors
        /// \param a
        /// \param b
        /// \return
        static Vector2 Max(const Vector2 &a, const Vector2 &b) {
            return {
                    a.x >= b.x ? a.x : b.x,
                    a.y >= b.y ? a.y : b.y
            };
        }

        /// Get the per component minimum of N vectors
        /// \param ...
        /// \return
        template< class... N>
        static Vector2 Min(const Vector2 &a, const Vector2 &b, const Vector2 &c, const N &... rest) {
            return Min(Min(a, b), c, rest...);
        }

        /// Get the per component maximum of N vectors
        /// \param ...
        /// \return
        template< class... N>
        static Vector2 Max(const Vector2 &a, const Vector2 &b, const Vector2 &c, const N &... rest) {
            return Max(Max(a, b), c, rest...);
        }

        /// Saturate a vector per component between 0 - 1
        /// \param v
        /// \return
        static Vector2 Saturate(const Vector2& v) {
            return {
                    Math::Saturate(v.x),
                    Math::Saturate(v.y)
            };
        }

        /// Saturate a vector per component between 0 - inf
        /// \param v
        /// \return
        static Vector2 SaturateNegative(const Vector2& v) {
            return {
                    Math::SaturateNegative(v.x),
                    Math::SaturateNegative(v.y)
            };
        }

        /// Take the per component absolute value
        /// \param v
        /// \return
        static Vector2 Abs(const Vector2& v) {
            return {
                    Math::Abs(v.x),
                    Math::Abs(v.y)
            };
        }

        /// Compare
        /// \param rhs
        /// \return
        bool operator==(const Vector2 &rhs) const {
            return x == rhs.x && y == rhs.y;
        }

        /// Compare
        /// \param rhs
        /// \return
        bool operator!=(const Vector2 &rhs) const {
            return x != rhs.x || y != rhs.y;
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector2 operator+(const Vector2 &rhs) const {
            return Vector2(x + rhs.x, y + rhs.y);
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector2 operator+(float rhs) const {
            return Vector2(x + rhs, y + rhs);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector2 operator-(const Vector2 &rhs) const {
            return Vector2(x - rhs.x, y - rhs.y);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector2 operator-(float rhs) const {
            return Vector2(x - rhs, y - rhs);
        }

        /// Negate this vector
        /// \return
        Vector2 operator-() const {
            return Vector2(-x, -y);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector2 operator/(const Vector2 &rhs) const {
            return Vector2(x / rhs.x, y / rhs.y);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector2 operator/(float rhs) const {
            return Vector2(x / rhs, y / rhs);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector2 operator*(const Vector2 &rhs) const {
            return Vector2(x * rhs.x, y * rhs.y);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector2 operator*(float rhs) const {
            return Vector2(x * rhs, y * rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2 &operator+=(const Vector2 &rhs) {
            return Add(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2 &operator+=(float rhs) {
            return Add(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2 &operator-=(const Vector2 &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2 &operator-=(float rhs) {
            return Sub(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2 &operator/=(const Vector2 &rhs) {
            return Div(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2 &operator/=(float rhs) {
            return Div(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2 &operator*=(const Vector2 &rhs) {
            return Mul(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2 &operator*=(float rhs) {
            return Mul(rhs);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float operator|(const Vector2 &rhs) {
            return Dot(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2 &Add(const Vector2 &rhs) {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector2 &Add(float rhs) {
            x += rhs;
            y += rhs;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2 &Sub(const Vector2 &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector2 &Sub(float rhs) {
            x -= rhs;
            y -= rhs;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2 &Div(const Vector2 &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector2 &Div(float rhs) {
            x /= rhs;
            y /= rhs;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2 &Mul(const Vector2 &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector2 &Mul(float rhs) {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float Dot(const Vector2 &rhs) {
            return x * rhs.x + y * rhs.y;
        }

        /// Calculate normalized vector
        /// \return
        Vector2 Normalized() {
            return Vector2(x, y).Normalize();
        }

        /// Normalize this vector
        /// \return
        Vector2 &Normalize() {
            float len = Length();
            x /= len;
            y /= len;
            return *this;
        }

        /// Calculate squared length
        /// \return
        float LengthSqr() {
            return x * x + y * y;
        }

        /// Calculate length
        /// \return
        float Length() {
            return Math::Sqrt(LengthSqr());
        }

        /// Get data pointer
        /// \return
        float *Ptr() {
            return &x;
        }

        /// Get element
        /// \param i
        /// \return
        float &operator[](int i) {
            switch (i) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    DJINN_THROW EVector2IndexOOB();
            }
        }

        /// Get element
        /// \param i
        /// \return
        float operator[](int i) const {
            switch (i) {
                case 0:
                    return x;
                case 1:
                    return y;
                default:
                    DJINN_THROW EVector2IndexOOB();
            }
        }

        /// Helper
        DirectX::XMVECTOR __Load() const {
            return DirectX::XMVectorSet(x, y, 0, 0);
        }

        // Components
        float x;
        float y;
    };
}