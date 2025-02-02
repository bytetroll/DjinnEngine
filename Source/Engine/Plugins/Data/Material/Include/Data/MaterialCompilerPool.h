//
// Created by Dev on 4/6/2018.
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