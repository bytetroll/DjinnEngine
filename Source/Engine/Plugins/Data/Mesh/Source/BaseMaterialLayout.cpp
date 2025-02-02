//
// Created by Dev on 4/7/2018.
//

#include <Data/BaseMaterialLayout.h>

DJINN_NS2(Data);

BaseMaterialLayout::BaseMaterialLayout(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMaterialLayout(registry, outer, host) {

}

Core::String BaseMaterialLayout::GetPath() {
    return "Material/BaseMaterialLayout.dsf";
}
