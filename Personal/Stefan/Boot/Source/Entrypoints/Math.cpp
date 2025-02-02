#include <Base/Application.h>
#include <Base/DefaultLogs.h>

#include <Math/Vector3.h>
#include <Math/Matrix4.h>
#include <Math/Quat.h>

DJINN_NS2(Math);

void EntryPointMath(Base::Application &app) {
    InfoLog(&app).Write("[Math]");

    Vector3 dir(1, 0, 0);

    Quaternion quat_y({0, 1, 0}, -kPI / 2);

    dir = quat_y * dir;

    InfoLog(&app).Write("Dir: [", dir.x, ", ", dir.y, ", ", dir.z, "]");

    Matrix4 mat_translate = Matrix4::Translate({-5, 0, 0});

    Vector4 pos = mat_translate * Vector4(dir, 1);

    InfoLog(&app).Write("Pos: [", pos.x, ", ", pos.y, ", ", pos.z, "]");
}
