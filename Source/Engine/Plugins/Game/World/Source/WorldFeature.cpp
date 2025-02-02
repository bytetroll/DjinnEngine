//
// Created by Dev on 7/14/2018.
//

#include <Game/WorldFeature.h>

DJINN_NS2(Game);

WorldFeature::WorldFeature(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IWorldFeature(registry, outer, host) {

}

void WorldFeature::AddPackage(const COM::ClassID &cid) {
    packages.Add(cid);
}

Core::ArrayView<COM::ClassID> WorldFeature::EnumeratePackages() {
    return packages;
}
