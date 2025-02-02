//
// Created by Dev on 3/16/2018.
//
#pragma once

#include <Core/Array.h>
#include <Core/Buffer.h>
#include "Vector2.h"

namespace Djinn::Math {
    class Spline2 {
    public:
        Vector2 Evaluate(UInt32 offset, float t) {
            if (offset >= p1.Size() || offset >= p2.Size()) {
                return {};
            }

            float omt = 1.0f - t;

            Vector2 _p0 = controlPoints[offset];
            Vector2 _p1 = p1[offset];
            Vector2 _p2 = p2[offset];
            Vector2 _p3 = controlPoints[offset+1];

            float xVal = omt*omt*omt*_p0.x + 3*omt*omt*t*_p1.x +3*omt*t*t*_p2.x+t*t*t*_p3.x;
            float yVal = omt*omt*omt*_p0.y + 3*omt*omt*t*_p1.y +3*omt*t*t*_p2.y+t*t*t*_p3.y;
            return Vector2(xVal,yVal);
        }

        bool Compute() {
            if (controlPoints.Size() == 0) {
                return false;
            }

            UInt32 N = static_cast<UInt32>(controlPoints.Size()) - 1;
            p1.Resize(N);
            p2.Resize(N);
            if (N == 0) {
                return false;
            }

            if (N == 1) {  // Only 2 points...just create a straight line.
                // Constraint:  3*P1 = 2*P0 + P3
                p1[0] = (controlPoints[0] * (2.0f / 3.0f) + controlPoints[1] * (1.0f / 3.0f));
                // Constraint:  P2 = 2*P1 - P0
                p2[0] = p1[0] * 2.0f - controlPoints[0];
                return true;
            }

            a.Resize(N);
            b.Resize(N);
            c.Resize(N);
            r.Resize(N);

            /*left most segment*/
            a[0].x = 0;
            b[0].x = 2;
            c[0].x = 1;
            r[0].x = controlPoints[0].x + 2 * controlPoints[1].x;

            a[0].y = 0;
            b[0].y = 2;
            c[0].y = 1;
            r[0].y = controlPoints[0].y + 2 * controlPoints[1].y;

            /*internal segments*/
            for (Int32 i = 1; i < static_cast<Int32>(N) - 1; i++) {
                a[i].x = 1;
                b[i].x = 4;
                c[i].x = 1;
                r[i].x = 4 * controlPoints[i].x + 2 * controlPoints[i + 1].x;

                a[i].y = 1;
                b[i].y = 4;
                c[i].y = 1;
                r[i].y = 4 * controlPoints[i].y + 2 * controlPoints[i + 1].y;
            }

            /*right segment*/
            a[N - 1].x = 2;
            b[N - 1].x = 7;
            c[N - 1].x = 0;
            r[N - 1].x = 8 * controlPoints[N - 1].x + controlPoints[N].x;

            a[N - 1].y = 2;
            b[N - 1].y = 7;
            c[N - 1].y = 0;
            r[N - 1].y = 8 * controlPoints[N - 1].y + controlPoints[N].y;


            /*solves Ax=b with the Thomas algorithm (from Wikipedia)*/
            for (Int32 i = 1; i < static_cast<Int32>(N); i++) {
                float m;

                m = a[i].x / b[i - 1].x;
                b[i].x = b[i].x - m * c[i - 1].x;
                r[i].x = r[i].x - m * r[i - 1].x;

                m = a[i].y / b[i - 1].y;
                b[i].y = b[i].y - m * c[i - 1].y;
                r[i].y = r[i].y - m * r[i - 1].y;
            }

            p1[N - 1].x = r[N - 1].x / b[N - 1].x;
            p1[N - 1].y = r[N - 1].y / b[N - 1].y;
            for (Int32 i = N - 2; i >= 0; --i) {
                p1[i].x = (r[i].x - c[i].x * p1[i + 1].x) / b[i].x;
                p1[i].y = (r[i].y - c[i].y * p1[i + 1].y) / b[i].y;
            }

            /*we have p1, now compute p2*/
            for (Int32 i = 0; i < static_cast<Int32>(N) - 1; i++) {
                p2[i].x = 2 * controlPoints[i + 1].x - p1[i + 1].x;
                p2[i].y = 2 * controlPoints[i + 1].y - p1[i + 1].y;
            }

            p2[N - 1].x = 0.5f * (controlPoints[N].x + p1[N - 1].x);
            p2[N - 1].y = 0.5f * (controlPoints[N].y + p1[N - 1].y);

            return true;
        }

        Core::Buffer<Vector2> controlPoints;

    private:
        Core::Buffer<Vector2> p1;
        Core::Buffer<Vector2> p2;
        Core::Buffer<Vector2> a;
        Core::Buffer<Vector2> b;
        Core::Buffer<Vector2> c;
        Core::Buffer<Vector2> r;
    };
}