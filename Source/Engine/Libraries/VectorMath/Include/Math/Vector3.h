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
#include "Vector2.h"

namespace Djinn::Math {
    struct EVector3IndexOOB {
    };

    class Vector3 {
    public:
        Vector3() : x(0), y(0), z(0) {

        }

        Vector3(float x, float y, float z) : x(x), y(y), z(z) {
        }

        Vector3(const Vector2 &v, float z) : x(v.x), y(v.y), z(z) {
        }

        /// Get the dot product of two vectors
        /// \param a
        /// \param b
        /// \return
        static float Dot(const Vector3 &a, const Vector3 &b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        /// Get the per component minimum of two vectors
        /// \param a
        /// \param b
        /// \return
        static Vector3 Min(const Vector3 &a, const Vector3 &b) {
            return {
                    a.x < b.x ? a.x : b.x,
                    a.y < b.y ? a.y : b.y,
                    a.y < b.y ? a.y : b.y
            };
        }

        /// Get the per component maximum of two vectors
        /// \param a
        /// \param b
        /// \return
        static Vector3 Max(const Vector3 &a, const Vector3 &b) {
            return {
                    a.x >= b.x ? a.x : b.x,
                    a.y >= b.y ? a.y : b.y,
                    a.z >= b.z ? a.z : b.z
            };
        }

        /// Get the per component minimum of N vectors
        /// \param ...
        /// \return
        template< class... N>
        static Vector3 Min(const Vector3 &a, const Vector3 &b, const Vector3 &c, const N &... rest) {
            return Min(Min(a, b), c, rest...);
        }

        /// Get the per component maximum of N vectors
        /// \param ...
        /// \return
        template< class... N>
        static Vector3 Max(const Vector3 &a, const Vector3 &b, const Vector3 &c, const N &... rest) {
            return Max(Max(a, b), c, rest...);
        }

        /// Saturate a vector per component between 0 - 1
        /// \param v
        /// \return
        static Vector3 Saturate(const Vector3& v) {
            return {
                    Math::Saturate(v.x),
                    Math::Saturate(v.y),
                    Math::Saturate(v.z)
            };
        }

        /// Saturate a vector per component between 0 - inf
        /// \param v
        /// \return
        static Vector3 SaturateNegative(const Vector3& v) {
            return {
                    Math::SaturateNegative(v.x),
                    Math::SaturateNegative(v.y),
                    Math::SaturateNegative(v.z)
            };
        }

        /// Take the per component absolute value
        /// \param v
        /// \return
        static Vector3 Abs(const Vector3& v) {
            return {
                    Math::Abs(v.x),
                    Math::Abs(v.y),
                    Math::Abs(v.z)
            };
        }

        static Vector3 Cross(const Vector3 &a, const Vector3 &b) {
            return Vector3(
                    a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x
            );
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector3 operator+(const Vector3 &rhs) const {
            return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector3 operator+(float rhs) const {
            return Vector3(x + rhs, y + rhs, z + rhs);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector3 operator-(const Vector3 &rhs) const {
            return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector3 operator-(float rhs) const {
            return Vector3(x - rhs, y - rhs, z - rhs);
        }

        /// Negate this vector
        /// \return
        Vector3 operator-() const {
            return Vector3(-x, -y, -z);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector3 operator/(const Vector3 &rhs) const {
            return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector3 operator/(float rhs) const {
            return Vector3(x / rhs, y / rhs, z / rhs);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector3 operator*(const Vector3 &rhs) const {
            return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector3 operator*(float rhs) const {
            return Vector3(x * rhs, y * rhs, z * rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3 &operator+=(const Vector3 &rhs) {
            return Add(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3 &operator+=(float rhs) {
            return Add(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3 &operator-=(const Vector3 &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3 &operator-=(float rhs) {
            return Sub(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3 &operator/=(const Vector3 &rhs) {
            return Div(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3 &operator/=(float rhs) {
            return Div(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3 &operator*=(const Vector3 &rhs) {
            return Mul(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3 &operator*=(float rhs) {
            return Mul(rhs);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float operator|(const Vector3 &rhs) {
            return Dot(rhs);
        }

        /// Calculate cross product to new vector
        /// \param rhs
        /// \return
        Vector3 operator^(const Vector3 &rhs) {
            return Cross(*this, rhs);
        }

        /// Calculate cross product to this vector
        /// \param rhs
        /// \return
        Vector3 &operator^=(const Vector3 &rhs) {
            Vector3 tmp = *this;
            return *this = Cross(tmp, rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3 &Add(const Vector3 &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3 &Add(float rhs) {
            x += rhs;
            y += rhs;
            z += rhs;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3 &Sub(const Vector3 &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3 &Sub(float rhs) {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3 &Div(const Vector3 &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3 &Div(float rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3 &Mul(const Vector3 &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3 &Mul(float rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float Dot(const Vector3 &rhs) {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        /// Calculate normalized vector
        /// \return
        Vector3 Normalized() const {
            return Vector3(x, y, z).Normalize();
        }

        /// Normalize this vector
        /// \return
        Vector3 &Normalize() {
            float len = Length();
            x /= len;
            y /= len;
            z /= len;
            return *this;
        }

        /// Calculate squared length
        /// \return
        float LengthSqr() {
            return x * x + y * y + z * z;
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
        bool operator==(const Vector3 &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z;
        }

        /// Compare
        /// \param rhs
        /// \return
        bool operator!=(const Vector3 &rhs) const {
            return x != rhs.x || y != rhs.y || z != rhs.z;
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
                default:
                    DJINN_THROW EVector3IndexOOB();
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
                default:
                    DJINN_THROW EVector3IndexOOB();
            }
        }

        /// Helper
        DirectX::XMVECTOR __Load() const {
            return DirectX::XMVectorSet(x, y, z, 0);
        }

        /// Helper
        Math::Vector2 XY() const {
            return {x, y};
        }

        /// Helper
        Math::Vector2 XZ() const {
            return {x, z};
        }

        /// Helper
        Math::Vector2 YZ() const {
            return {y, z};
        }

        // Components
        float x;
        float y;
        float z;
    };
}