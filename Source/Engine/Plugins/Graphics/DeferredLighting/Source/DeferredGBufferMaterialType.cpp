//
// Created by Dev on 4/7/2018.
//

#include <Graphics/DeferredGBufferMaterialType.h>

DJINN_NS2(Graphics);

DeferredGBufferMaterialType::DeferredGBufferMaterialType(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IMaterialType(registry, outer, host) {

}

Core::String DeferredGBufferMaterialType::GetHeaderPath() {
    return "Material/DeferredGBufferMaterialHeader.dsf";
}

Core::String DeferredGBufferMaterialType::GetKernelPath() {
    return "Material/DeferredGBufferMaterialKernel.dsf";
}
