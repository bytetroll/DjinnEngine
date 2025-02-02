//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Core/String.h>
#include <Data/IAssetConfiguration.h>
#include "MeshSourceFormat.h"

namespace Djinn::Data {
    class MeshConfiguration : public IAssetConfiguration {
    public:
        DJINN_COM_CLASSID();

        /// Override
        COM::ClassID GetClassID() const override {
            return kCID;
        }

        /// Override
        USize Hash() override {
            return CombineHash(format, includePath);
        }

        /// Override
        IAssetConfiguration *Copy() override {
            return new MeshConfiguration(*this);
        }

        /// Source format of this mesh
        MeshSourceFormat format = MeshSourceFormat::eNone;

        /// Include path of this mesh compilation
        Core::String includePath = "";
    };
}