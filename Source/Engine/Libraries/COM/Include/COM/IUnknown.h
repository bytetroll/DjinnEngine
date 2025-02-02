#pragma once

#include "ID.h"
#include "Result.h"
#include "IInstanceHost.h"
#include <Core/Array.h>
#include <Core/WeakPtr.h>

namespace Djinn {
    namespace Host {
        class IRegistry;
    }

    namespace COM {
        /// Developer helpers
#define DJINN_COM_REFERENCECOLLECTORS() \
        auto GetWeakPtr() {\
            return Core::WeakPtr<DJINN_QUALIFIED_THIS>(this->IUnknown::GetReferenceController(), this);\
        }\
        auto PromoteSharedPtr() {\
            this->IUnknown::PromoteShared(); \
            return Core::SharedPtr<DJINN_QUALIFIED_THIS>(this->IUnknown::GetReferenceController(), this);\
        }                               \

        /// Interface helper
#define DJINN_COM_INTERFACE() \
        DJINN_COM_REFERENCECOLLECTORS() \
        DJINN_COM_INTERFACEID()

        /// Class helper
#define DJINN_COM_CLASS() \
        const char* GetClassName() override { \
            static const char* name = GetTypeName<DJINN_QUALIFIED_THIS>(); \
            return name; \
        } \
        COM::ClassID GetClassID() override { \
            return kCID;\
        }\
        DJINN_COM_REFERENCECOLLECTORS() \
        DJINN_COM_CLASSID()

        /// Base rtti type
        class IQueryable {
        public:
            /// Query for interface
            /// \param iid
            /// \param out
            /// \return
            virtual Result QueryInterface(const InterfaceID &iid, void **out) = 0;

            /// Query for interface
            /// \tparam ICLASS
            /// \param out
            /// \return
            template<class ICLASS>
            Result QueryInterface(ICLASS **out) {
                void *obj;
                if (CheckedResult result = QueryInterface(ICLASS::kIID, &obj)) {
                    return result;
                }
                *out = static_cast<ICLASS *>(obj);
                return COM::kOK;
            }

            /// Query for interface
            /// Shorthand, considered unsafe
            /// \tparam T
            /// \return
            template<class T>
            T *QueryInterface() {
                T *out;
                if (!QueryInterface < T > (&out)) {
                    out = nullptr;
                }
                return out;
            }

            /// Get front class id
            /// \return
            virtual ClassID GetClassID() = 0;
        };

        /// Component base type
        class IUnknown : public IQueryable {
        public:
            DJINN_COM_INTERFACE();

            /// Pass through
            /// \param registry
            /// \param outer
            IUnknown(Host::IRegistry *registry, IUnknown *outer, IInstanceHost* host) : registry(registry), outer(nullptr), host(host) {
                if (outer) {
                    outer->Bind(this);
                }
            }

            /// Deconstruct
            virtual ~IUnknown() {
                DJINN_ASSERT(deconstructed, "Components cannot be deleted manually, they must be deleted by calling Release()");
            }

            /// Release this component
            /// The component is invalidated after this
            /// \return
            COM::Result Release() {
                if (COM::CheckedResult result = DestructInternal()) {
                    return result;
                }

                // Delete component
                // Everything after this point cannot touch this class
                if (host) {
                    return host->DestroyInstance(this);
                }

                // ...
                return COM::kOK;
            }

            /// Bind a component to this component
            /// \param other
            void Bind(IUnknown *other) {
                DJINN_ASSERT(!isSharedComponent, "Shared components cannot be bound");

                // Clean up dangling registrations
                other->Detach();

                // Add as referenced
                registered.Add(other);
                other->outer = this;
            }

            /// Detach from outer component
            void Detach() {
                if (outer) {
                    outer->registered.RemoveValue(this);
                    outer = nullptr;
                }
            }

            /// Get the reference controller, will create one if it doesnt exist
            /// Must be manually released if requested
            /// \return
            Core::ReferenceController *GetReferenceController() {
                if (!referenceController) {
                    referenceController = new Core::ReferenceController(this);

                    // A single instance is always alive
                    referenceController->shared++;
                }
                return referenceController;
            }

            /// Helper
            template<class ICLASS>
            Result QueryInterface(ICLASS **out) {
                return IQueryable::QueryInterface(out);
            }

            /// Helper
            template<class T>
            T *QueryInterface() {
                return IQueryable::QueryInterface<T>();
            }

            /// Override
            virtual Result QueryInterface(const InterfaceID &iid, void **out) override {
                switch (iid) {
                    case IUnknown::kIID:
                        *out = this;
                        return kOK;
                }
                return kNotImplemented;
            }

            /// Set the worldHost user data
            /// \param data
            void SetHostUserdata(void* data) {
                hostUserdata = data;
            }

