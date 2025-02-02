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
    struct EQuaternionIndexOOB {
    };

    class Quaternion {
    public:
        Quaternion() : x(0), y(0), z(0), w(1) {

        }

        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
        }

        Quaternion(const Vector3 &axis, float angle) {
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(Ptr()), DirectX::XMQuaternionRotationAxis(axis.__Load(), angle));
        }

        static Quaternion RotateRPY(const Vector3 &angles) {
            Quaternion mat;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(mat.Ptr()), DirectX::XMQuaternionRotationRollPitchYaw(angles.x, angles.y, angles.z));
            return mat;
        }

        static Vector3 Transform(const Quaternion& quat, const Vector3& vec) {
            Vector3 transformed;
            DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(transformed.Ptr()), DirectX::XMVector3Rotate(vec.__Load(), quat.__Load()));
            return transformed;
        }

        static Quaternion Lookat(const Vector3& direction) {
            auto norm = direction.Normalized();

            auto axis = Vector3(1, 0, 0) ^ norm;
            float dot = Vector3::Dot(Vector3(1, 0, 0), norm);

            return Quaternion(axis.x, axis.y, axis.z, dot + 1).Normalized();
        }

        Vector3 AsDirection() const {
            Vector3 vec;
            DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(vec.Ptr()), DirectX::XMVector3Rotate(DirectX::XMVectorSet(1, 0, 0, 0), __Load()));
            return vec;
        }

        /// Add to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator+(const Quaternion &rhs) const {
            return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        /// Add to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator+(float rhs) const {
            return Quaternion(x + rhs, y + rhs, z + rhs, w + rhs);
        }

        /// Subtract to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator-(const Quaternion &rhs) const {
            return Quaternion(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        /// Subtract to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator-(float rhs) const {
            return Quaternion(x - rhs, y - rhs, z - rhs, w - rhs);
        }

        /// Divide to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator/(const Quaternion &rhs) const {
            return Quaternion(x / rhs.x, y / rhs.y, z - rhs.z, w - rhs.w);
        }

        /// Divide to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator/(float rhs) const {
            return Quaternion(x / rhs, y / rhs, z / rhs, w / rhs);
        }

        /// Multiply to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator*(const Quaternion &rhs) const {
            Quaternion quat;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(quat.Ptr()), DirectX::XMQuaternionMultiply(__Load(), rhs.__Load()));
            return quat;
        }

        /// Calculate inverse of quaternion
        /// \param quat
        /// \return
        static Quaternion Inversed(const Quaternion& quat) {
            Quaternion inversed;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(inversed.Ptr()), DirectX::XMQuaternionInverse(quat.__Load()));
            return quat;
        }

        /// Calculate inverse to new quaternion
        /// \return
        Quaternion Inversed() const {
            Quaternion quat;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(quat.Ptr()), DirectX::XMQuaternionInverse(__Load()));
            return quat;
        }

        /// Calculate inverse to this quaternion
        /// \return
        Quaternion& Inverse() {
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(Ptr()), DirectX::XMQuaternionInverse(__Load()));
            return *this;
        }

        /// Multiply to new quaternion
        /// \param rhs
        /// \return
        Quaternion operator*(float rhs) const {
            return Quaternion(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        /// Transform a vector
        /// \param rhs
        /// \return
        Vector3 operator*(const Vector3& vec) const {
            return Transform(vec);
        }

        /// Add to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator+=(const Quaternion &rhs) {
            return Add(rhs);
        }

        /// Add to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator+=(float rhs) {
            return Add(rhs);
        }

        /// Subtract to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator-=(const Quaternion &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator-=(float rhs) {
            return Sub(rhs);
        }

        /// Divide to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator/=(const Quaternion &rhs) {
            return Div(rhs);
        }

        /// Divide to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator/=(float rhs) {
            return Div(rhs);
        }

        /// Multiply to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator*=(const Quaternion &rhs) {
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(Ptr()), DirectX::XMQuaternionMultiply(__Load(), rhs.__Load()));
            return *this;
        }

        /// Multiply to this quaternion
        /// \param rhs
        /// \return
        Quaternion &operator*=(float rhs) {
            return Mul(rhs);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float operator|(const Quaternion &rhs) {
            return Dot(rhs);
        }

        /// Add to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Add(const Quaternion &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /// Add to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Add(float rhs) {
            x += rhs;
            y += rhs;
            z += rhs;
            w += rhs;
            return *this;
        }

        /// Subtract to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Sub(const Quaternion &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        /// Subtract to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Sub(float rhs) {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            w -= rhs;
            return *this;
        }

        /// Divide to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Div(const Quaternion &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            w /= rhs.w;
            return *this;
        }

        /// Divide to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Div(float rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
            return *this;
        }

        /// Multiply to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Mul(const Quaternion &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
            return *this;
        }

        /// Multiply to this quaternion
        /// \param rhs
        /// \return
        Quaternion &Mul(float rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }

        /// Transform vector
        /// \param vec
        /// \return
        Vector3 Transform(const Vector3& vec) const {
            return Transform(*this, vec);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float Dot(const Quaternion &rhs) {
            return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
        }

        /// Calculate normalized quaternion
        /// \return
        Quaternion Normalized() {
            return Quaternion(x, y, w, z).Normalize();
        }

        /// Normalize this quaternion
        /// \return
        Quaternion &Normalize() {
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
        bool operator==(const Quaternion &rhs) const {
            return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
        }

        /// Compare
        /// \param rhs
        /// \return
        bool operator!=(const Quaternion &rhs) const {
            return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w;
        }

        static Quaternion Identity() {
            return {0, 0, 0, 1};
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
                    DJINN_THROW EQuaternionIndexOOB();
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
                    DJINN_THROW EQuaternionIndexOOB();
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