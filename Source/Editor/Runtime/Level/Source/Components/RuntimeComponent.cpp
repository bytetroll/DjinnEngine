//
// Created by Dev on 7/13/2018.
//

#include <Editor/Components/RuntimeComponent.h>
#include <Base/IPipeline.h>

DJINN_NS2(Editor);

RuntimeComponent::RuntimeComponent(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISceneComponent(registry, outer, host) {

}
