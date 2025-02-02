//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class IfExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.IfExtension");
        LANG_ACTIONID(Action, "Lang.IfExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static IfExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        IfExtension(Registry*registry);

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
