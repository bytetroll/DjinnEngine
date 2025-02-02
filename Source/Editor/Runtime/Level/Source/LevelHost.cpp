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
 on 7/11/2018.
//

#include <Editor/LevelHost.h>
#include <Editor/Level.h>
#include <Base/DefaultLogs.h>
#include <Game/IWorldHost.h>
#include <Game/IWorldFeature.h>
#include "../../Layout/Include/Editor/WorldFeatures.h"

DJINN_NS2(Editor);

LevelHost::LevelHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ILevelHost(registry, outer, host) {

}

COM::Result LevelHost::Initialize() {
    // Get world worldHost
    Game::IWorldHost *worldHost;
    if (!registry->GetPipeline()->GetInterface(&worldHost)) {
        ErrorLog(this).Write("Expected world worldHost to be installed");
        return COM::kOK;
    }

    // Create editor feature
    Game::IWorldFeature* feature;
    if (!worldHost->CreateFeature(kEditorFeature, &feature)) {
        return COM::kError;
    }

    // OK
    return COM::kOK;
}

COM::Result LevelHost::CreateLevel(ILevel **out) {
    Level* level;
    if (!registry->CreateClass(this, &level) || !level->Initialize()) {
        ErrorLog(this).Write("Failed to create level");
        return COM::kError;
    }

    // OK
    *out = level;
    return COM::kOK;
}

COM::Result LevelHost::SaveLevel(ILevelAsset **out) {
    return COM::kNotImplemented;
}

COM::Result LevelHost::LoadLevel(ILevelAsset *asset) {
    return COM::kNotImplemented;
}
