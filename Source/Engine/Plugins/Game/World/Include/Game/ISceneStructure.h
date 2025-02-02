//
// Created by Dev on 8/25/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Math/Matrix4.h>
#include <Core/ArrayView.h>

namespace Djinn::Game {
    class ISceneOcclusion;
    class ISceneProxy;
    struct SceneObject;
    struct SceneStructureViewport;

    class ISceneStructure : public COM::TUnknown<ISceneStructure> {
    public:
        DJINN_COM_INTERFACE();

        ISceneStructure(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Build the structure
        /// \return
        virtual COM::Result Build() = 0;

        /// Build a viewport
        /// Structure must be built before building viewports
        /// TS: This method is thread safe as long as no other method than BuildViewport is being invoked
        /// \param queryable
        /// \param transform
        /// \return
        virtual COM::Result BuildViewport(COM::IQueryable *queryable, SceneStructureViewport* viewport, const Math::Matrix4& transform) = 0;

        /// Set the scene occlusion
        /// \return
        virtual void SetOcclusion(ISceneOcclusion* occlusion) = 0;

        /// Get the scene occlusion
        /// \return
        virtual ISceneOcclusion* GetOcclusion() = 0;

        /// Register a scene proxy
        /// \param proxy
        virtual void Register(ISceneProxy *proxy) = 0;

        /// Deregister a scene proxy
        /// \param proxy
        virtual void Deregister(ISceneProxy *proxy) = 0;
    };
}