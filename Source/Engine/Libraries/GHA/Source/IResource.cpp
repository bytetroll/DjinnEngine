//
// Created by Dev on 3/9/2018.
//

#include <GHA/IResource.h>

DJINN_NS2(GHA);

namespace {
    Core::AtomicCounter _uid;
}

IResource::IResource(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host), uid(static_cast<USize>(_uid++)) {

}
