//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IType.h>

namespace NyLang::IL {
    class Class;

    class ClassType : public IType {
    public:
        LANG_CLASSID("ClassType");

        /// Construct
        /// \param bitWidth
        ClassType(Class* contained);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get contained
        /// \return
        Class* GetContained() const {
            return contained;
        }

    private:
        // Contained class
        Class* contained = nullptr;
    };
}
