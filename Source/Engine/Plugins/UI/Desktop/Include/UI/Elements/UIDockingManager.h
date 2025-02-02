//
// Created by Dev on 8/22/2018.
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