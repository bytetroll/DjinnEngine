//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Data/IAssetSerializer.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_UIML IUIMLSerializer : public IAssetSerializer {
    public:
        DJINN_COM_INTERFACE();

        IUIMLSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetSerializer(registry, outer, host) {

        }
    };
}