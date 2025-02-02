//
// Created by Dev on 7/14/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/ArrayView.h>

namespace Djinn::Game {
    class IWorldFeature : public COM::TUnknown<IWorldFeature> {
    public:
        DJINN_COM_INTERFACE();

        IWorldFeature(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Add a package
        /// \param cid
        virtual void AddPackage(const COM::ClassID& cid) = 0;

        /// Enumerate all packages
        /// \return
        virtual Core::ArrayView<COM::ClassID> EnumeratePackages() = 0;

        /// Helper
        /// \tparam T
        template<typename T>
        void AddPackage() {
            AddPackage(T::kCID);
        }
    };
}