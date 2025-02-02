//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Semantic/SemanticScopeStack.h>
#include <functional>

namespace NyLang {
    class UtilExtension;

    class MixinExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.MixinExtension");
        LANG_ACTIONID(Action, "Lang.MixinExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static MixinExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        MixinExtension(Registry*registry);

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
