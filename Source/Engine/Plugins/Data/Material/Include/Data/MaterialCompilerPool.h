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
 on 4/6/2018.
//

#include <Platform/ConditionalVar.h>
#include <Core/Queue.h>
#include <Core/Array.h>
#include <NyLang/Core/Shared.h>
#include <Data/IShaderCompilerPool.h>

namespace NyLang {
    class Registry;
}

namespace Djinn::Data {
    class DJINN_EXPORT_PLUGINS_DATA_MATERIAL MaterialCompilerPool : public IShaderCompilerPool {
    public:
        DJINN_COM_CLASS();

        MaterialCompilerPool(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host);

        COM::Result Initialize();

        /// Overrides
        virtual COM::Result Pool(NyLang::Registry **out) override;
        virtual void Push(NyLang::Registry *compiler) override;

    private:
        COM::Result Create();

        Platform::Mutex mutex;
        Platform::ConditionalVar var;
        Core::Queue<NyLang::Registry*> available;
        Core::Array<NyLang::Registry*> registries;
    };
}