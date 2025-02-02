//
// Created by Dev on 10/22/2017.
//
#pragma once

#include <Core/Unique.h>
#include <Core/StringView.h>

namespace Djinn::Base {
    /// ID Aliases
    DJINN_UNIQUEID(PathID, UInt32);

    /// To Path
    /// \param path
    /// \return
    static PathID ToPath(const char* path) {
        return Base::PathID(Core::ComputeCRC32(path), path);
    }

    /// To Path
    /// \param path
    /// \return
    static PathID ToPath(const Core::StringView& path) {
        return Base::PathID(Core::ComputeCRC32(path.Ptr()), path.End());
    }
}

namespace Djinn {
    /// Literals
    constexpr Base::PathID operator "" _path(const char *str, size_t) { return Base::PathID(Core::ComputeCRC32(str), str); }

    /// Common hashes
    template<> struct Hasher<Base::PathID> { USize operator()(const Base::PathID& value) const { return Hasher<Int64>{}(value.Get()); }};
}
