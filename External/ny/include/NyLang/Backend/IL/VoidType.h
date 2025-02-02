//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IType.h>

namespace NyLang::IL {
    class VoidType : public IType {
    public:
        LANG_CLASSID("VoidType");

        /// Construct
        VoidType();

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;
    };
}
