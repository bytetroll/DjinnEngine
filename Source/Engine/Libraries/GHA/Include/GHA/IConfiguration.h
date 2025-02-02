//
// Created by Dev on 12/21/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/String.h>

namespace Djinn::GHA {
    class IConfiguration : public COM::TUnknown<IConfiguration> {
    public:
        DJINN_COM_INTERFACE();

        IConfiguration(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Get display / decorative name
        /// \return
        virtual Core::String GetDisplayName() = 0;

        /// Get instance classid
        /// \return
        virtual COM::ClassID GetInstanceCID() = 0;

        /// Get supported scheduler classid's
        /// \param count
        /// \param out - Sorted in performance wise preference
        /// \return
        virtual COM::Result GetSupportedSchedulerCIDs(UInt32& count, COM::ClassID* out) = 0;

        /// Get supported interpreter classid's from given scheduler classid
        /// \param scheduler
        /// \param count
        /// \param out - Sorted in performance wise preference
        /// \return
        virtual COM::Result GetSupportedInterpreterCIDs(const COM::ClassID& scheduler, UInt32& count, COM::ClassID* out) = 0;
    };
}