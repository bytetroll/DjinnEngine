//
// Created by Dev on 12/21/2017.
//
#pragma once

#include <GHA/IConfiguration.h>

namespace Djinn::GHA::Vulkan {
    class Configuration : public IConfiguration {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        Configuration(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Overrides
        virtual Core::String GetDisplayName() override;
        virtual COM::ClassID GetInstanceCID() override;
        virtual COM::Result GetSupportedSchedulerCIDs(UInt32 &count, COM::ClassID *out) override;
        virtual COM::Result GetSupportedInterpreterCIDs(const COM::ClassID &scheduler, UInt32 &count, COM::ClassID *out) override;

    protected:
        /// Overrides
        COM::Result Released() override;
    };
}