//
// Created by Dev on 11/28/2017.
//
#pragma once

#include <Core/Unique.h>
#include <Core/String.h>
#include <Core/Array.h>
#include <Data/IAssetConfiguration.h>

namespace Djinn::Data {
    class ShaderConfiguration : public IAssetConfiguration {
    public:
        DJINN_COM_CLASSID();

        /// Add an entry point to this shader
        /// \param value
        /// \return
        ShaderConfiguration &AddEntryPoint(const Core::String &value) {
            entryPoints.Add(value);
            return *this;
        }

        /// Set all entry points
        /// \param value
        /// \return
        ShaderConfiguration &EntryPoints(const Core::Array<Core::String> &value) {
            entryPoints.Add(value);
            return *this;
        }

        /// Override
        COM::ClassID GetClassID() const override {
            return kCID;
        }

        /// Override
        USize Hash() override {
            USize hash = 0;
            for (auto &str : entryPoints) {
                hash = CombineHash(hash, str);
            }
            return hash;
        }

        /// Override
        IAssetConfiguration *Copy() override {
            return new ShaderConfiguration(*this);
        }

        // Entry points to compile
        Core::Array<Core::String> entryPoints;
    };
}