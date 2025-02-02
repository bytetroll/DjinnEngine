//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Math/Detail/DirectXMath/Inc/DirectXMath.h>
#include <Core/Math.h>
#include "Vector4.h"
#include "Vector3.h"
#include "Quat.h"

namespace Djinn::Math {
    struct EMatrix4IndexOOB {
    };

    struct Matrix4Decomposition {
        Math::Quaternion rotation;
        Math::Vector3 translation;
        Math::Vector3 scale;
    };

    class Matrix4 {
    public:
        Matrix4() {

        }

        Matrix4(const Vector4 &x, const Vector4 &y, const Vector4 &z, const Vector4 &w) : x(x), y(y), z(z), w(w) {
        }

        static Matrix4 Translate(const Vector3 &position) {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixTranslationFromVector(position.__Load()));
            return mat;
        }

        /// Apply a translation to an existing matrix
        /// \param transform
        /// \param position
        /// \return
        static Matrix4 Translate(const Math::Matrix4& transform, const Vector3 &position) {
            auto copy = transform;
            copy.w += {position, 0};
            return copy;
        }

        static Matrix4 Rotate(const Quaternion &quat) {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixRotationQuaternion(quat.__Load()));
            return mat;
        }

        static Matrix4 RotateRPY(const Vector3 &angles) {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixRotationRollPitchYaw(angles.x, angles.y, angles.z));
            return mat;
        }

        static Matrix4 Scale(const Vector3 &scale) {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixScalingFromVector(scale.__Load()));
            return mat;
        }

        static Matrix4 Lookat(const Vector3& eye, const Vector3& focus) {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixLookAtLH(eye.__Load(), focus.__Load(), DirectX::XMVectorSet(0, 1, 0, 0)));
            return mat;
        }

        static Matrix4 Perspective(float fov, float aspectRation, float nearZ, float farZ) {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixPerspectiveFovLH(fov, aspectRation, nearZ, farZ));
            return mat;
        }

        /// Decompose a matrix
        /// \return
        static Matrix4Decomposition Decompose(const Matrix4& mat) {
            Matrix4Decomposition decomp;
            DirectX::XMVECTOR scale, translation, rotation;
            DirectX::XMMatrixDecompose(&scale, &rotation, &translation, mat.__Load());
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(decomp.scale.Ptr()), scale);
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(decomp.rotation.Ptr()), rotation);
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(decomp.translation.Ptr()), translation);
            return decomp;
        }

        /// Decompose this matrix
        /// \return
        Matrix4Decomposition Decompose() const {
            return Decompose(*this);
        }

        /// Add to new matrix
        /// \param rhs
        /// \return
        Matrix4 operator+(const Matrix4 &rhs) const {
            return Matrix4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        /// Add to new matrix
        /// \param rhs
        /// \return
        Matrix4 operator+(float rhs) const {
            return Matrix4(x + rhs, y + rhs, z + rhs, w + rhs);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Matrix4 operator-(const Matrix4 &rhs) const {
            return Matrix4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Matrix4 operator-(float rhs) const {
            return Matrix4(x - rhs, y - rhs, z - rhs, w - rhs);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Matrix4 operator/(const Matrix4 &rhs) const {
            return Matrix4(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Matrix4 operator/(float rhs) const {
            return Matrix4(x / rhs, y / rhs, z / rhs, w / rhs);
        }

        /// Multiply to new (positioned) vector
        /// \param rhs
        /// \return
        Vector4 operator*(Vector3 rhs) const {
            Vector4 vec;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(vec.Ptr()), DirectX::XMVector3TransformCoord(rhs.__Load(), __Load()));
            return vec;
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector4 operator*(Vector4 rhs) const {
            Vector4 vec;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(vec.Ptr()), DirectX::XMVector4Transform(rhs.__Load(), __Load()));
            return vec;
        }

        /// Multiply to this matrix
        /// \param rhs
        /// \return
        Matrix4 operator*(const Matrix4 &rhs) const {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixMultiply(__Load(), rhs.__Load()));
            return mat;
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Matrix4 operator*(float rhs) const {
            return Matrix4(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator+=(const Matrix4 &rhs) {
            return Add(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator+=(float rhs) {
            return Add(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator-=(const Matrix4 &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator-=(float rhs) {
            return Sub(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator/=(const Matrix4 &rhs) {
            return Div(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator/=(float rhs) {
            return Div(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator*=(const Matrix4 &rhs) {
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(Ptr()), DirectX::XMMatrixMultiply(__Load(), rhs.__Load()));
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Matrix4 &operator*=(float rhs) {
            return Mul(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Matrix4 &Add(const Matrix4 &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Matrix4 &Add(float rhs) {
            x += rhs;
            y += rhs;
            z += rhs;
            w += rhs;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Matrix4 &Sub(const Matrix4 &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Matrix4 &Sub(float rhs) {
            x -= rhs;
            y -= rhs;
            z += rhs;
            w += rhs;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Matrix4 &Div(const Matrix4 &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Matrix4 &Div(float rhs) {
            x /= rhs;
            y /= rhs;
            z += rhs;
            w += rhs;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Matrix4 &Mul(const Matrix4 &rhs) {
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(Ptr()), DirectX::XMMatrixMultiply(__Load(), rhs.__Load()));
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Matrix4 &Mul(float rhs) {
            x *= rhs;
            y *= rhs;
            z += rhs;
            w += rhs;
            return *this;
        }

        /// Calculate inverse to this
        /// \return
        Matrix4 &Inverse() {
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(Ptr()), DirectX::XMMatrixInverse(nullptr, __Load()));
            return *this;
        }

        /// Calculate inversed as new matrix
        /// \return
        Matrix4 Inversed() const {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixInverse(nullptr, __Load()));
            return mat;
        }

        /// Calculate tranpose to this
        /// \return
        Matrix4 &Transpose() {
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(Ptr()), DirectX::XMMatrixTranspose(__Load()));
            return *this;
        }

        /// Calculate tranposed as new matrix
        /// \return
        Matrix4 Transposed() const {
            Matrix4 mat;
            DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4 *>(mat.Ptr()), DirectX::XMMatrixTranspose(__Load()));
            return mat;
        }

        /// Transform a position
        /// \param v
        /// \return
        static Vector3 TransformPosition(const Matrix4& mat, const Vector3 &v) {
            Vector3 vec;
            DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(vec.Ptr()), DirectX::XMVector3TransformCoord(v.__Load(), mat.__Load()));
            return vec;
        }

        /// Transform a position
        /// \param v
        /// \return
        Vector3 TransformPosition(const Vector3 &v) const {
            return TransformPosition(*this, v);
        }

        /// Inverse transform a position
        /// \param v
        /// \return
        static Vector3 InverseTransformPosition(const Matrix4& mat, const Vector3 &v) {
            Vector3 vec;
            DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(vec.Ptr()), DirectX::XMVector3TransformCoord(v.__Load(), DirectX::XMMatrixInverse(nullptr, mat.__Load())));
            return vec;
        }

        /// Inverse transform a position
        /// \param v
        /// \return
        Vector3 InverseTransformPosition(const Vector3 &v) const {
            return InverseTransformPosition(*this, v);
        }

        /// Transform a vector, does not take translation into account
        /// \param v
        /// \return
        static Vector3 TransformVector(const Matrix4& mat, const Vector3 &v) {
            Vector3 vec;
            DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(vec.Ptr()), DirectX::XMVector3Transform(v.__Load(), mat.__Load()));
            return vec;
        }

        /// Transform a vector
        /// \param v
        /// \return
        Vector3 TransformVector(const Vector3 &v) const {
            return TransformVector(*this, v);
        }

        /// Inverse transform a vector, does not take translation into account
        /// \param v
        /// \return
        static Vector3 InverseTransformVector(const Matrix4& mat, const Vector3 &v) {
            Vector3 vec;
            DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3 *>(vec.Ptr()), DirectX::XMVector3Transform(v.__Load(), DirectX::XMMatrixInverse(nullptr, mat.__Load())));
            return vec;
        }

        /// Inverse transform a vector
        /// \param v
        /// \return
        Vector3 InverseTransformVector(const Vector3 &v) const {
            return InverseTransformVector(*this, v);
        }

        /// Transform a position
        /// \param v
        /// \return
        static Vector4 Transform(const Matrix4& mat, const Vector4 &v) {
            Vector4 vec;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(vec.Ptr()), DirectX::XMVector4Transform(v.__Load(), mat.__Load()));
            return vec;
        }

        /// Transform a position
        /// \param v
        /// \return
        Vector4 Transform(const Vector4 &v) const {
            return Transform(*this, v);
        }

        /// Inverse transform a position
        /// \param v
        /// \return
        static Vector4 InverseTransform(const Matrix4& mat, const Vector4 &v) {
            Vector4 vec;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(vec.Ptr()), DirectX::XMVector4Transform(v.__Load(), DirectX::XMMatrixInverse(nullptr, mat.__Load())));
            return vec;
        }

        /// Inverse transform a position
        /// \param v
        /// \return
        Vector4 InverseTransform(const Vector4 &v) const {
            return InverseTransform(*this, v);
        }

        /// Transform a vector, does not take translation into account
        /// \param v
        /// \return
        static Vector4 TransformVector(const Matrix4& mat, const Vector4 &v) {
            Vector4 vec;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(vec.Ptr()), DirectX::XMVector4Transform(v.__Load(), mat.__Load()));
            return vec;
        }

        /// Transform a vector
        /// \param v
        /// \return
        Vector4 TransformVector(const Vector4 &v) const {
            return TransformVector(*this, v);
        }

        /// Inverse transform a vector, does not take translation into account
        /// \param v
        /// \return
        static Vector4 InverseTransformVector(const Matrix4& mat, const Vector4 &v) {
            Vector4 vec;
            DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4 *>(vec.Ptr()), DirectX::XMVector4Transform(v.__Load(), DirectX::XMMatrixInverse(nullptr, mat.__Load())));
            return vec;
        }

        /// Inverse transform a vector
        /// \param v
        /// \return
        Vector4 InverseTransformVector(const Vector4 &v) const {
            return InverseTransformVector(*this, v);
        }

        /// Get element pointer
        /// \return
        float *Ptr() {
            return x.Ptr();
        }

        /// Helper
        DirectX::XMMATRIX __Load() const {
            return DirectX::XMMatrixSet(
                    x.x, x.y, x.z, x.w,
                    y.x, y.y, y.z, y.w,
                    z.x, z.y, z.z, z.w,
                    w.x, w.y, w.z, w.w
            );
        }

        static Matrix4 Identity() {
            return {{1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}};
        }

        // Components
        Vector4 x;
        Vector4 y;
        Vector4 z;
        Vector4 w;
    };
}