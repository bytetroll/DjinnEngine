//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IType.h>
#include <NyLang/Backend/IL/PrettyPrintContext.h>

namespace NyLang::IL {
    class PointerType : public IType {
    public:
        LANG_CLASSID("PointerType");

        /// Construct
        /// \param bitWidth
        PointerType(IType* contained);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get contained
        /// \return
        const auto& GetContained() const {
            return contained;
        }

    private:
        // Contained type
        IType* contained = nullptr;
    };
}
