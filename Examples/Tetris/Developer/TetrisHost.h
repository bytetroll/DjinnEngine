//
// Created by nathan on 1/12/21.
//

#pragma once

#include <Base/Application.h>

#include "Code/Common.h"
#include "TetrisHostContext.h"

class TetrisHost {
public:
    static TetrisHostContext* Create(Application* app);
    static void Free(TetrisHostContext *context);
};
