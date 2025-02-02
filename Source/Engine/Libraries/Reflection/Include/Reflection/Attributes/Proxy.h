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
 on 9/12/2018.
//

#pragma once

#include <Reflection/IAttribute.h>

namespace Djinn::Attributes {
    struct Proxy : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        Proxy(const char *name);

        /// Overrides
        void Attached(COM::IQueryable *container) override;
        COM::ClassID GetClassID() override;

        const char *name;
    };

    struct EnumProxy : public Reflection::IAttribute {
        DJINN_COM_CLASSID();

        EnumProxy(const char *from, const char* to);

        /// Overrides
        void Attached(COM::IQueryable *container) override;
        COM::ClassID GetClassID() override;

        const char *from;
        const char *to;
    };
}