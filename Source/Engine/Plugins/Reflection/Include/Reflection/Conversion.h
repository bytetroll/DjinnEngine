//
// Created by Dev on 9/12/2018.
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