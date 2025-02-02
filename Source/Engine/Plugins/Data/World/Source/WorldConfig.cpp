//
// Created by Dev on 2/28/2018.
//

#include <Data/WorldConfig.h>
#include <Host/IRegistry.h>
#include <Base/DefaultLogs.h>
#include <Data/IWorldSetting.h>

DJINN_NS2(Data);

WorldConfig::WorldConfig(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

}

IWorldSetting *WorldConfig::Get(COM::ClassID cid) {
    auto &ptr = settings[cid];
    if (!ptr) {
        if (!registry->CreateClass(cid, nullptr, &ptr)) {
            ErrorLog(this).Write("Failed to get world setting, could not create class");
            return nullptr;
        }
    }
    return ptr;
}

