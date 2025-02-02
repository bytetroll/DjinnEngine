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
#include "Vector3.h"

namespace Djinn::Math {
    struct EVector4IndexOOB {
    };

    class Vector4 {
    public:
        Vector4() : x(0), y(0), z(0), w(0) {

        }

        Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
        }

        Vector4(const Vector3 &v, float w) : x(v.x), y(v.y), z(v.z), w(w) {
        }

        Vector4(const Vector2 &x, const Vector2 &y) : x(x.x), y(x.y), z(y.x), w(y.y) {
        }

        Vector4(const Vector2 &x, float z, float w) : x(x.x), y(x.y), z(z), w(w) {
        }

        /// Get the dot product of two vectors
        /// \param a
        /// \param b
        /// \return
        static float Dot(const Vector4 &a, const Vector4 &b) {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        /// Get the per component minimum of two vectors
        /// \param a
        /// \param b
        /// \return
        static Vector4 Min(const Vector4 &a, const Vector4 &b) {
            return {
                    a.x < b.x ? a.x : b.x,
                    a.y < b.y ? a.y : b.y,
                    a.y < b.y ? a.y : b.y,
                    a.w < b.w ? a.w : b.w
            };
        }

        /// Get the per component maximum of two vectors
        /// \param a
        /// \param b
        /// \return
        static Vector4 Max(const Vector4 &a, const Vector4 &b) {
            return {
                    a.x >= b.x ? a.x : b.x,
                    a.y >= b.y ? a.y : b.y,
                    a.z >= b.z ? a.z : b.z,
                    a.w >= b.w ? a.w : b.w
            };
        }

        /// Get the per component minimum of N vectors
        /// \param ...
        /// \return
        template< class... N>
        static Vector4 Min(const Vector4 &a, const Vector4 &b, const Vector4 &c, const N &... rest) {
            return Min(Min(a, b), c, rest...);
        }

        /// Get the per component maximum of N vectors
        /// \param ...
        /// \return
        template< class... N>
        static Vector4 Max(const Vector4 &a, const Vector4 &b, const Vector4 &c, const N &... rest) {
            return Max(Max(a, b), c, rest...);
        }

        /// Saturate a vector per component between 0 - 1
        /// \param v
        /// \return
        static Vector4 Saturate(const Vector4& v) {
            return {
                    Math::Saturate(v.x),
                    Math::Saturate(v.y),
                    Math::Saturate(v.z),
                    Math::Saturate(v.w)
            };
        }

        /// Saturate a vector per component between 0 - inf
        /// \param v
        /// \return
        static Vector4 SaturateNegative(const Vector4& v) {
            return {
                    Math::SaturateNegative(v.x),
                    Math::SaturateNegative(v.y),
                    Math::SaturateNegative(v.z),
                    Math::SaturateNegative(v.w)
            };
        }

        /// Take the per component absolute value
        /// \param v
        /// \return
        static Vector4 Abs(const Vector4& v) {
            return {
                    Math::Abs(v.x),
                    Math::Abs(v.y),
                    Math::Abs(v.z),
                    Math::Abs(v.w)
            };
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector4 operator+(const Vector4 &rhs) const {
            return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector4 operator+(float rhs) const {
            return Vector4(x + rhs, y + rhs, z + rhs, w + rhs);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector4 operator-(const Vector4 &rhs) const {
            return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector4 operator-(float rhs) const {
            return Vector4(x - rhs, y - rhs, z - rhs, w - rhs);
        }

        /// Negate this vector
        /// \return
        Vector4 operator-() const {
            return Vector4(-x, -y, -z, -w);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector4 operator/(const Vector4 &rhs) const {
            return Vector4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector4 operator/(float rhs) const {
            return Vector4(x / rhs, y / rhs, z / rhs, w / rhs);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector4 operator*(const Vector4 &rhs) const {
            return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector4 operator*(float rhs) const {
            return Vector4(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4 &operator+=(const Vector4 &rhs) {
            return Add(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4 &operator+=(float rhs) {
            return Add(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4 &operator-=(const Vector4 &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4 &operator-=(float rhs) {
            return Sub(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4 &operator/=(const Vector4 &rhs) {
            return Div(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4 &operator/=(float rhs) {
            return Div(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4 &operator*=(const Vector4 &rhs) {
            return Mul(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4 &operator*=(float rhs) {
            return Mul(rhs);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float operator|(const Vector4 &rhs) {
            return Dot(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4 &Add(const Vector4 &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4 &Add(float rhs) {
            x += rhs;
            y += rhs;
            z += rhs;
            w += rhs;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4 &Sub(const Vector4 &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4 &Sub(float rhs) {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            w -= rhs;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4 &Div(const Vector4 &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            w /= rhs.w;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4 &Div(float rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4 &Mul(const Vector4 &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4 &Mul(float rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float Dot(const Vector4 &rhs) {
            return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
        }

        /// Calculate normalized vector
        /// \return
        Vector4 Normalized() {
            return Vector4(x, y, z, w).Normalize();
        }

        /// Normalize this vector
        /// \return
        Vector4 &Normalize() {
            float len = Length();
            x /= len;
            y /= len;
            z /= len;
            w /= len;
            return *this;
        }

        /// Calculate squared length
        /// \return
        float LengthSqr() {
            return x * x + y * y + z * z + w * w;
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

        /// Compare
        /// \param rhs
        /// \return
        bool operator==(const Vector4 &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
        }

        /// Compare
        /// \param rhs
        /// \return
        bool operator!=(const Vector4 &rhs) const {
            return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
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
                case 2:
                    return z;
                case 3:
                    return w;
                default:
                    DJINN_THROW EVector4IndexOOB();
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
                case 2:
                    return z;
                case 3:
                    return w;
                default:
                    DJINN_THROW EVector4IndexOOB();
            }
        }

        /// Helper
        DirectX::XMVECTOR __Load() const {
            return DirectX::XMVectorSet(x, y, z, w);
        }

        // Components
        float x;
        float y;
        float z;
        float w;
    };
}