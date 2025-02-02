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
 on 5/1/2018.
//
#pragma once

#include <Async/Container.h>
#include <Core/HashMap.h>
#include "IPipeline.h"

namespace Djinn::Base {
    class Pipeline : public IPipeline {
    public:
        DJINN_COM_CLASS();

        Pipeline(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        /// Overrides
        void SetConfigurationMode(PipelineConfigurationMode mode) override;
        COM::Result Configure() override;
        COM::Result Register(ISystem *system) override;
        COM::Result Deregister(ISystem *system) override;
        COM::Result AddDependency(ISystem *source, ISystem *dest) override;
        virtual COM::Result RegisterProxy(const COM::ClassID &cid, ISystem *system) override;
        COM::Result GetClass(const COM::ClassID &id, ISystem **out) override;
        COM::Result GetInterface(const COM::InterfaceID &id, ISystem **out) override;
        void RunAsync(Async::Group &group, const Core::Delegate<void(ISystem *)> &delegate) override;

    private:
        Core::Atomic<bool> dirty{false};
        Core::HashMap<COM::ClassID, ISystem*> classes;
        Core::Array<ISystem*> systems;
        Core::Array<Core::Pair<ISystem*, ISystem*>> uniformDependencies;
        PipelineConfigurationMode mode = PipelineConfigurationMode::eIteration;
        Async::Container<ISystem*> container;
        Platform::SpinMutex mutex;
    };
}