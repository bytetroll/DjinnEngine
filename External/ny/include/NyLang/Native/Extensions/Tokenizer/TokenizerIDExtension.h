//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class TokenizerIDExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.TokenizerIDExtension");
        LANG_ACTIONID(Action, "Lang.TokenizerIDExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static TokenizerIDExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        TokenizerIDExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
    };
}
