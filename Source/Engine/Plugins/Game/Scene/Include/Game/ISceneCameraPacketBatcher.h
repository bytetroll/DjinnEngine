//
// Created by Dev on 8/30/2018.
//
#pragma once

#include <Graphics/IScenePacketBatcher.h>

namespace Djinn::Game {
    class ISceneCameraPacketBatcher : public COM::TUnknown<ISceneCameraPacketBatcher>, public Graphics::IScenePacketBatcher {
    public:
        DJINN_COM_INTERFACE();

        ISceneCameraPacketBatcher(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }
    };
}