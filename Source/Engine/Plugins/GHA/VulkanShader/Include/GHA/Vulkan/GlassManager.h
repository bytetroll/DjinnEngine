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
 on 11/30/2017.
//
#pragma once

#include <lunarglass/Core/PrivateManager.h>
#include <COM/IUnknown.h>
#include <GHA/Stage.h>
#include <Core/String.h>
#include <Data/ShaderCompilerNILDesc.h>

namespace llvm {
    class BasicBlock;
}

namespace Djinn::GHA::Vulkan {
    class GlassManager : public gla::PrivateManager {
    public:
        /// Construct
        /// \param stage
        GlassManager(Host::IRegistry *registry, const Data::ShaderCompileNILDesc& desc);
        virtual ~GlassManager();

        /// Overrides
        virtual void createContext() override;
        virtual void clear() override;

        /// Compile the current kernel to glsl
        /// \param glsl
        /// \return
        COM::Result Compile(Core::String &out);

    private:
        void PatchTextureMapInlining(llvm::BasicBlock* block);

        Host::IRegistry *registry;
    };
}
