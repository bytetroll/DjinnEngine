//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/IType.h>

namespace NyLang::IL {
    class ArrayType : public IType {
    public:
        LANG_CLASSID("ArrayType");

        /// Construct
        /// \param bitWidth
        ArrayType(IType* contained, int count);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get contained
        /// \return
        const auto& GetContained() const {
            return contained;
        }

        /// Get number of elements
        /// \return
        int GetCount() const {
            return count;
        }

    private:
        // Contained type
        IType* contained = nullptr;

        // Number of elements
        int count;
    };
}
