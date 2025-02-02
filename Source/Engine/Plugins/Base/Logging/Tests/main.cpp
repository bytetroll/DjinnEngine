//
// Created by Dev on 5/12/2018.
//

#include <Base/Application.h>

DJINN_NS();

void _main(Base::Application &app) {
    malloc(500);
}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("Logging test"), Bindg(_main));
    return 0;
}