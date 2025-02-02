//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class TokenizerStringExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.TokenizerStringExtension");
        LANG_ACTIONID(Action, "Lang.TokenizerStringExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static TokenizerStringExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        TokenizerStringExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
    };
}
