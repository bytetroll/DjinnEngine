//
// Created by Dev on 10/23/2017.
//

#include <Core/Common.h>
#include <Math/Vector2.h>
#include <Core/Timer.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Matrix4.h>
#include <Math/Quat.h>
#include <iostream>

DJINN_NS2(Math);

void Test2() {
    Vector3 a = {0, 1, 2};
    Vector3 b = {2, 1, 0};

    Vector3 c = a + b;

    Matrix4 t0 = Matrix4::Translate({3, 5, 7});
    Matrix4 t1 = Matrix4::Translate({3, 5, 7});

    auto t2 = t0 * t1;
}

int main() {
    Test2();
}
