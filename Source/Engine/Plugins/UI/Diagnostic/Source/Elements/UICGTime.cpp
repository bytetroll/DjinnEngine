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
 on 8/31/2018.
//

#include <UI/Elements/UICGTime.h>
#include <UI/Elements/UILabel.h>
#include <Game/IActorSystem.h>
#include <Base/DefaultLogs.h>
#include <GHA/IDevice.h>
#include <GHA/IDeviceMemory.h>
#include <GHA/IInterpreter.h>
#include <Game/IGameSystem.h>
#include <UI/ElementComponents/UILayoutTransformComponent.h>
#include <Graphics/IGraphicsSystem.h>

DJINN_NS2(UI);

UICGTime::UICGTime(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIElement(registry, outer, host) {

}

void UICGTime::StaticInitialize() {
    callback.SetCallback(Bindff(Update));
    callback.Register(this);

    // Get game system
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&system), Error, "Expected game system to be installed");

    // Get graphics system
    Graphics::IGraphicsSystem *graphicsSystem;
    CheckMsgVoid(registry->GetPipeline()->GetInterface(&graphicsSystem), Error, "Expected graphics system to be installed");

    // Get memory
    interpreter = graphicsSystem->GetInterpreter();

    CheckVoid(GetSystem()->CreateActor(GetScene(), &label));

    SetTransform(label->GetTransform());
}

void UICGTime::Update(double delta) {
    double weight = 0.1;

    avgCDelta = (1.0 - weight) * avgCDelta + weight * system->GetDelta();
    avgGDelta = (1.0 - weight) * avgGDelta + weight * interpreter->GetGPUDelta();

    if ((elapsed += delta) > 0.05) {
        elapsed = 0;
        label->SetText(Core::String("CPU ") + static_cast<int>(1.0 / avgCDelta) + " | GPU " + static_cast<int>(1.0 / avgGDelta));
    }
}
