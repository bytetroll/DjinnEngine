//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class UtilExtension;

    class FunctionExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.FunctionExtension");
        LANG_ACTIONID(Action, "Lang.FunctionExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static FunctionExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        FunctionExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
    };
}
