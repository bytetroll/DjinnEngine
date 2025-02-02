// Copyright (C) 2018 Djinn Engine Team.  All rights reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.

#include <Base/Application.h>

DJINN_NS2(Core);

void _main(Base::Application &app) {

}

int main() {
    Base::Launch(Base::ApplicationInfo().Name("CodeGeneration"), Bindg(_main));
    return 0;
}

