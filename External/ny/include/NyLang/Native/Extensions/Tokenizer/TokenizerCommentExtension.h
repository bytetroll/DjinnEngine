//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class TokenizerCommentExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.TokenizerCommentExtension");
        LANG_ACTIONID(Action, "Lang.TokenizerCommentExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static TokenizerCommentExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        TokenizerCommentExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
    };
}
