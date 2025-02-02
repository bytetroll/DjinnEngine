//
// Created by nathan on 1/11/21.
//

#include <ctime>

#include <Base/Application.h>
#include <Base/ApplicationInfo.h>

#include "Code/Common.h"
#include "Tetris.h"
#include "TetrisHost.h"
#include "TetrisHostContext.h"



//namespace Tetris {
//    bool Tetris::Load(Application const& app) {
//    }
//
//    void Think();
//
//    void Tetris::Think() {
//    }
//
//    void Tetris::Unload() {
//        delete timer;
//    }
//}



void __TetrisMain(Djinn::Base::Application const& app) {
    InfoLog(app.GetRegistry()).Write("Launched application", TETRIS_NAME);

    InfoLog(app.GetRegistry()).Write("Preloading and initialization..."));
    srand(time(0));
    Always(app.GetRegistry()->LoadPlugins());

    TetrisHostContext* context = TetrisHost::Create(&app);

    // Something here.

    TetrisHost::Free(context);
    delete context;
}

int main(int argc, char** argv) {
    Launch(ApplicationInfo().Name(TETRIS_NAME), Bindg(__TetrisMain));
    return 0;
}