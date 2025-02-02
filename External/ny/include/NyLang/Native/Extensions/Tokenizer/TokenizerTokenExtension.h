//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/COM/IUnknown.h>
#include <NyLang/Frontend/Token/Token.h>
#include <set>
#include <map>
#include <NyLang/Core/StdTypes.h>

namespace NyLang {
    class TokenizerTokenExtension : public IUnknown {
    public:
        LANG_CLASSID("Lang.TokenizerTokenExtension");
        LANG_ACTIONID(Action, "Lang.TokenizerTokenExtension.Action");

        /// Construct and install instance into registry
        /// \return
        static TokenizerTokenExtension* Install(Registry*registry);

        /// Construct
        /// \param registry
        TokenizerTokenExtension(Registry*registry);

        /// Initialize
        /// \return
        bool Initialize();

        /// Add a string token
        /// \param id
        /// \param str
        void AddToken(TokenID id, const String& str);

        /// Overrides
        virtual ClassID GetClassID() override;

    private:
        Registry* registry = nullptr;

        // Segment set
        std::set<String> segments;

        // Token map
        Map<String, TokenID> tokens;
    };
}
