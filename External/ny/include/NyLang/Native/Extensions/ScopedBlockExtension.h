//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class ScopedBlockExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.ScopedBlockExtension");
        LANG_ACTIONID(Action, "Lang.ScopedBlockExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static ScopedBlockExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        ScopedBlockExtension(Registry*registry);

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
