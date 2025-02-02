//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/IConstant.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class ConstantDoubleValue : public IConstant {
    public:
        LANG_CLASSID("ConstantDoubleValue");

        /// Construct
        /// \param type
        ConstantDoubleValue(Context& context, double value);

        /// Overrides
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get value
        /// \return
        double GetValue() const {
            return value;
        }

    private:
        double value;
    };
}
