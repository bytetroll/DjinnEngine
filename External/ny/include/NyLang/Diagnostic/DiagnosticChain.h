//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <set>

namespace NyLang {
    class IMessage;

    class DiagnosticChain {
    public:
        void PushScope(IMessage* msg);
        void PopScope(IMessage* msg);
        bool IsScope(IMessage* msg);

    private:
        std::set<IMessage*> scopes;
    };
}
