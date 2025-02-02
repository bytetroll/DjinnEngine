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
#pragma once

#include <COM/IUnknown.h>
#include "ComponentAttribute.h"

namespace Djinn::Game {
    class IActor;
    class IActorComponent;
}

namespace Djinn::Editor {
    class IComponentHost : public COM::TUnknown<IComponentHost> {
    public:
        DJINN_COM_INTERFACE();

        IComponentHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Add an attribute set to a component
        /// \param cid
        /// \param attributes
        /// \return
        virtual COM::Result AddAttribute(const COM::ClassID& cid, const ComponentAttributeSet& attributes) = 0;

        /// Create a component
        /// \param entity
        /// \param cid
        /// \param out
        /// \return
        virtual COM::Result Create(Game::IActor* actor, const COM::ClassID& cid, Game::IActorComponent** out) = 0;

        /// Helper
        template<typename T>
        COM::Result Create(Game::IActor* actor, Game::IActorComponent** out) {
            return Create(actor, T::kCID, out);
        }
    };
}