//
// Created by Dev on 7/11/2018.
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