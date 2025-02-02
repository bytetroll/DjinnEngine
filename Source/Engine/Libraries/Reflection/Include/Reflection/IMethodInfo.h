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

#include <Core/Types.h>
#include "TypeInfo.h"
#include "ParameterInfo.h"
#include <Core/DynamicDelegate.h>
#include <Core/ArrayView.h>
#include "IContainerInfo.h"

namespace Djinn::Reflection {
    class IAttribute;
    class IClassInfo;

    class IMethodInfo : public IContainerInfo {
    public:
        DJINN_COM_INTERFACEID();

        /// Get the owning class
        /// \return
        virtual IClassInfo* GetClass() = 0;

        /// Get the return type
        /// \return
        virtual TypeInfo GetReturnType() = 0;

        /// Get all parameters
        /// \return
        virtual Core::ArrayView<ParameterInfo> GetParameters() = 0;

        /// Get the name
        /// \return
        virtual const char *GetName() = 0;

        /// Get the full decorated name
        /// \return
        virtual const char *GetDecoratedName() = 0;

        /// Get the full decorated signature
        /// \return
        virtual const char *GetDecoratedSignature() = 0;

        /// Get the signature hash
        /// \return
        virtual USize GetSignatureHash() = 0;

        /// Get the dynamic delegate representation from a given frame
        /// \param frame - May be nullptr
        /// \return
        virtual Core::DynamicDelegate GetDynamic(void *frame) = 0;

        /// Get the delegate representation from a given frame
        /// \param frame
        /// \return
        template<typename T>
        Core::Delegate<T> Get(void *frame) {
            return GetDynamic(frame).As<T>();
        }

        /// Override
        COM::Result QueryInterface(const COM::InterfaceID &iid, void **out) override {
            switch (iid) {
                case IContainerInfo::kIID:
                    *out = static_cast<IContainerInfo *>(this);
                    return COM::kOK;
                case kIID:
                    *out = static_cast<IMethodInfo *>(this);
                    return COM::kOK;
            }
            return COM::kUnknownInterface;
        }
    };
}