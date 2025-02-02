#pragma once

#include <COM/IUnknown.h>
#include <Core/String.h>
#include "Format.h"
#include "FormatCapability.h"

namespace Djinn {
    namespace GHA {
        class IOutput;

        class ICapabilities : public COM::TUnknown<ICapabilities> {
        public:
            DJINN_COM_INTERFACE();

            ICapabilities(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Check if a format is supported
            /// \param format
            /// \return
            virtual bool IsFormatSupported(GHA::Format format, const GHA::FormatCapabilitySet& capabilities) = 0;

            /// Select first supported or optimal format
            /// \param count - Number of formats
            /// \param formats - Format pointer
            /// \param capabilities - Required capabilities
            /// \param out - Resulting format
            /// \return
            virtual bool SelectFormat(UInt32 count, GHA::Format* formats, const GHA::FormatCapabilitySet& capabilities, GHA::Format& out) = 0;

            /// Get format size
            /// \param format
            /// \return
            virtual USize GetFormatSize(GHA::Format format) = 0;
        };
    }
}