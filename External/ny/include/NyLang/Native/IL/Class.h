//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Backend/IL/IValue.h>
#include <NyLang/Backend/IL/Container.h>
#include <NyLang/Backend/IL/ExecutionBlock.h>

namespace NyLang::IL {
    class Class : public Container {
    public:
        LANG_CLASSID("Class");

        /// Construct
        Class(Context& context, const String& name);

        /// Overrides
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Add base
        /// \param base
        void AddBase(Class* base) {
            bases.push_back(base);
        }

        /// Get bases
        /// \return
        const PtrVector<Class>& GetBases() const {
            return bases;
        }

        /// Get name
        /// \return
        const String& GetName() const {
            return name;
        }

    private:
        // Name
        String name;

        // All bases
        PtrVector<Class> bases;
    };
}
