//
// Created by Dev on 10/23/2017.
//
#pragma once

#include <Core/Array.h>
#include "IMaterialCompiler.h"

namespace Djinn::Data {
    class MaterialCompilerPool;
    class IShaderCompiler;

    class DJINN_EXPORT_PLUGINS_DATA_MATERIAL MaterialCompiler : public IMaterialCompiler {
    public:
        DJINN_COM_CLASS();

        MaterialCompiler(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this compiler
        /// \return
        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Register(IMaterialLayout *layout) override;
        virtual COM::Result Register(IMaterialType *type) override;
        virtual COM::Result Compile(Core::IStream *stream, IAssetConfiguration *config, IAsset **out) override;

    private:
        Core::Array<IMaterialLayout*> layouts;
        Core::Array<IMaterialType*> types;
        IShaderCompiler* shaderCompiler;
        MaterialCompilerPool* defaultPool;
    };
}