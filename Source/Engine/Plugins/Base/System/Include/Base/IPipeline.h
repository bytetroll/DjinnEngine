//
// Created by Dev on 5/1/2018.
//
#pragma once

#include <COM/IUnknown.h>
#include <Async/Group.h>
#include "PipelineConfigurationMode.h"
#include "ISystem.h"

namespace Djinn::Base {
    class IPipeline : public COM::TUnknown<IPipeline> {
    public:
        DJINN_COM_INTERFACE();

        IPipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Set the configuration mode
        /// \param mode
        virtual void SetConfigurationMode(PipelineConfigurationMode mode) = 0;

        /// Configure this pipeline
        /// \return
        virtual COM::Result Configure() = 0;

        /// Register a system
        /// \param system
        /// \return
        virtual COM::Result Register(ISystem *system) = 0;

        /// Deregister a system
        /// \param system
        /// \return
        virtual COM::Result Deregister(ISystem *system) = 0;

        /// Register a proxy mapping
        /// \param system
        /// \return
        virtual COM::Result RegisterProxy(const COM::ClassID &cid, ISystem *system) = 0;

        /// Run this pipeline async
        /// \param group
        /// \param delegate
        virtual void RunAsync(Async::Group &group, const Core::Delegate<void(ISystem *system)> &delegate) = 0;

        /// Add a dependency
        /// \param source
        /// \param dest
        /// \return
        virtual COM::Result AddDependency(ISystem *source, ISystem *dest) = 0;

        /// Get registered class
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result GetClass(const COM::ClassID &id, ISystem **out) = 0;

        /// Get registered interface
        /// \param id
        /// \param out
        /// \return
        virtual COM::Result GetInterface(const COM::InterfaceID &id, ISystem **out) = 0;

        /// Helper
        template<typename C>
        COM::Result GetClass(C **out) {
            ISystem* obj;
            if (COM::CheckedResult result = GetClass(C::kCID, &obj)) {
                return result;
            }

            *out = static_cast<C*>(obj);
            return COM::kOK;
        }

        /// Helper
        template<typename I>
        COM::Result GetInterface(I **out) {
            ISystem* obj;
            if (COM::CheckedResult result = GetInterface(I::kIID, &obj)) {
                return result;
            }
            return obj->QueryInterface<I>(out);
        }

        /// Helper
        template<typename T>
        T *Get() const {
            ISystem *dependency;
            if constexpr(COM::HasKCID<T>::kValue) {
                if (!GetClass(T::kCID, &dependency)) {
                    return nullptr;
                }
            } else {
                if (!GetInterface(T::kIID, &dependency)) {
                    return nullptr;
                }
            }
            return dependency;
        }

        /// Helper
        template<typename T, typename F>
        void RunAsyncAs(Async::Group &group, F &&action) {
            RunAsync(group, [action](ISystem *system) {
                if (auto ptr = COM::Cast<T>(system)) {
                    action(ptr);
                }
            });
        }
    };
}