//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class AccessArrayExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.AccessArrayExtension");
        LANG_ACTIONID(Action, "Lang.AccessArrayExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static AccessArrayExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        AccessArrayExtension(Registry*registry);

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
