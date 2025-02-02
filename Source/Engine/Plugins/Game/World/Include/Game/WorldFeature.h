//
// Created by Dev on 7/14/2018.
//
#pragma once


#include <Core/Array.h>
#include "IWorldFeature.h"

namespace Djinn::Game {
    class DJINN_EXPORT_PLUGINS_GAME_WORLD WorldFeature : public IWorldFeature {
    public:
        DJINN_COM_CLASS();

        WorldFeature(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void AddPackage(const COM::ClassID &cid) override;
        Core::ArrayView<COM::ClassID> EnumeratePackages() override;

    private:
        Core::Array<COM::ClassID> packages;
    };
}