//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 1/21/2019.
//

#include <Editor/LevelLayoutSceneState.h>
#include <Editor/IEditorWorld.h>
#include <Host/IRegistry.h>
#include <Host/IPipeline.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorld.h>
#include <Editor/ILevel.h>
#include <Host/Pipeline.h>

DJINN_NS2(Editor);

LevelLayoutSceneState::LevelLayoutSceneState(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : ILevelLayoutSceneState(registry, outer, host) {

}

COM::Result LevelLayoutSceneState::Initialize(ILevel *level) {
    this->level = level;

    // Create pipeline
    pipeline = new Host::Pipeline();

    // OK
    return COM::kOK;
}

ILevel *LevelLayoutSceneState::GetLevel() {
    return level;
}

Host::IPipeline *LevelLayoutSceneState::GetPipeline() {
    return pipeline;
}
