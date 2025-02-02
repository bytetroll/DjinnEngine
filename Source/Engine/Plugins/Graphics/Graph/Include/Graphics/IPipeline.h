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
 on 12/26/2017.
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