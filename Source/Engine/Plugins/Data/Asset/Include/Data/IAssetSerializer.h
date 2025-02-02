//
// Created by Dev on 9/4/2017.
//
#pragma once

#include <COM/ID.h>
#include <COM/IUnknown.h>
#include <Serialization/ISerializer.h>

namespace Djinn {
    namespace Data {
        class DJINN_EXPORT_PLUGINS_DATA_ASSET IAssetSerializer : public Serialization::ISerializer {
        public:
            DJINN_COM_INTERFACE();

            IAssetSerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : ISerializer(registry, outer, host) {

            }
        };
    }
}