//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/IConstant.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class ConstantIntValue : public IConstant {
    public:
        LANG_CLASSID("ConstantIntValue");

        /// Construct
        /// \param type
        ConstantIntValue(Context& context, int bitWidth, std::int64_t value);

        /// Overrides
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get value
        /// \return
        std::int64_t GetValue() const {
            return value;
        }

    private:
        std::int64_t value;
    };
}
