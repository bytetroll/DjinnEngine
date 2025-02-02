#include <Base/Application.h>

#include "EntryPoints.h"

DJINN_NS();

void StefanoPatrono(Base::Application &app) {
    EntryPointMath(app);
    EntryPointReflection(app);
    EntryPointFilesystem(app);
    EntryPointAsync(app);

    // Launch the UI test
    EntryPointUI(app);
}

int main() {
    // So, an application loads and initializes a set of plugins that it considers requires
    // It greatly simplifies development times, plus there's a convenient Base::Launch that just kicks it off.
    //  Bindg "binds" a global function as a delegate (a function which is invocable). This could also just be a lambda!
    if (COM::CheckedResult result = Base::Launch(Base::ApplicationInfo().Name("Boot App"), Bindg(StefanoPatrono))) {
        return 1;
    }
    return 0;
}
