//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Core/Math.h>
#include "Vector3I.h"

namespace Djinn::Math {
    struct EVector4IIndexOOB {
    };

    class Vector4I {
    public:
        Vector4I() {

        }

        Vector4I(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
        }

        Vector4I(const Vector3I& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {
        }

        Vector4I(const Vector2I& x, const Vector2I& y) : x(x.x), y(x.y), z(y.x), w(y.y) {
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector4I operator+(const Vector4I &rhs) const {
            return Vector4I(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector4I operator+(float rhs) const {
            return Vector4I(x + rhs, y + rhs, z + rhs, w + rhs);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector4I operator-(const Vector4I &rhs) const {
            return Vector4I(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector4I operator-(float rhs) const {
            return Vector4I(x - rhs, y - rhs, z - rhs, w - rhs);
        }

        /// Negate this vector
        /// \return
        Vector4I operator-() const {
            return Vector4I(-x, -y, -z, -w);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector4I operator/(const Vector4I &rhs) const {
            return Vector4I(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector4I operator/(float rhs) const {
            return Vector4I(x / rhs, y / rhs, z / rhs, w / rhs);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector4I operator*(const Vector4I &rhs) const {
            return Vector4I(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector4I operator*(float rhs) const {
            return Vector4I(x * rhs, y * rhs, z * rhs, w * rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4I &operator+=(const Vector4I &rhs) {
            return Add(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4I &operator+=(float rhs) {
            return Add(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4I &operator-=(const Vector4I &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4I &operator-=(float rhs) {
            return Sub(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4I &operator/=(const Vector4I &rhs) {
            return Div(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4I &operator/=(float rhs) {
            return Div(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4I &operator*=(const Vector4I &rhs) {
            return Mul(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4I &operator*=(float rhs) {
            return Mul(rhs);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float operator|(const Vector4I &rhs) {
            return Dot(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4I &Add(const Vector4I &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector4I &Add(float rhs) {
            x += rhs;
            y += rhs;
            z += rhs;
            w += rhs;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4I &Sub(const Vector4I &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector4I &Sub(float rhs) {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            w -= rhs;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4I &Div(const Vector4I &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            w /= rhs.w;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector4I &Div(float rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4I &Mul(const Vector4I &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector4I &Mul(float rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float Dot(const Vector4I &rhs) {
            return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
        }

        /// Calculate normalized vector
        /// \return
        Vector4I Normalized() {
            return Vector4I(x, y, z, w).Normalize();
        }

        /// Normalize this vector
        /// \return
        Vector4I &Normalize() {
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
                    DJINN_THROW EVector4IIndexOOB();
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
                    DJINN_THROW EVector4IIndexOOB();
            }
        }

        // Components
        float x;
        float y;
        float z;
        float w;
    };
}