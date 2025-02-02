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

#include <Editor/Elements/Templates.h>
#include <Base/DefaultLogs.h>
#include <UI/ElementComponents/IUITransformComponent.h>
#include <UI/Elements/UIOutputDisplay.h>
#include <Game/ActorSystem.h>
#include <Game/ICamera.h>
#include <Game/IWorld.h>
#include <Game/IScene.h>
#include <Editor/ILevel.h>
#include <Editor/ITemplatesHost.h>
#include <Editor/ILevelLayoutSceneState.h>

DJINN_NS2(Editor);

Templates::Templates(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ITemplates(registry, outer, host) {

}

void Templates::StaticInitialize() {
    Construct("Runtime/Editor/UI/Templates.uiml"_path);

    auto layout = GetLayout();

    layout->SetScaling(UI::UIScaling::eFill, UI::UIScaling::eFill);
}

void Templates::Created() {
    // Get state
    auto state = COM::Cast<ILevelLayoutSceneState>(GetScene()->GetState());
    if (!state) {
        ErrorLog(this).Write("Templates created within a non level layout scene");
        return;
    }

    // Get the worldHost
    CheckMsgVoid(world->GetPipeline()->GetInterface(&host), Error, "Expected templates worldHost to be installed");

    // Register this
    CheckVoid(host->Register(this));
}
