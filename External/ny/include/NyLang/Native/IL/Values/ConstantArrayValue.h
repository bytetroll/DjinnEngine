//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/IConstant.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class ConstantArrayValue : public IConstant {
    public:
        LANG_CLASSID("ConstantArrayValue");

        /// Construct
        /// \param type
        ConstantArrayValue(Context& context, IType* type, const PtrVector<IValue>& values);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get value
        /// \return
        const PtrVector<IValue>& GetValues() const {
            return values;
        }

        /// Get contained type
        /// \return
        IType* GetContained() const {
            return contained;
        }

    private:
        PtrVector<IValue> values;
        IType* contained = nullptr;
    };
}
