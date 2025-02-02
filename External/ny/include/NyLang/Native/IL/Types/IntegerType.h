//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IType.h>

namespace NyLang::IL {
    class IntegerType : public IType {
    public:
        LANG_CLASSID("IntegerType");

        /// Construct
        /// \param bitWidth
        IntegerType(int bitWidth, bool isUnsigned);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Is this integer unsigned
        /// \return
        bool IsUnsigned() const {
            return isUnsigned;
        }

    private:
        bool isUnsigned;
    };
}
