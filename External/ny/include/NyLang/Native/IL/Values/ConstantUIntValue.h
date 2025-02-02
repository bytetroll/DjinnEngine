//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/IConstant.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class ConstantUIntValue : public IConstant {
    public:
        LANG_CLASSID("ConstantUIntValue");

        /// Construct
        /// \param type
        ConstantUIntValue(Context& context, int bitWidth, std::uint64_t value);

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
        std::uint64_t value;
    };
}
