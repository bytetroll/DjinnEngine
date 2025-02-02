//
// Created by Dev on 6/28/2018.
//
#pragma once

#include <Game/ISceneState.h>

namespace Djinn::Host {
    class IPipeline;
}

namespace Djinn::Editor {
    class ILevel;

    class ILevelLayoutSceneState : public Game::ISceneState {
    public:
        DJINN_COM_INTERFACE();

        ILevelLayoutSceneState(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : ISceneState(registry, outer, host) {

        }

        /// Get the level
        /// \return
        virtual ILevel *GetLevel() = 0;

        /// Get the internally hosted pipeline
        /// \return
        virtual Host::IPipeline *GetPipeline() = 0;

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case COM::IUnknown::kIID:
                    *out = static_cast<IUnknown *>(this);
                    return COM::kOK;
                case Game::ISceneState::kIID:
                    *out = static_cast<ISceneState *>(this);
                    return COM::kOK;
                case ILevelLayoutSceneState::kIID:
                    *out = static_cast<ILevelLayoutSceneState *>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }
    };
}