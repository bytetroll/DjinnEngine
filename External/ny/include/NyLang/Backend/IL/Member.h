//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//>> Copyright (C) 2016 Miguel N. Petersen.  All rights reserved.
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#pragma once

#include <NyLang/Core/Shared.h>
#include <NyLang/Core/StdTypes.h>
#include <NyLang/Core/COM/ID.h>
#include <NyLang/Backend/IL/IValue.h>
#include <NyLang/Core/BitSet.h>
#include "PrettyPrintContext.h"

namespace NyLang::IL {
    class IType;

    enum class MemberStorageFlag {
        eNone = 0,
        eExtern = 1,
    }; LANG_ENUM_BITSET(MemberStorageFlag);

    class Member : public IValue {
    public:
        LANG_CLASSID("Member");

        Member(const String& name, IType* type, const MemberStorageFlagSet& flags, int offset, int index);

        /// Overrides
        virtual void PrettyPrint(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual void PrettyPrintReference(PrettyStream &stream, PrettyPrintContext &context) override;
        virtual ClassID GetClassID() override;

        /// Get return type
        /// \return
        const auto& GetType() const {
            return type;
        }

        /// Get offset
        /// \return
        int GetOffset() const {
            return offset;
        }

        /// Get name
        /// \return
        const String& GetName() const {
            return name;
        }

        /// Get flags
        /// \return
        const MemberStorageFlagSet& GetFlags() const {
            return flags;
        }

        /// Get index
        /// \return
        int GetIndex() const {
            return index;
        }

    private:
        // Name
        String name;

        // Contained type
        IType* type = nullptr;

        // Flags
        MemberStorageFlagSet flags;

        // Offset of this member
        int offset = 0;

        // Index of this member
        int index = 0;
    };
}
