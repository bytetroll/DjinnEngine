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
 on 12/9/2017.
//

#include <GHA/Vulkan/UnorderedVertexBufferView.h>
#include <GHA/Vulkan/VertexBuffer.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>

DJINN_NS3(GHA, Vulkan);

UnorderedVertexBufferView::UnorderedVertexBufferView(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IResource(registry, outer, host) {

}

COM::Result UnorderedVertexBufferView::Initialize(vk::Device, IVertexBuffer* vertexBuffer) {
    this->vertexBuffer = vertexBuffer;
    auto vkVertexBuffer = static_cast<VertexBuffer*>(vertexBuffer);

    // Describe descriptor
    descriptor.offset = 0;
    descriptor.range = vkVertexBuffer->GetSize();
    descriptor.buffer = vkVertexBuffer->GetBuffer();

    // OK
    return COM::kOK;
}

IVertexBuffer *UnorderedVertexBufferView::GetVertexBuffer() {
    return vertexBuffer;
}

COM::InterfaceID UnorderedVertexBufferView::GetIID() {
    return kIID;
}

COM::Result UnorderedVertexBufferView::Released() {
    // OK
    return COM::kOK;
}
