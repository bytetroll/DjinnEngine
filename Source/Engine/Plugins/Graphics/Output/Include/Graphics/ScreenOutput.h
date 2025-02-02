//
// Created by Dev on 4/8/2018.
//
#pragma once

#include <GHA/Size.h>
#include <Core/Optional.h>
#include <Graphics/PassBuilder.h>
#include <Graphics/ID.h>
#include "IOutput.h"

namespace Djinn::Graphics {
    class IGraphicsSystem;
    class DJINN_EXPORT_PLUGINS_GRAPHICS_OUTPUT ScreenOutput : public IOutput {
    public:
        DJINN_COM_CLASS();

        ScreenOutput(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this output
        /// \param size
        /// \return
        COM::Result Initialize();

        /// Overrides
        COM::Result Configure(PassBuilder builder) override;
        GHA::Size GetSize() override;
        Texture GetTexture(PassBuilder builder) override;

    private:
        IGraphicsSystem *system;
        Texture texture;
    };
}