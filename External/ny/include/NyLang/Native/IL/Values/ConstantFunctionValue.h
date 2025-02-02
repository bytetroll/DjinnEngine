//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/IConstant.h>

namespace NyLang::IL {
    class Function;

    class ConstantFunctionValue : public IConstant {
    public:
        LANG_CLASSID("ConstantFunctionValue");

        /// Construct
        /// \param type
        ConstantFunctionValue(Function* function);

        /// Overrides
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get function
        /// \return
        Function* GetFunction() const {
            return function;
        }

    private:
        Function* function = nullptr;
    };
}
