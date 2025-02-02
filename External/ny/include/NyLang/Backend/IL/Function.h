//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Backend/IL/ExecutionBlock.h>
#include <NyLang/Backend/IL/IValue.h>
#include <NyLang/Core/BitSet.h>
#include <NyLang/Core/Context.h>
#include "PrettyPrintContext.h"

namespace NyLang::IL {
    class Argument;

    enum class FunctionStorageFlag {
        eExtern = 1,
        eStatic = (int)eExtern << 1,
        eConstructor = (int)eStatic << 1,
        eDeconstructor = (int)eConstructor << 1,
    }; LANG_ENUM_BITSET(FunctionStorageFlag);

    class Function : public IValue {
    public:
        LANG_CLASSID("Function");

        /// Construct
        /// \param returnType
        /// \param argumentTypes
        Function(Context& context, const String& name, const FunctionStorageFlagSet& flags, IType* returnType, const PtrVector<Argument>& arguments);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get primary execution block
        /// \return
        ExecutionBlock* GetExecutionBlock() {
            return block;
        }

        /// Get return type
        /// \return
        IType* GetReturnType() const {
            return returnType;
        }

        /// Get argument types
        /// \return
        const PtrVector<Argument>& GetArguments() const {
            return arguments;
        }

        /// Get name
        /// \return
        const String& GetName() const {
            return name;
        }

        /// Get flags
        /// \return
        const FunctionStorageFlagSet& GetFlags() const {
            return flags;
        }

    private:
        // Name
        String name;

        // Flags
        FunctionStorageFlagSet flags;

        // Execution block
        ExecutionBlock* block = nullptr;

        // Type
        IType* type = nullptr;

        // Return type
        IType* returnType = nullptr;

        // Arguments
        PtrVector<Argument> arguments;
    };
}
