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
 on 10/18/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include "Archive.h"

namespace Djinn::Serialization {
    class ISerializer : public COM::TUnknown<ISerializer> {
    public:
        DJINN_COM_INTERFACE();

        ISerializer(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : TUnknown(registry, outer, host) {

        }

        /// Serialize supplied object
        /// \param object
        /// \return
        virtual COM::Result Serialize(Archive &archive, COM::IUnknown *object) = 0;

        /// Deserialize from archive
        /// \param object
        /// \return
        virtual COM::Result Deserialize(Archive& archive, COM::IUnknown** object) = 0;

        /// Deserialize from archive into existing object
        /// \param object
        /// \return
        virtual COM::Result Deserialize(Archive& archive, COM::IUnknown* object) = 0;
    };
}