//
// Created by Dev on 10/25/2017.
//
#pragma once

#include <Data/IAssetSerializer.h>

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_MESH IMeshSerializer : public IAssetSerializer {
    public:
        DJINN_COM_INTERFACE();

        IMeshSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IAssetSerializer(registry, outer, host) {

        }
    };
}