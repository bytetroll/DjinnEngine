//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class TokenizerNumberExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.TokenizerNumberExtension");
        LANG_ACTIONID(Action, "Lang.TokenizerNumberExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static TokenizerNumberExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        TokenizerNumberExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
    };
}
