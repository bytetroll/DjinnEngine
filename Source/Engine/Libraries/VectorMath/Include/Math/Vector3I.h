//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Core/Math.h>
#include "Vector2I.h"

namespace Djinn::Math {
    struct EVector3IIndexOOB {
    };

    class Vector3I {
    public:
        Vector3I() {

        }

        Vector3I(float x, float y, float z) : x(x), y(y), z(z) {
        }

        Vector3I(const Vector2I &v, float z) : x(v.x), y(v.y), z(z) {
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector3I operator+(const Vector3I &rhs) const {
            return Vector3I(x + rhs.x, y + rhs.y, z + rhs.z);
        }

        /// Add to new vector
        /// \param rhs
        /// \return
        Vector3I operator+(float rhs) const {
            return Vector3I(x + rhs, y + rhs, z + rhs);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector3I operator-(const Vector3I &rhs) const {
            return Vector3I(x - rhs.x, y - rhs.y, z - rhs.z);
        }

        /// Subtract to new vector
        /// \param rhs
        /// \return
        Vector3I operator-(float rhs) const {
            return Vector3I(x - rhs, y - rhs, z - rhs);
        }

        /// Negate this vector
        /// \return
        Vector3I operator-() const {
            return Vector3I(-x, -y, -z);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector3I operator/(const Vector3I &rhs) const {
            return Vector3I(x / rhs.x, y / rhs.y, z / rhs.z);
        }

        /// Divide to new vector
        /// \param rhs
        /// \return
        Vector3I operator/(float rhs) const {
            return Vector3I(x / rhs, y / rhs, z / rhs);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector3I operator*(const Vector3I &rhs) const {
            return Vector3I(x * rhs.x, y * rhs.y, z * rhs.z);
        }

        /// Multiply to new vector
        /// \param rhs
        /// \return
        Vector3I operator*(float rhs) const {
            return Vector3I(x * rhs, y * rhs, z * rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3I &operator+=(const Vector3I &rhs) {
            return Add(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3I &operator+=(float rhs) {
            return Add(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3I &operator-=(const Vector3I &rhs) {
            return Sub(rhs);
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3I &operator-=(float rhs) {
            return Sub(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3I &operator/=(const Vector3I &rhs) {
            return Div(rhs);
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3I &operator/=(float rhs) {
            return Div(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3I &operator*=(const Vector3I &rhs) {
            return Mul(rhs);
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3I &operator*=(float rhs) {
            return Mul(rhs);
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float operator|(const Vector3I &rhs) {
            return Dot(rhs);
        }

        /// Calculate cross product to new vector
        /// \param rhs
        /// \return
        Vector3I operator^(const Vector3I &rhs) {
            return Vector3I(x, y, z).Cross(rhs);
        }

        /// Calculate cross product to this vector
        /// \param rhs
        /// \return
        Vector3I &operator^=(const Vector3I &rhs) {
            return Cross(rhs);
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3I &Add(const Vector3I &rhs) {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            return *this;
        }

        /// Add to this vector
        /// \param rhs
        /// \return
        Vector3I &Add(float rhs) {
            x += rhs;
            y += rhs;
            z += rhs;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3I &Sub(const Vector3I &rhs) {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            return *this;
        }

        /// Subtract to this vector
        /// \param rhs
        /// \return
        Vector3I &Sub(float rhs) {
            x -= rhs;
            y -= rhs;
            z -= rhs;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3I &Div(const Vector3I &rhs) {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            return *this;
        }

        /// Divide to this vector
        /// \param rhs
        /// \return
        Vector3I &Div(float rhs) {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3I &Mul(const Vector3I &rhs) {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            return *this;
        }

        /// Multiply to this vector
        /// \param rhs
        /// \return
        Vector3I &Mul(float rhs) {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            return *this;
        }

        /// Cross to this vector
        /// \param rhs
        /// \return
        Vector3I &Cross(const Vector3I &rhs) {
            x = y * rhs.z - z * rhs.y;
            y = z * rhs.x - x * rhs.z;
            z = x * rhs.y - y * rhs.x;
            return *this;
        }

        /// Calculate dot product
        /// \param rhs
        /// \return
        float Dot(const Vector3I &rhs) {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        /// Calculate normalized vector
        /// \return
        Vector3I Normalized() {
            return Vector3I(x, y, z).Normalize();
        }

        /// Normalize this vector
        /// \return
        Vector3I &Normalize() {
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
                    DJINN_THROW EVector3IIndexOOB();
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
                    DJINN_THROW EVector3IIndexOOB();
            }
        }

        // Components
        float x;
        float y;
        float z;
    };
}