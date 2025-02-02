#pragma once

#include <Core/Common.h>
#include <Core/Size.h>
#include <cmath>
#include <cstdlib>
#include <Core/Template/Common.h>
#include <Core/Template/Bits.h>

// Note: For now this is a proxy namespace, will be expanded later

namespace Djinn::Math {
    /// Get the sign of a value
    /// \param v
    /// \return
    template<typename T, typename U = typename Core::SelectLargest<T, char>::Type>
    DJINN_INLINE static U Sign(const T &v) {
        return static_cast<U>(v < 0 ? -1 : 1);
    }

    /// Ceil float value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Ceil(const T &v) {
        return static_cast<T>(ceil(v));
    }

    /// Floor float value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Floor(const T &v) {
        return static_cast<T>(floor(v));
    }

    /// Get log2 of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Log2(const T &v) {
        return static_cast<T>(log2(v));
    }

    /// Get log of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Log(const T &v) {
        return static_cast<T>(log(v));
    }

    /// Get abs of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Abs(const T &v) {
        return static_cast<T>(fabs(v));
    }

    /// Get power of value
    /// \param v
    /// \param p
    /// \return
    template<typename T, typename U, typename H = typename Core::SelectLargest<T, U>::Type>
    DJINN_INLINE static H Pow(const T &v, const U &p) {
        if constexpr(Core::IsSame<H, double>::kValue) {
            return static_cast<H>(pow(v, p));
        } else {
            return static_cast<H>(powf(v, p));
        }
    }

    /// Round float value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Round(const T &v) {
        return static_cast<T>(round(v));
    }

    /// Get sqrt of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Sqrt(const T &v) {
        return static_cast<T>(sqrt(v));
    }

    /// Square value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static auto Square(const T &v) -> decltype(v * v) {
        return v * v;
    }

    /// Cosine of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Cos(const T &v) {
        return static_cast<T>(cos(v));
    }

    /// Sine of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Sin(const T &v) {
        return static_cast<T>(sin(v));
    }

    /// ACosine of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Acos(const T &v) {
        return static_cast<T>(acos(v));
    }

    /// ASine of value
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Asin(const T &v) {
        return static_cast<T>(asin(v));
    }

    /// Random float
    /// \param min
    /// \param max
    /// \return
    template<typename T, typename U, typename R = typename Core::SelectLargest<T, U>::Type>
    DJINN_INLINE static R RandFFast(const T &min, const U &max) {
        return static_cast<R>(min + static_cast <double> (rand()) / (RAND_MAX / static_cast<double>(max - min)));
    }

    /// Get minimum of two values
    /// \param a
    /// \param b
    /// \return
    template<typename X, typename Y>
    DJINN_INLINE static typename Core::SelectLargest<X, Y>::Type Min(const X &a, const Y &b) {
        return (a < b) ? a : b;
    }

    /// Get minimum of n values
    /// \param a
    /// \param b
    /// \param c
    /// \param rest
    /// \return
    template<typename X, typename Y, typename Z, typename... N>
    DJINN_INLINE static typename Core::SelectLargest<X, Y>::Type Min(const X &a, const Y &b, const Z &c, const N &... rest) {
        return Min(Min(a, b), c, rest...);
    }

    /// Get maximum of two values
    /// \param a
    /// \param b
    /// \return
    template<typename X, typename Y>
    DJINN_INLINE static typename Core::SelectLargest<X, Y>::Type Max(const X &a, const Y &b) {
        return (a > b) ? a : b;
    }

    /// Get maximum of n values
    /// \param a
    /// \param b
    /// \param c
    /// \param rest
    /// \return
    template<typename X, typename Y, typename Z, typename... N>
    DJINN_INLINE static typename Core::SelectLargest<X, Y>::Type Max(const X &a, const Y &b, const Z &c, const N &... rest) {
        return Max(Max(a, b), c, rest...);
    }

    /// Lerp between two values
    /// \param l
    /// \param r
    /// \param a
    /// \return
    template<typename X, typename Y, typename Z>
    DJINN_INLINE static auto Lerp(const X &l, const Y &r, Z a) -> decltype(l + (r - l) * a) {
        return l + (r - l) * a;
    }

    /// Clamp value
    /// \param v
    /// \param MIN
    /// \param MAX
    /// \return
    template<typename X, typename Y, typename Z>
    DJINN_INLINE static X Clamp(const X &v, const Y &MIN, const Z &MAX) {
        if (v < MIN) return MIN;
        if (v > MAX) return MAX;
        return v;
    }

    /// Snap value to step
    /// \param v
    /// \param grid
    /// \return
    template<typename T, typename U>
    DJINN_INLINE static U Snap(const T &v, const U &grid) {
        return Round(static_cast<double>(v) / grid) * grid;
    }

    /// Snap value to step
    /// \param v
    /// \param grid
    /// \return
    template<typename T, typename U>
    DJINN_INLINE static U SnapLower(const T &v, const U &grid) {
        return Floor(static_cast<double>(v) / grid) * grid;
    }

    /// Snap value to step
    /// \param v
    /// \param grid
    /// \return
    template<typename T, typename U>
    DJINN_INLINE static U SnapUpper(const T &v, const U &grid) {
        return Ceil(static_cast<double>(v) / grid) * grid;
    }

    /// Saturate a value between 0 and 1
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T Saturate(const T &v) {
        return Clamp(v, 0, 1);
    }

    /// Saturate a value between 0 and inf
    /// \param v
    /// \return
    template<typename T>
    DJINN_INLINE static T SaturateNegative(const T &v) {
        return Max(v, 0);
    }

    /// Convert a 3D coordinate to a 1d coordinate
    /// \param width
    /// \param depth
    /// \param x
    /// \param y
    /// \param z
    /// \return
    DJINN_INLINE static USize To1D(USize width, USize depth, USize x, USize y, USize z) {
        return x + width * (y + depth * z);
    }

    /// Convert a 2D coordinate to a 1d coordinate
    /// \param width
    /// \param x
    /// \param y
    /// \return
    DJINN_INLINE static USize To1D(USize width, USize x, USize y) {
        return y * width + x;
    }

    /// Constants
    static constexpr float kPI = 3.14159265358979f;
    static constexpr double kPId = 3.14159265358979323846;
    static constexpr long double kPIl = 3.141592653589793238462643383279502884L;
}