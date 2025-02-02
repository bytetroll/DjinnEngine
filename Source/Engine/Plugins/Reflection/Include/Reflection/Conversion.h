//
// Copyright 2025 Miguel Peterson.  All rights reserved.
// Copyright 2025 Nathan Young.  All rights reserved.
// Copyright 2025 Stefen Wendling.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the “Software”), to deal in the Software without restriction, including without
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 on 9/12/2018.
//
#pragma once

#include <Reflection/ID.h>
#include <Core/StringView.h>

namespace Djinn::Host {
    class IRegistry;
}

namespace Djinn::Reflection {
    /// Convert name hash to enum value
    /// \param registry
    /// \param cid
    /// \param hash
    /// \param out
    /// \return
    DJINN_EXPORT_PLUGINS_REFLECTION bool ToEnum(Host::IRegistry *registry, ClassID cid, USize hash, Int64 &out);

    /// Convert name to enum value
    /// \param registry
    /// \param cid
    /// \param name
    /// \param out
    /// \return
    static bool ToEnum(Host::IRegistry *registry, ClassID cid, const Core::StringView &name, Int64 &out) {
        return ToEnum(registry, cid, name.Hash(), out);
    }

    /// Convert hash to enum value of type
    /// \tparam T
    /// \param registry
    /// \param hash
    /// \param out
    /// \return
    template<typename T>
    static bool ToEnum(Host::IRegistry *registry, USize hash, T &out) {
        Int64 value;
        if (ToEnum(registry, ToID<T>(), hash, value)) {
            out = static_cast<T>(value);
            return true;
        }
        return false;
    }

    /// Convert name to enum value of type
    /// \tparam T
    /// \param registry
    /// \param hash
    /// \param out
    /// \return
    template<typename T>
    static bool ToEnum(Host::IRegistry *registry, const Core::StringView &name, T &out) {
        Int64 value;
        if (ToEnum(registry, ToID<T>(), name.Hash(), value)) {
            out = static_cast<T>(value);
            return true;
        }
        return false;
    }
}