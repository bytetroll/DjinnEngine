//
// Created by Dev on 11/29/2017.
//
#pragma once

#include <COM/Result.h>
#include <NyLang/Core/Shared.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <Core/String.h>
#include <Core/HashSet.h>
#include <Host/IRegistry.h>

namespace NyLang {
    class Registry;
}

namespace Djinn::Data::NyShader {
    class ShaderContext : public NyLang::IUnknown {
    public:
        LANG_CLASSID("ShaderContext");

        ShaderContext();

        void Reset();

        /// Overrides
        virtual NyLang::ClassID GetClassID() override;

        /// Paths
        Core::HashSet<Core::String> paths;
    };

    extern DJINN_EXPORT_PLUGINS_DATA_SHADER COM::Result InstallOperations(Host::IRegistry* _registry, NyLang::Registry* registry);
}