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
 on 8/22/2018.
//
#pragma once

#include "IUIElement.h"
#include "IUILayer.h"
#include <Game/IScene.h>
#include <UI/ElementComponents/IUITransformComponent.h>

namespace Djinn::Graphics {
    class IOutput;
}

namespace Djinn::UI {
    class Style;

    class IUICanvas : public Game::IScene, public IUILayer {
    public:
        DJINN_COM_INTERFACE();

        IUICanvas(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IScene(registry, outer, host) {

        }

        /// Trace a mouse position
        /// \param position - The screen space mouse position
        /// \param out - The canvas space mouse position
        /// \return - True if the mouse is in canvas space
        virtual bool Trace(const Math::Vector2 &position, Math::Vector2 *out) = 0;

        /// Trace a child element
        /// \param point
        /// \return
        virtual IUIElement *TraceChild(const Math::Vector2 &point, TransformTraceMode mode) = 0;

        /// Add a child to this canvas
        /// \param child
        virtual void AddElement(IUIElement *element) = 0;

        /// Remove a child from this canvas
        /// \param child
        virtual void RemoveElement(IUIElement *element) = 0;

        /// Assign this canvas as a proxy
        /// \return
        virtual void SetProxy(IUICanvas* canvas) = 0;

        /// Get the style of this canvas
        /// \return
        virtual Style* GetStyle() = 0;

        /// Overrides
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown *>(this);
                    return COM::kOK;
                case IScene::kIID:
                    *out = static_cast<IScene *>(this);
                    return COM::kOK;
                case IUICanvas::kIID:
                    *out = static_cast<IUICanvas *>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Set the output target
        /// \param output
        void SetOutput(Graphics::IOutput *output) {
            this->output = output;
        }

        /// Get the output target
        /// \return
        Graphics::IOutput *GetOutput() const {
            return output;
        }

        /// Set the scaling mode
        /// \param scaleOutput
        void SetScaling(bool scaleOutput) {
            this->scaleOutput = scaleOutput;
        }

        /// Get the scaling mode
        /// \return
        bool IsScaled() const {
            return scaleOutput;
        }

    protected:
        /// Output target
        Graphics::IOutput *output = nullptr;

        /// Scale according to output size?
        bool scaleOutput = false;
    };
}