//
// Created by Dev on 12/26/2017.
//
#pragma once

#include "PassBuilder.h"

namespace Djinn::Graphics {
    class IPass;

    class IPipeline : public COM::IUnknown {
    public:
        DJINN_COM_INTERFACE();

        IPipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IUnknown(registry, outer, host) {

        }

        /// Register a pass
        /// \param pass
        /// \return
        virtual COM::Result Register(IPass* pass) = 0;

        /// Deregister a pass
        /// \param pass
        /// \return
        virtual COM::Result Deregister(IPass* pass) = 0;

        /// Get pass by class id
        /// \param cid
        /// \return
        virtual COM::Result GetPass(const COM::ClassID& cid, IPass** out) = 0;
        /// Get pass by interface id

        /// \param iid
        /// \return
        virtual COM::Result GetPass(const COM::InterfaceID& iid, IPass** out) = 0;

        /// Build all commands
        /// \param builder
        /// \return
        virtual COM::Result Build(PassBuilder builder, double delta) = 0;

        /// Add a dependency to a locally hosted pass
        /// \param dependent
        /// \param target
        /// \return
        virtual COM::Result AddDependency(IPass* dependent, IPass* target) = 0;

        /// Get pass helper
        /// \param out
        /// \return
        template<typename T>
        COM::Result GetPass(T** out) {
            if constexpr(COM::HasKCID<T>::kValue) {
                return GetPass(T::kCID, reinterpret_cast<IPass**>(out));
            } else {
                return GetPass(T::kIID, reinterpret_cast<IPass**>(out));
            }
        }
    };
}