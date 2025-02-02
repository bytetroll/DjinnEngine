//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#pragma once

#include <NyLang/Core/Shared.h>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/PrettyStream.h>
#include <NyLang/Core/Context.h>
#include "ExecutionBlock.h"
#include "IValue.h"
#include "Member.h"

namespace NyLang::IL {
    class Member;
    class Function;
    class IType;

    class Container : public IValue {
    public:
        using FunctionMap = UnorderedMultimap<String, Function*>;
        using FunctionIt = FunctionMap::iterator;

        LANG_CLASSID("Container");

        Container(Context& context);

        /// Pretty print
        /// \param stream
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context);

        /// Overrides
        virtual ClassID GetClassID() override;

        /// Add member
        /// \param member
        Member* AddMember(const String &name, IType*type, const MemberStorageFlagSet &flags);

        /// Get members
        /// \return
        PtrVector<Member> &GetMembers() {
            return members;
        }

        /// Add function
        /// \param function
        void AddFunction(Function*function);

        /// Get functions
        /// \return
        PtrVector<Function> &GetFunctions() {
            return functions;
        }

        /// Get functions with name
        /// \param name
        /// \return
        std::pair<FunctionIt, FunctionIt> GetFunctionsWithName(const String &name) {
            return mappedFunctions.equal_range(name);
        }

        /// Get initialization execution block
        /// \return
        ExecutionBlock*GetInitializationExecutionBlock() {
            return initializationBlock;
        }

    private:
        Context& context;

        // Initialization execution block
        ExecutionBlock* initializationBlock = nullptr;

        // All members
        PtrVector<Member> members;

        // All functions
        PtrVector<Function> functions;

        // Mapped functions
        UnorderedMultimap<String, Function*> mappedFunctions;
    };
}
