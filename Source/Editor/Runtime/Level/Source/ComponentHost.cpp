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
 on 7/15/2018.
//

#include <Editor/ComponentHost.h>
#include <Editor/Components/RuntimeComponent.h>
#include <Game/IActor.h>

DJINN_NS2(Editor);

COM::Result ComponentHost::AddAttribute(const COM::ClassID &cid, const ComponentAttributeSet &attributes) {
    auto &com = componentMap[cid];

    // Add attributes
    com.attributes += attributes;

    // OK
    return COM::kOK;
}

COM::Result ComponentHost::Create(Game::IActor *actor, const COM::ClassID &cid, Game::IActorComponent **out) {
    auto &com = componentMap[cid];

    // Run in editor?
    if (com.attributes & ComponentAttribute::eRunInEditor) {
        *out = actor->CreateComponent(cid);
    } else {
        // Non editor components are always runtime components
        auto runtime = actor->CreateComponent<RuntimeComponent>();
        runtime->SetComponentID(cid);

        // ...
        *out = runtime;
    }

    // OK
    return COM::kOK;
}
