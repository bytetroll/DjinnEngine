//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;
    class CastExtension;

    class ArrayExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.ArrayExtension");
        LANG_ACTIONID(Action, "Lang.ArrayExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static ArrayExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        ArrayExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;
        UtilExtension* utilExtension = nullptr;
        CastExtension* castExtension = nullptr;
    };
}
