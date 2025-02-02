//
// Created by nathan on 2/13/21.
//

#include <Core/String.h>

#include "Common.h"
#include "GameState.h"
#include "GameStateHandler.h"

namespace Tetris {
    using namespace Djinn;

    class StatePlay : public GameState {
    public:
        TETRIS_STATE_REGISTER(StatePlay);

    public:
        StatePlay();
        ~StatePlay() noexcept;

        void Enter();
        void Exit();
        void Think(double delta);
    };
}