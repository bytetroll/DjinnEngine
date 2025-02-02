//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class MemberInitializerListExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.MemberInitializerListExtension");
        LANG_ACTIONID(Action, "Lang.MemberInitializerListExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static MemberInitializerListExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        MemberInitializerListExtension(Registry*registry);

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
