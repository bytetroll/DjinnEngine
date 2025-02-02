//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>

namespace NyLang {
    class UtilExtension;

    class AnnotationExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.AnnotationExtension");
        LANG_ACTIONID(Action, "Lang.AnnotationExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static AnnotationExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        AnnotationExtension(Registry*registry);

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
