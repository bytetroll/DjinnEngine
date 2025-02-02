//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class MessageExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.MessageExtension");
        LANG_ACTIONID(Action, "Lang.MessageExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static MessageExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        MessageExtension(Registry*registry);

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
