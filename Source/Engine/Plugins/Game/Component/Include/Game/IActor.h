//
// Created by Dev on 8/15/2018.
//
#pragma once

#include "IActorSystemInstance.h"
#include <Core/StringView.h>
#include <Core/ArrayView.h>
#include <Core/Array.h>

namespace Djinn::Game {
    class IActorComponent;
    class IActorSystem;

    class DJINN_EXPORT_PLUGINS_GAME_COMPONENT IActor : public IActorSystemInstance {
    public:
        DJINN_COM_INTERFACE();

        IActor(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Destroy this actor
        /// Destruction may be deferred to the end of the frame
        void Destroy();

        /// Register a component
        /// \param component
        void Register(IActorComponent *component);

        /// Set the name of this actor
        /// \param value
        void SetName(const Core::StringView& value);

        /// Get the name of this actor
        /// \return
        Core::StringView GetName() const {
            return name;
        }

        /// Create a component with class id
        /// \param cid
        /// \return
        IActorComponent *CreateComponent(const COM::ClassID &cid);

        /// Create the first component with a class id
        /// \param cid
        /// \return
        IActorComponent *GetFirstComponent(const COM::ClassID &cid);

        /// Create the first component with an interface id
        /// \param cid
        /// \return
        IActorComponent *GetFirstComponent(const COM::InterfaceID &iid);

        /// Remove a component
        /// \param component
        void RemoveComponent(IActorComponent *component);

        /// Create a component of type
        /// \return
        template<typename T>
        T *CreateComponent() {
            return static_cast<T *>(CreateComponent(T::kCID));
        }

        /// Create the first component of a type (class or interface)
        /// \return
        template<typename T>
        T *GetFirstComponent() {
            if constexpr(COM::HasKCID<T>::kValue) {
                return static_cast<T *>(GetFirstComponent(T::kCID));
            } else {
                return static_cast<T *>(GetFirstComponent(T::kIID));
            }
        }

        /// Get or create the first component of a type
        /// \tparam T
        /// \return
        template<typename T>
        T* GetOrCreateFirstComponent() {
            auto ptr = GetFirstComponent<T>();
            if (!ptr) {
                ptr = CreateComponent<T>();
            }
            return ptr;
        }

        /// Get all components
        /// \return
        Core::ArrayView<Core::WeakPtr<IActorComponent>> GetComponents() {
            return components;
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IUnknown::kIID:
                    *out = static_cast<IUnknown*>(this);
                    return COM::kOK;
                case IActorSystemInstance::kIID:
                    *out = static_cast<IActorSystemInstance*>(this);
                    return COM::kOK;
                case IActor::kIID:
                    *out = static_cast<IActor*>(this);
                    return COM::kOK;
            }
            return COM::kNotImplemented;
        }

    public:
        /// Invoked before actor creation for initialization of static sub objects
        virtual void StaticInitialize() {}

        /// Invoked during actor creation
        virtual void Created() {};

        /// Invoked during actor destruction
        virtual void Destroyed() {};

        /// Invoked when this actor has been enabled
        virtual void Enabled() {};

        /// Invoked when this actor has been disabled
        virtual void Disabled() {};

    protected:
        /// Overrides
        COM::Result Released() override;

    public:
        /// Enable or disable cross world actor referencing
        /// Communication across worlds can be dangerous as they may operate asynchronously or out-of-order
        /// \param enabled
        void SetCrossWorldReferencing(bool enabled) {
            allowCrossWorldReferencing = enabled;
        }

        /// Can this actor communicate across worlds?
        /// \return
        bool GetCrossWorldReferencing() const {
            return allowCrossWorldReferencing;
        }

        /// Check if this actor can safely reference another
        /// \param other
        /// \return
        bool MayReference(const IActor* other) const {
            // Only the remote actor needs to be marked as cross world referencing
            return (world == other->world) || other->allowCrossWorldReferencing;
        }

        /// Ensure this instance can safely reference another
        /// \param other
        /// \return
        void AssertReference(const IActor* other) const {
            DJINN_ASSERT(MayReference(other), "Attempting to reference a cross-world actor system instance without permission");
        }

    protected:
        /// Name of this actor
        Core::String name;

        /// Is this actor permitted to perform cross-world operations?
        bool allowCrossWorldReferencing = false;

        /// All components
        Core::Array<Core::WeakPtr<IActorComponent>> components;
    };
}