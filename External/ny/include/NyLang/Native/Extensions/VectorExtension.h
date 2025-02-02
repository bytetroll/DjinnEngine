//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;
    class BuiltinExtension;

    class VectorExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.VectorExtension");

        /// Construct and install instance into registry
        /// \return
        static VectorExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        VectorExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
        BuiltinExtension* builtinExtension = nullptr;
    };
}
