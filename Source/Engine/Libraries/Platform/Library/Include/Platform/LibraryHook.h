//
// Created by Dev on 5/13/2018.
//
#pragma once

namespace Djinn::Platform {
    template<typename T>
    class LibraryHook {
    public:
        using Type = typename T::Type;

        LibraryHook(void** user, Type* ptr) : user(user), ptr(ptr) {

        }

        /// Is this hook valid?
        /// \return
        bool Valid() const {
            return ptr != nullptr;
        }

        /// Is this hook valid?
        /// \return
        operator bool() const {
            return ptr != nullptr;
        }

        /// Assign a new hook
        /// \param hook
        void Assign(void* hookUser, Type hook) const {
            if (user) {
                *user = hookUser;
            }
            *ptr = hook;
        }

        /// Get current hook
        /// \return
        Type Get() const {
            return *ptr;
        }

    private:
        void** user;
        Type* ptr;
    };
}