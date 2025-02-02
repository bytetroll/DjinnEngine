//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Backend/IL/IType.h>
#include <NyLang/Backend/IL/PrettyPrintContext.h>

namespace NyLang::IL {
    class FunctionType : public IType {
    public:
        LANG_CLASSID("FunctionType");

        /// Construct
        /// \param returnType
        /// \param argumentTypes
        FunctionType(IType* returnType, const PtrVector<IType>& argumentTypes);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get return type
        /// \return
        IType* GetReturnType() {
            return returnType;
        }

        /// Get argument types
        /// \return
        const PtrVector<IType>& GetArgumentTypes() {
            return argumentTypes;
        }

    private:
        // Return type
        IType* returnType = nullptr;

        // Argument types
        PtrVector<IType> argumentTypes;
    };
}
