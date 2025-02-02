#pragma once

#include <COM/IUnknown.h>
#include "SwapchainDesc.h"

namespace Djinn {
    namespace GHA {
        class ITexture;

        class ISwapchain : public COM::TUnknown<ISwapchain> {
        public:
            DJINN_COM_INTERFACE();

            ISwapchain(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

            }

            /// Get current backbuffer index
            /// \return
            virtual UInt32 GetCurrentIndex() = 0;

            /// Get backbuffer texture from index
            /// \param index
            /// \return
            virtual ITexture* GetTexture(int index) = 0;

            /// Get current backbuffer texture
            /// \return
            virtual ITexture* GetTexture() = 0;

            /// Get description
            /// \return
            virtual const SwapchainDesc& GetDesc() = 0;
        };
    }
}