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
 on 5/9/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Data/Asset.h>
#include <Data/IUIMLAsset.h>
#include <Core/StringView.h>
#include <UI/UIMLID.h>
#include <Core/Visitor.h>

namespace Djinn::UI {
    class IUIElement;
    class IUICanvas;

    class IUIMLHost : public COM::TUnknown<IUIMLHost> {
    public:
        DJINN_COM_INTERFACE();

        IUIMLHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Register a name against a class ID
        /// \param cid
        /// \param name
        virtual void Register(COM::ClassID cid, const Core::StringView& name) = 0;

        /// Build an UIML asset within a given canvas
        /// Does not add elements
        /// \param canvas
        /// \param asset
        /// \param model - Model in which all events are routed to
        /// \param visitor
        /// \return
        virtual COM::Result Build(IUICanvas* canvas, Data::IUIMLAsset *asset, COM::IUnknown* model, const Core::Visitor<void(IUIElement*)>& visitor) = 0;

        /// Register a name against a class
        /// \param name
        template<typename T>
        void Register(const Core::StringView& name) {
            return Register(T::kCID, name);
        }
    };
}