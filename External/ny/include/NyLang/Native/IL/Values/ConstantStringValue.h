//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/IConstant.h>
#include <NyLang/Core/Context.h>

namespace NyLang::IL {
    class ConstantStringValue : public IConstant {
    public:
        LANG_CLASSID("ConstantStringValue");

        /// Construct
        /// \param type
        ConstantStringValue(Context& context, const std::string& str);

        /// Overrides
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get string
        /// \return
        const std::string& GetString() const {
            return string;
        }

    private:
        std::string string;
    };
}
