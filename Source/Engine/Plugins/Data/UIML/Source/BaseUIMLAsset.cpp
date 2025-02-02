//
// Created by Dev on 5/5/2018.
//

#include <Data/BaseUIMLAsset.h>

DJINN_NS2(Data);

BaseUIMLAsset::BaseUIMLAsset(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUIMLAsset(registry, outer, host) {

}

UIMLContent *BaseUIMLAsset::GetContent() {
    return &content;
}
