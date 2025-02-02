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
 on 1/17/2018.
//
#pragma once

#include "EntityID.h"
#include <Core/Assert.h>
#include <Core/Template/Common.h>
#include <COM/ID.h>

namespace Djinn::Game {
    class IEntityComponentState;

    /// Detail
    namespace Detail {
        DJINN_EXPORT_PLUGINS_GAME_COMPONENT extern IEntityComponentState *GetEntityComponentPtr(const EntityComponentReference &ref);
        DJINN_EXPORT_PLUGINS_GAME_COMPONENT extern bool DestroyEntityComponent(const EntityComponentReference &ref);
    }

    /// Forward
    template<typename T> struct EntityComponent;

    /// Opaque component, represents a typeless component
    struct OpaqueEntityComponent {
        /// Construct empty
        OpaqueEntityComponent(std::nullptr_t = nullptr) {

        }

        /// Construct
        /// \param id
        /// \param system
        OpaqueEntityComponent(const EntityComponentReference &ref, Int64 *externCommit, COM::ClassID cid) : ref(ref), externCommit(externCommit), componentClassID(cid) {

        }

        /// Get component pointer
        /// Note that this should not be cached since it may be invalidated
        /// \return
        IEntityComponentState *Get() const {
            if (NeedsCommit()) {
                cached = Detail::GetEntityComponentPtr(ref);

                // Invalid pointer signifies deleted component
                if (!cached) {
                    ref = {};
                }
            }
            return cached;
        }

        /// Destroy this component
        /// \return
        bool Destroy() {
            return Detail::DestroyEntityComponent(ref);
        }

        /// Compare component
        /// \param other
        /// \return
        bool operator==(const OpaqueEntityComponent &other) {
            return ref == other.ref;
        }

        /// Compare component
        /// \param other
        /// \return
        bool operator!=(const OpaqueEntityComponent &other) {
            return ref != other.ref;
        }

        /// Validate this component
        /// \return
        operator bool() const {
            return Valid();
        }

        /// Validate this component
        /// \return
        bool Valid() const {
            return ref.component != nullptr && Get() != nullptr;
        }

        /// Check if this component is of a type
        /// \tparam T
        /// \return
        template<typename U>
        bool Is() {
            return componentClassID == U::kCID;
        }

        /// Check if this component is of a type
        /// \return
        bool Is(COM::ClassID cid) {
            return componentClassID == cid;
        }

        /// Get typed component representation
        template<typename T>
        EntityComponent<T> As() const {
            return EntityComponent<T>(*this);
        }

        /// Get the reference
        /// \return
        const EntityComponentReference &GetReference() const {
            return ref;
        }

        /// Get external commit index
        /// \return
        Int64 *GetExternCommit() const {
            return externCommit;
        }

        /// Get cached
        /// \return
        IEntityComponentState *GetCached() const {
            return cached;
        }

        /// Get the class id of this opaque component
        /// \return
        COM::ClassID GetComponentClassID() const {
            return componentClassID;
        }

        /// Access component
        /// \return
        IEntityComponentState* operator->() {
            return Get();
        }

        /// Access component
        /// \return
        IEntityComponentState* operator->() const {
            return Get();
        }

    private:
        bool NeedsCommit() const {
            auto index = *externCommit;
            if (index != currentCommit) {
                currentCommit = index;
                return true;
            }
            return cached == nullptr;
        }

    private:
        mutable EntityComponentReference ref;
        mutable IEntityComponentState *cached = nullptr;
        Int64 *externCommit = nullptr;
        mutable Int64 currentCommit = -1;
        COM::ClassID componentClassID;
    };

    /// Component handle
    /// \tparam T
    template<typename T>
    struct EntityComponent {
    public:
        /// Construct empty
        EntityComponent(std::nullptr_t = nullptr) {

        }

        /// Construct
        /// \param id
        /// \param system
        EntityComponent(const EntityComponentReference &ref, Int64 *externCommit) : ref(ref), externCommit(externCommit) {

        }

        /// Construct from opaque component
        /// \param other
        EntityComponent(const OpaqueEntityComponent &other) : ref(other.GetReference()), externCommit(other.GetExternCommit()), cached(other.GetCached()) {
            if (Valid()) {
                DJINN_ASSERT(T::kCID == other.GetComponentClassID(), "Invalid component cast, component types must match");
            }
        }

        /// Get opaque representation
        OpaqueEntityComponent AsOpaque() const {
            return OpaqueEntityComponent(ref, externCommit, T::kCID);
        }

        /// Implicit cast to opaque
        operator OpaqueEntityComponent() const {
            return AsOpaque();
        }

        /// Get component pointer
        /// Note that this should not be cached since it may be invalidated
        /// \return
        template<typename U = T>
        typename U::ComponentState *Get() const {
            if (NeedsCommit()) {
                cached = Detail::GetEntityComponentPtr(ref);

                // Invalid pointer signifies deleted component
                if (!cached) {
                    ref = {};
                }
            }
            return static_cast<typename U::ComponentState*>(cached);
        }

        /// Destroy this component
        /// \return
        bool Destroy() const {
            return Detail::DestroyEntityComponent(ref);
        }

        /// Access component
        /// \return
        template<typename U = T>
        typename U::ComponentState* operator->() {
            return Get();
        }

        /// Access component
        /// \return
        template<typename U = T>
        typename U::ComponentState* operator->() const {
            return Get();
        }

        /// Compare component
        /// \param other
        /// \return
        bool operator==(const EntityComponent &other) {
            return ref == other.ref;
        }

        /// Compare component
        /// \param other
        /// \return
        bool operator!=(const EntityComponent &other) {
            return ref != other.ref;
        }

        /// Validate this component
        /// \return
        operator bool() const {
            return Valid();
        }

        /// Validate this component
        /// \return
        bool Valid() const {
            return ref.component != nullptr && Get() != nullptr;
        }

        /// Get the reference
        /// \return
        const EntityComponentReference &GetReference() const {
            return ref;
        }

        /// Get external commit index
        /// \return
        Int64 *GetExternCommit() const {
            return externCommit;
        }

        /// Get cached
        /// \return
        template<typename U = T>
        typename U::ComponentState *GetCached() const {
            return static_cast<typename U::ComponentState*>(cached);
        }

    private:
        bool NeedsCommit() const {
            if (!externCommit) {
                return false;
            }

            auto index = *externCommit;
            if (index != currentCommit) {
                currentCommit = index;
                return true;
            }
            return cached == nullptr;
        }

    private:
        mutable EntityComponentReference ref;
        mutable IEntityComponentState *cached = nullptr;
        Int64 *externCommit = nullptr;
        mutable Int64 currentCommit = -1;
    };
}