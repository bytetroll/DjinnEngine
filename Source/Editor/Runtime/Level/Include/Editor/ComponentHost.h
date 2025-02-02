//
// Created by Dev on 7/11/2018.
//
#pragma once

#include <Core/HashMap.h>
#include "IComponentHost.h"

namespace Djinn::Editor {
    class ComponentHost : public IComponentHost {
    public:
        DJINN_COM_CLASS();

        ComponentHost(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IComponentHost(registry, outer, host) {

        }

        /// Overrides
        COM::Result AddAttribute(const COM::ClassID &cid, const ComponentAttributeSet &attributes) override;
        COM::Result Create(Game::IActor *actor, const COM::ClassID &cid, Game::IActorComponent **out) override;

    private:
        struct Component {
            ComponentAttributeSet attributes;
        };

        Core::HashMap<COM::ClassID, Component> componentMap;
    };
}