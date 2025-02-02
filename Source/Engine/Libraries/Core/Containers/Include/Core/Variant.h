//
// Created by Dev on 9/10/2018.
//
#pragma once

#include <Core/Template/Common.h>
#include <Core/Assert.h>

namespace Djinn::Core {
    /// Basic variant type
    /// \tparam TX - Must all be copy and move constructable
    template<typename... TX>
    struct Variant {
        /// Verify that all types are both copy and move constructable
        static_assert((... && std::is_copy_constructible_v<TX>) && (... && std::is_move_constructible_v<TX>), "Variant types must be both copy and move constructable");

        /// Get index of type
        template<typename U>
        static constexpr int Index = IndexOf<U, TX...>;

        /// Zero constructor
        Variant() {}

        /// Value constructor
        template<typename T>
        Variant(const T &value) {
            Set(value);
        }

        /// Copy constructor
        Variant(const Variant &other) : typeIndex(other.typeIndex), stub(other.stub) {
            if (typeIndex != -1) {
                stub.copy(&other.storage, &storage);
            }
        }

        /// Move constructor
        /*Variant(Variant &&other) : typeIndex(other.typeIndex), stub(other.stub) {
            if (typeIndex != -1) {
                move(&other.storage, &storage);
            }

            other.typeIndex = -1;
        }*/

        /// Destructor
        ~Variant() {
            Unset();
        }

        /// Set the stored value
        /// \param value
        /// \return
        template<typename T, typename U = RemoveCR<T>>
        U &Set(T &&value) {
            Unset();
            typeIndex = IndexOf<U, TX...>;
            stub.destroy = [](void *obj) { static_cast<U *>(obj)->~U(); };
            stub.copy = [](const void *src, void *dst) { new(static_cast<U *>(dst)) U(*static_cast<const U *>(src)); };
            //stub.move = [](void *src, void *dst) { new(static_cast<U *>(dst)) U(std::move(*static_cast<U *>(src))); };
            return *new(&storage) U(value);
        }

        /// Unset the current stored value
        void Unset() {
            if (typeIndex != -1) {
                stub.destroy(&storage);
                typeIndex = -1;
            }
        }

        /// Get a reference to the current value, throws on invalid casts
        /// \return
        template<typename T>
        T &Get() {
            DJINN_ASSERT(IsOf<T>(), "Attempting to get an invalid type of given variant");
            void* ptr = &storage;
            return *reinterpret_cast<T *>(ptr);
        }

        /// Get a reference to the current value, throws on invalid casts
        /// \return
        template<typename T>
        const T &Get() const {
            DJINN_ASSERT(IsOf<T>(), "Attempting to get an invalid type of given variant");
            const void* ptr = &storage;
            return *reinterpret_cast<const T *>(ptr);
        }

        /// Copy assignment
        /// \param other
        /// \return
        Variant &operator=(const Variant& other) {
            Unset();
            typeIndex = other.typeIndex;
            if (typeIndex != -1) {
                stub = other.stub;
                stub.copy(&other.storage, &storage);
            }
            return *this;
        }

        /// Move assignment
        /// \param other
        /// \return
        /*template<typename T>
        Variant &operator=(Variant&& other) {
            typeIndex = other.typeIndex;
            stub = other.stub;
            stub.move(&other.storage, &storage);
            other.typeIndex = -1;
            return *this;
        }*/

        /// Assign value
        /// \param value
        /// \return
        template<typename T>
        Variant &operator=(const T &value) {
            Set(value);
            return *this;
        }

        /// Check if this variant is storing a value of type T
        /// \return
        template<typename T>
        bool IsOf() const {
            return IndexOf<RemoveCR<T>, TX...> == typeIndex;
        }

        /// Get the current type index
        /// \return
        int GetTypeIndex() const {
            return typeIndex;
        }

    private:
        /// Index of the stored type
        int typeIndex = -1;

        /// Buffer
        std::aligned_storage_t<MaxSize<sizeof(TX)...>, MaxSize<alignof(TX)...>> storage;

        /// Stub operations
        struct {
            void (*destroy)(void *obj) = nullptr;
            void (*copy)(const void *src, void *dst) = nullptr;
            //void (*move)(void *src, void *dst) = nullptr;
        } stub;
    };
}