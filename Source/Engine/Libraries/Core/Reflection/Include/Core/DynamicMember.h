//
// Created by Dev on 12/30/2017.
//
#pragma once

#include <Core/Size.h>
#include <Core/Memory.h>
#include "Member.h"

namespace Djinn::Core {
    struct MemberDummy {
        float f;
    };

    class DynamicMember {
        /// TODO: Test this
        static constexpr USize kMaxMemberSize = sizeof(Member<MemberDummy, int>);

    public:
        DynamicMember() = default;
        DynamicMember(const DynamicMember &) = default;

        /// Construct from member
        /// \tparam C
        /// \tparam T
        /// \param member
        template<typename C, typename T>
        DynamicMember(const Member<C, T> &member) : isValid(true) {
            static_assert(sizeof(member) <= kMaxMemberSize, "Unexpected member size");

            // Copy to buffer
            Core::Memory::Copy(buffer, &member, sizeof(member));

            // Prepare stubs
            stub.getter = Getter<C, T>;
            stub.setter = Setter<C, T>;
        }

        /// Get member value in instance
        /// \param instance
        /// \return
        void *GetPtr(void *instance = nullptr) const {
            return stub.getter(buffer, instance);
        }

        /// Get member value in instance
        /// \param instance
        /// \return
        template<typename T>
        T &Get(void *instance = nullptr) const {
            return *reinterpret_cast<T *>(stub.getter(buffer, instance));
        }

        /// Set member value in instance
        /// \param instance
        /// \param value
        /// \return
        template<typename T>
        T &Set(void *instance, T &value) const {
            return *reinterpret_cast<T *>(stub.setter(buffer, instance, &value));
        }

        /// Set member value in instance
        /// \param instance
        /// \param value
        /// \return
        template<typename T>
        T &Set(T &value) const {
            return *reinterpret_cast<T *>(stub.setter(buffer, nullptr, &value));
        }

        /// Is this dynamic member valid?
        /// \return
        bool IsValid() const {
            return isValid;
        }

    private:
        struct Stub {
            void *(*getter)(const void *, void *) = nullptr;
            void *(*setter)(const void *, void *, void *) = nullptr;
        } stub;

        /// Getter
        template<typename C, typename T>
        static void *Getter(const void *buffer, void *instance) {
            if (instance) {
                return &reinterpret_cast<const Member<C, T> *>(buffer)->Get(static_cast<C*>(instance));
            } else {
                return &reinterpret_cast<const Member<C, T> *>(buffer)->Get();
            }
        }

        /// Setter
        template<typename C, typename T>
        static void *Setter(const void *buffer, void *instance, void *value) {
            if (instance) {
                return &reinterpret_cast<const Member<C, T> *>(buffer)->Set(static_cast<C*>(instance), *static_cast<T*>(value));
            } else {
                return &reinterpret_cast<const Member<C, T> *>(buffer)->Set(*static_cast<T*>(value));
            }
        }

        char buffer[kMaxMemberSize] = {};
        bool isValid = false;
    };
}