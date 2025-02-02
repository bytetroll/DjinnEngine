//
// Created by Dev on 2/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Async/Async.h>
#include <Game/StepMode.h>
#include <Core/ArrayView.h>
#include <Game/IStepSystem.h>
#include "IScene.h"

namespace Djinn::Host {
    class IPipeline;
}

namespace Djinn::Data {
    class WorldConfig;
}

namespace Djinn::Base {
    class IPipeline;
}

namespace Djinn::Game {
    class IWorldSystem;
    class IWorldPackage;

    class IWorld : public IStepSystem {
    public:
        DJINN_COM_INTERFACE();

        IWorld(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host) : IStepSystem(registry, outer, host) {

        }

        /// Register a system
        /// \param system
        /// \return
        virtual COM::Result Register(IWorldSystem *system) = 0;

        /// Deregister a system
        /// \param system
        /// \return
        virtual COM::Result Deregister(IWorldSystem *system) = 0;

        /// Register a package
        /// This does not install the package
        /// \param package
        virtual void Register(IWorldPackage *package) = 0;

        /// Register a scene type
        /// \param iid - Interface which is supplied during scene creation
        /// \param cid - Actual instantiated class
        virtual void RegisterSceneType(COM::InterfaceID iid, COM::ClassID cid) = 0;

        /// Helper
        template<typename T>
        void RegisterSceneType() {
            RegisterSceneType(T::kIID, T::kCID);
        }

        /// Create a scene
        /// \param state - may be null
        /// \param out
        /// \return
        virtual COM::Result CreateScene(COM::InterfaceID iid, ISceneState *state, IScene **out) = 0;

        /// Helper
        template<typename T>
        COM::Result CreateScene(ISceneState *state, T **out) {
            IScene *scene;
            if (COM::CheckedResult result = CreateScene(T::kIID, state, &scene)) {
                return result;
            }

            return scene->QueryInterface<T>(out);
        }

        /// Get all scenes
        /// \return
        virtual Core::ArrayView<IScene *> GetScenes() = 0;

        /// Get the internally hosted pipeline
        /// \return
        virtual Host::IPipeline *GetPipeline() = 0;

        /// Get the system pipeline
        /// \return
        virtual Base::IPipeline *GetSystemPipeline() = 0;

        /// Get configuration
        /// \return
        virtual Data::WorldConfig *GetConfig() = 0;

        /// Asynchronous methods
        Asyncf(Run);
    };
}