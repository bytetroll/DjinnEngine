//
// Created by Dev on 4/5/2018.
//

#include <Data/BaseMaterialAsset.h>

DJINN_NS2(Data);

BaseMaterialAsset::BaseMaterialAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMaterialAsset(registry, outer, host) {

}

COM::Result BaseMaterialAsset::GetShader(const MaterialQuery &query, ShaderAsset **out) {
    auto it = shaders.Find(query.Hash());
    if (it == shaders.End()) {
        return COM::kError;
    }

    *out = (*it).second;
    return COM::kOK;
}
