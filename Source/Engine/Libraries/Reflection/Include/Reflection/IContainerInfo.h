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
 on 5/29/2018.
//
#pragma once

#include <Core/Size.h>
#include <COM/IUnknown.h>
#include <Core/ArrayView.h>
#include "ID.h"

namespace Djinn::Reflection {
    class IAttribute;

    class IContainerInfo : public COM::IQueryable {
    public:
        DJINN_COM_INTERFACEID();

        /// Get attributes
        /// \return
        virtual Core::ArrayView<IAttribute *> GetAttributes() = 0;

        /// Get attribute with class id
        /// \param id
        /// \return
        virtual IAttribute *GetAttribute(const ClassID &id) = 0;

        /// Get attribute from type
        /// \return
        template<typename T>
        T *GetAttribute() {
            return static_cast<T *>(GetAttribute(T::kCID));
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case kIID:
                    *out = static_cast<IContainerInfo*>(this);
                    break;
            }
            return COM::kUnknownInterface;
        }
    };
}