//
// Created by nathan on 1/12/21.
//

#include <ctime>

#include <Base/Application.h>
#include <Base/ApplicationInfo.h>
#include <Base/DefaultLogs.h>

#include "Common.h"
#include "GameState.h"
#include "GameStateHandler.h"
#include "StateStartup.h"
#include "StateMenu.h"
#include "StatePlay.h"
#include "StateShutdown.h"

using namespace Djinn;

void __TetrisMain(Base::Application& app) {
    InfoLog(app.GetRegistry()).Write("Launched application", TETRIS_NAME);

    InfoLog(app.GetRegistry()).Write("Preloading and initialization...");
    std::srand(time(0));
    Always(app.GetRegistry()->LoadPlugins());

    Tetris::GameStateHandler* stateHandler = new Tetris::GameStateHandler(app);

    Tetris::StateStartup::Manage(stateHandler, "startup");
    Tetris::StateMenu::Manage(stateHandler, "menu");
    Tetris::StatePlay::Manage(stateHandler, "play");
    Tetris::StateShutdown::Manage(stateHandler, "shutdown");

    stateHandler->Start("startup");

    delete stateHandler;
}

int main(int argc, char** argv) {
    Base::Launch(Base::ApplicationInfo().Name(TETRIS_NAME), Bindg(__TetrisMain));
    return 0;
}