#pragma once

#include <NyLang/Core/Unique.h>
#include <NyLang/Core/Hash.h>

namespace NyLang {
    // ID Aliases
    LANG_UNIQUEID(ClassID, std::uint64_t);
    LANG_UNIQUEID(InterfaceID, std::uint64_t);
    LANG_UNIQUEID(ActionID, std::uint64_t);

    // ID Declare Helpers
#define LANG_CLASSID(STR_NAME) \
    __attribute__ ((dllexport)) static constexpr NyLang::ClassID kCID = NyLang::ClassID(STR_NAME##_id)
#define LANG_ACTIONID(NAME, STR_NAME) \
    __attribute__ ((dllexport)) static constexpr NyLang::ActionID k##NAME = NyLang::ActionID(STR_NAME##_id)
#define LANG_INTERFACEID(STR_NAME) \
    __attribute__ ((dllexport)) static constexpr NyLang::InterfaceID kIID = NyLang::InterfaceID(STR_NAME##_id)
}