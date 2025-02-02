//
// Created by nathan on 2/13/21.
//

#include <Base/DefaultLogs.h>

#include "Common.h"
#include "StateStartup.h"

namespace Tetris {
    using namespace Djinn;

    StateStartup::StateStartup() {
    }

    StateStartup::~StateStartup() noexcept {
    }

    void StateStartup::Enter() {
        InfoLog(Application).Write("Startup state entered.");
    }

    void StateStartup::Exit() {
    }

    void StateStartup::Think(double delta) {
    }
}