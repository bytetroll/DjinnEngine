//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class StaticExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.StaticExtension");
        LANG_ACTIONID(Action, "Lang.StaticExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static StaticExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        StaticExtension(Registry*registry);

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
