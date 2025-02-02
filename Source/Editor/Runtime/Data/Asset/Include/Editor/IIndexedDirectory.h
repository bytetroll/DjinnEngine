//
// Created by Dev on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Base/Paths.h>
#include <Core/ArrayView.h>
#include <Core/StringView.h>

namespace Djinn::Editor {
    class IIndexedDirectory : public COM::TUnknown<IIndexedDirectory> {
    public:
        DJINN_COM_INTERFACE();

        IIndexedDirectory(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get the name of this directory
        /// \return
        virtual Core::StringView GetName() = 0;

        /// Register an asset
        /// \param path
        virtual void Register(const Base::PathID &path) = 0;

        /// Register a directory
        /// \param directory
        virtual void Register(IIndexedDirectory *directory) = 0;

        /// Get the contained assets
        /// \return
        virtual Core::ArrayView<Base::PathID> GetAssets() = 0;

        /// Get the contained directories
        /// \return
        virtual Core::ArrayView<IIndexedDirectory *> GetDirectories() = 0;
    };
}