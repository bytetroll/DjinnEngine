//
// Created by Dev on 1/28/2018.
//
#pragma once

#include <Data/IAssetConfiguration.h>

namespace Djinn::Data {
    class MaterialConfiguration : public IAssetConfiguration {
    public:
        DJINN_COM_CLASSID();

        /// Override
        COM::ClassID GetClassID() const override {
            return kCID;
        }

        /// Override
        USize Hash() override {
            return 0;
        }

        /// Override
        IAssetConfiguration *Copy() override {
            return new MaterialConfiguration(*this);
        }
    };
}