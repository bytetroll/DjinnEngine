//
// Created by Dev on 6/1/2018.
//
#pragma once

#include <COM/ID.h>

namespace Djinn::Reflection {
    struct ClassID {
    public:
        ClassID(std::nullptr_t * = nullptr) : set(false) {

        }

        ClassID(const COM::ClassID& cid) : ClassID(cid.Get()) {

        }

        ClassID(const COM::InterfaceID & iid) : ClassID(iid.Get()) {

        }

        explicit ClassID(USize uid) : uid(uid), set(true) {

        }

        USize Get() const {
            return uid;
        }

        bool IsSet() const {
            return set;
        }

        COM::ClassID AsCOMClassID() const {
            return COM::ClassID(uid);
        }

        COM::InterfaceID AsCOMInterfaceID() const {
            return COM::InterfaceID(uid);
        }

        // Bool operator
        operator bool() const {
            return set;
        }

        // Comparators
        bool operator==(const ClassID &other) const { return uid == other.uid; }
        bool operator!=(const ClassID &other) const { return uid != other.uid; }
        bool operator>=(const ClassID &other) const { return uid >= other.uid; }
        bool operator<=(const ClassID &other) const { return uid <= other.uid; }
        bool operator>(const ClassID &other) const { return uid > other.uid; }
        bool operator<(const ClassID &other) const { return uid < other.uid; }

    private:
        USize uid = 0;

        // Is set
        bool set;
    };

    /// Get id of typename
    /// \tparam T
    /// \return
    template<typename T>
    static ClassID ToID() {
        return ClassID(GetTypeNameCRC64<T>());
    }

    /// Get id from string
    /// \return
    static ClassID ToID(const char* str) {
        return ClassID(Core::ComputeCRC64(str));
    }

    /// Get id from class id
    /// \return
    static ClassID ToID(COM::ClassID cid) {
        return ClassID(cid.Get());
    }

    /// Get id from interface id
    /// \return
    static ClassID ToID(COM::InterfaceID iid) {
        return ClassID(iid.Get());
    }
}

namespace Djinn {
    /// Common hashes
    template<>
    struct Hasher<Reflection::ClassID> {
        USize operator()(const Reflection::ClassID &value) const { return Hasher<UInt64>{}(value.Get()); }
    };
}