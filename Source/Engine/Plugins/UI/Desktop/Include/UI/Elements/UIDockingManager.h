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

#include <UI/IUIElement.h>
#include <Math/Vector2.h>
#include <UI/UIMLAnnotation.h>
#include <Reflection/Attributes/Proxy.h>

namespace Djinn::UI {
    class UILayerTransformComponent;
    class UIDockingManager;
    class UIDockingFitter;
    class UIDocking;
    class IDockable;

    ClassAttr(UIML::Class("dockingmanager"))
    class DJINN_EXPORT_PLUGINS_UI_DESKTOP UIDockingManager : public IUIElement {
    public:
        DJINN_COM_CLASS();

        UIDockingManager(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Overrides
        void StaticInitialize() override;

    public:
        /// Add a child
        /// \param child
        Attr(UIML::SocketAttach())
        void AddChild(IUITransformComponent *transform);

        /// Remove a child
        /// \param child
        Attr(UIML::SocketDetach())
        void RemoveChild(IUITransformComponent *transform);

        /// Set a child as floating
        /// \param transform
        void SetFloating(IUITransformComponent *transform);

        /// Set a child as the foreground transform
        /// \param transform
        void SetForeground(IUITransformComponent* transform);

        /// Is a transform floating?
        /// \param transform
        /// \return
        bool IsFloating(IUITransformComponent* transform);

    public:
        /// Find the docking element under a given position
        /// \param position
        /// \return
        IDockable* FindDocking(IDockable* ignore, const Math::Vector2& position);

        /// Open the docking fitters
        /// \param transform
        void ShowFitter(IDockable* ignore, IUITransformComponent* transform);

        /// Stop fitting the current transform
        void HideFitter();

    private:
        IDockable* FindDocking(IDockable* dockable, IDockable* ignore, const Math::Vector2& position);

    private:
        UILayerTransformComponent *layout;

        Math::Vector2 mousePosition;

        UIDockingFitter* fitter{nullptr};

        Core::Array<IDockable*> dockers;

        IUITransformComponent* foregroundTransform = nullptr;
    };
}