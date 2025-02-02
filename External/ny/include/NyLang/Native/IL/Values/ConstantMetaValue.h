//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Backend/IL/PrettyPrintContext.h>
#include <NyLang/Backend/IL/IConstant.h>

namespace NyLang::IL {
    class Class;

    class ConstantMetaValue : public IConstant {
    public:
        using Member = std::pair<String, IConstant*>;

        LANG_CLASSID("ConstantClassValue");

        /// Construct
        /// \param type
        ConstantMetaValue(const String& name, const Vector<Member>& values = {});

        /// Overrides
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;

        /// Get values
        /// \return
        const auto& GetValues() const {
            return values;
        }

        /// Get name
        /// \return
        const String& GetName() const {
            return name;
        }

        /// Get member with name
        /// \param name
        /// \return
        IConstant* GetMemberWithName(const String& name);

    private:
        String name;
        Vector<Member> values;
    };
}
