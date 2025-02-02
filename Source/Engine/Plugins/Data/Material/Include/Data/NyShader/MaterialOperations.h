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
 on 11/29/2017.
//
#pragma once

#include <COM/Result.h>
#include <NyLang/Core/Shared.h>
#include <NyLang/Core/COM/IUnknown.h>
#include <Core/String.h>

namespace NyLang {
    class Registry;
}

namespace Djinn::Data::NyShader {
    class MaterialInbuilts : public NyLang::IUnknown {
    public:
        LANG_CLASSID("MaterialInbuilts");

        MaterialInbuilts();

        /// Overrides
        virtual NyLang::ClassID GetClassID() override;

        /// Paths
        Core::String layoutPath;
        Core::String typePath;
    };

    extern DJINN_EXPORT_PLUGINS_DATA_MATERIAL COM::Result InstallMaterialOperations(NyLang::Registry* registry);
}