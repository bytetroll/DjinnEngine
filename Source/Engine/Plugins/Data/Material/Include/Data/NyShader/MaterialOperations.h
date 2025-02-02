//
// Created by Dev on 11/29/2017.
//
#pragma once

#include <COM/Result.h>
#include <NyLang/Core/Shared.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <Core/String.h>

namespace NyLang {
    class Registry;
}

namespace Djinn::Data::NyShader {
    class MaterialInbuilts : public NyLang::IUnknown {
    public:
        LANG_CLASSID("MaterialInbuilts");

        MaterialInbuilts();

        /// Overrides
        virtual NyLang::ClassID GetClassID() override;

        /// Paths
        Core::String layoutPath;
        Core::String typePath;
    };

    extern DJINN_EXPORT_PLUGINS_DATA_MATERIAL COM::Result InstallMaterialOperations(NyLang::Registry* registry);
}