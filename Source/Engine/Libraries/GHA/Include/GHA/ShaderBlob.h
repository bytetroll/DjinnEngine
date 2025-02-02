//
// Created by Dev on 12/9/2017.
//
#pragma once

#include <COM/ID.h>
#include <Core/Array.h>
#include <Core/HashMap.h>
#include <Core/String.h>
#include <COM/IUnknown.h>
#include "Stage.h"
#include "InputAssemblyDesc.h"
#include "ID.h"

namespace Djinn::GHA {
    class ICompiledShaderBlob : public COM::TUnknown<ICompiledShaderBlob> {
    public:
        DJINN_COM_INTERFACE();

        ICompiledShaderBlob(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }
    };

    struct ShaderBinding {
        BindID id;
        BindID target;
        Core::String name;
    };

    struct ShaderBlob {
        /// Stage
        GHA::Stage stage;

        /// Input assembly
        GHA::InputAssemblyDesc inputAssembly;

        /// Bindings
        Core::Array<ShaderBinding> bindings;

        /// Compiled shaders
        Core::HashMap<COM::ClassID, ICompiledShaderBlob*> compiledBlobs;
    };
}