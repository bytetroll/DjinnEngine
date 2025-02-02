//
// Created by Dev on 11/30/2017.
//
#pragma once

#include <lunarglass/Core/PrivateManager.h>
#include <COM/IUnknown.h>
#include <GHA/Stage.h>
#include <Core/String.h>
#include <Data/ShaderCompilerNILDesc.h>

namespace llvm {
    class BasicBlock;
}

namespace Djinn::GHA::Vulkan {
    class GlassManager : public gla::PrivateManager {
    public:
        /// Construct
        /// \param stage
        GlassManager(Host::IRegistry *registry, const Data::ShaderCompileNILDesc& desc);
        virtual ~GlassManager();

        /// Overrides
        virtual void createContext() override;
        virtual void clear() override;

        /// Compile the current kernel to glsl
        /// \param glsl
        /// \return
        COM::Result Compile(Core::String &out);

    private:
        void PatchTextureMapInlining(llvm::BasicBlock* block);

        Host::IRegistry *registry;
    };
}
