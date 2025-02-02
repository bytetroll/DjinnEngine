//
// Created by Dev on 5/9/2018.
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