            /// Get name of this class
            /// \return
            virtual const char *GetClassName() = 0;

            /// Get registry
            /// \return
            Host::IRegistry *GetRegistry() const {
                return registry;
            }

            /// Get outer
            /// \return
            IUnknown *GetOuter() const {
                return outer;
            }

            /// Get the user data associated with the worldHost
            /// \return
            void* GetHostUserdata() const {
                return hostUserdata;
            }

        protected:
            /// Invoked during component destruction
            /// \return
            virtual COM::Result Released() { return COM::kOK; };

            /// Internal destruction
            /// \return
            COM::Result DestructInternal() {
                if (deconstructed) {
                    return COM::kOK;
                }

                // Mark as pending
                pendingDestruction = true;

                // Must not have remaining shared references
                if (referenceController) {
                    if (!isSharedComponent) {
                        referenceController->shared--;
                    }
                    DJINN_ASSERT(referenceController->shared == 0, "Attempting to release component with dangling shared references");
                }

                COM::Result result = COM::kOK;

                // Destroy this component
                // Invoked before destruction of children to keep in line with traditional C++ object management
                if (COM::CheckedResult checked = Released()) {
                    result = checked;
                }

                // Delete children
                while (!registered.Empty()) {
                    auto last = registered.PopEnd();

                    if (COM::CheckedResult result = last->Release()) {
                        return result;
                    }
                }

                // Remove dangling
                if (outer && !outer->pendingDestruction) {
                    outer->registered.RemoveValue(this);
                }

                // Yay!
                deconstructed = true;

                // ...
                return result;
            }

        protected:
            /// Promote to shared usage
            void PromoteShared() {
                // Ensure the controller exists
                GetReferenceController();

                // Shared have no outer components
                Detach();

                // Implicit instance is not managed here
                if (!isSharedComponent) {
                    referenceController->shared--;
                    isSharedComponent = true;
                }
            }

        protected:
            /// Master registry
            Host::IRegistry *registry;

            /// Master outer component
            IUnknown *outer;

            /// Host that spawned this component
            IInstanceHost* host;

            /// Host user data
            void* hostUserdata = nullptr;

            /// Developer controller
            Core::ReferenceController *referenceController = nullptr;

            /// Is this a shared instance?
            bool isSharedComponent = false;

            /// Owned components
            Core::Array<IUnknown *> registered;

            /// Is this component pending destruction?
            bool pendingDestruction = false;

            /// Has this component been properly deconstructed?
            bool deconstructed = false;
        };

        /// Component base type shorthand for interfaces
        /// \tparam T
        template<typename T>
        class TUnknown : public IUnknown {
        public:
            /// Construct
            /// \param registry
            /// \param outer
            TUnknown(Host::IRegistry *registry, IUnknown *outer, IInstanceHost* host) : IUnknown(registry, outer, host) {

            }

            /// Overrides
            virtual Result QueryInterface(const InterfaceID &iid, void **out) override {
                switch (iid) {
                    case IUnknown::kIID:
                        *out = static_cast<IUnknown*>(this);
                        return kOK;
                    case T::kIID:
                        *out = static_cast<T*>(this);
                        return kOK;
                }
                return kNotImplemented;
            }

            /// Helper
            template<class ICLASS>
            Result QueryInterface(ICLASS **out) {
                return IQueryable::QueryInterface(out);
            }

            /// Helper
            template<class U>
            U *QueryInterface() {
                return IQueryable::QueryInterface<U>();
            }

        private:
            /*template<typename T0, typename T1, typename... TX>
            bool __QueryInterface(const InterfaceID& iid) {
                return __QueryInterface<T0>(iid) || __QueryInterface<T1, TX...>(iid);
            }

            template<typename T0>
            bool __QueryInterface(const InterfaceID& iid) {
                return iid == T0::kIID;
            }*/
        };

        /// Cast an unknown component to a front end class type of an interface type
        /// \param com
        /// \return
        template<typename T>
        T *Cast(COM::IUnknown *com) {
            if (!com) {
                return nullptr;
            }
            if constexpr(HasKCID<T>::kValue) {
                if (com->GetClassID() == T::kCID) {
                    return static_cast<T *>(com);
                }
                return nullptr;
            } else {
                void *obj;
                if (!com->QueryInterface(T::kIID, &obj)) {
                    return nullptr;
                }

                return static_cast<T *>(obj);
            }
        };

        /// Cast an unknown component to a front end class type of an interface type, assert on invalid cast
        /// \param com
        /// \return
        template<typename T>
        T *CastChecked(COM::IUnknown *com) {
            auto casted = Cast<T>(com);
            DJINN_ASSERT(casted != nullptr, "Invalid cast from '", com->GetClassName(), "' to '", GetStaticTypeName<T>(), "'");
            return casted;
        }
    }
}