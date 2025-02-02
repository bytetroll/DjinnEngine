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
 on 11/23/2017.
//
#pragma once

#include <vulkan/vulkan.ext.hpp>
#include <COM/IUnknown.h>
#include <GHA/TextureBufferViewDesc.h>

namespace Djinn::GHA {
    class ITextureBuffer;
}

namespace Djinn::GHA::Vulkan {
    class DJINN_EXPORT_PLUGINS_GHA_VULKAN TextureBufferView : public IResource {
    public:
        DJINN_COM_CLASS();

        /// Construct
        /// \param registry
        /// \param outer
        TextureBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost *host);

        /// Initialize
        /// \return
        COM::Result Initialize(const TextureBufferViewDesc &desc, vk::Device device, ITextureBuffer *TextureBuffer);

        /// Overrides
        ITextureBuffer *GetTextureBuffer();
        const TextureBufferViewDesc &GetDesc();
        virtual COM::InterfaceID GetIID() override {
            return kIID;
        }

        /// Get view
        /// \return
        const vk::BufferView &GetView() const {
            return view;
        }

    protected:
        /// Overrides
        COM::Result Released() override;

    private:
        ITextureBuffer *textureBuffer;
        TextureBufferViewDesc desc;
        vk::Device device;
        vk::BufferView view;
    };
}