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
 on 4/8/2018.
//
#pragma once

#include "IOutputPass.h"

namespace Djinn::Graphics {
    class IOutput;
    class IGraphicsSystem;

    class DJINN_EXPORT_PLUGINS_GRAPHICS_OUTPUT OutputPass : public IOutputPass {
    public:
        DJINN_COM_CLASS();

        OutputPass(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Initialize this worldHost
        /// \param pass
        /// \return
        COM::Result Initialize();

        /// Set the current output
        /// \param output
        /// \return
        COM::Result SetOutput(IOutput *output);

        /// Register output
        /// \param output
        /// \return
        COM::Result Register(IOutput* output);

        /// Deregister output
        /// \param output
        /// \return
        COM::Result Deregister(IOutput* output);

        /// Overrides
        virtual COM::Result Preprocess(PassBuilder builder) override;
        virtual COM::Result Build(PassBuilder builder, double delta) override;
        virtual COM::Result Configure() override;

    private:
        IGraphicsSystem *system = nullptr;
        IOutput *output = nullptr;
        Core::Array<IOutput *> outputs;
    };
}