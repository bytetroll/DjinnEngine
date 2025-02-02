//
// Created by Dev on 8/27/2018.
//

#include <Game/SceneOcclusion.h>

DJINN_NS2(Game);

SceneOcclusion::SceneOcclusion(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneOcclusion(registry, outer, host) {

}

COM::Result SceneOcclusion::Initialize() {
    return COM::kOK;
}
