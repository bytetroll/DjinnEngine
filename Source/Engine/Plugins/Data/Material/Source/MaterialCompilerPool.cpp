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

#include <Data/MaterialCompilerPool.h>
#include <Platform/LockGuard.h>
#include <NyLang/Diagnostic/Diagnostic.h>
#include <NyLang/Frontend/Compiler/Compiler.h>
#include <NyLang/Frontend/Tokenizer/Tokenizer.h>
#include <NyLang/Frontend/Parser/Parser.h>
#include <NyLang/Frontend/Semantic/Semantic.h>
#include <NyLang/Native/NativeExtensions.h>
#include <NyLang/Core/Host/Registry.h>
#include <Data/NyShader/Operations.h>
#include <Base/DefaultLogs.h>
#include <Platform/User.h>
#include <Data/NyShader/MaterialOperations.h>

DJINN_NS2(Data);

MaterialCompilerPool::MaterialCompilerPool(Host::IRegistry *registry, COM::IUnknown *outer, COM::IInstanceHost* host) : IShaderCompilerPool(registry, outer, host) {

}

COM::Result MaterialCompilerPool::Initialize() {
    /*for (UInt32 i = 0; i < Platform::User::GetCoreCount(); i++) {
        if (!Create()) {
            return COM::kError;
        }
    }*/
    return COM::kOK;
}

COM::Result MaterialCompilerPool::Pool(NyLang::Registry **out) {
    if (!Create()) {
        return COM::kError;
    }

    var.WaitNoUnlock(mutex, [&]() { return available.Size() > 0; });

    if (!available.Pop(*out)) {
        ErrorLog(this).Write("Failed to pool material shader compiler");
        return COM::kError;
    }

    mutex.Unlock();
    return COM::kOK;
}

void MaterialCompilerPool::Push(NyLang::Registry *compiler) {
    Platform::LockGuard guard(mutex);

    // available.Add(compiler);
    delete compiler;
}

COM::Result MaterialCompilerPool::Create() {
    Platform::LockGuard guard(mutex);

    // Create registry
    auto registry = new NyLang::Registry();
    registries.Add(registry);

    // Install diagnostic
    auto diag = NyLang::Diagnostic::Install(registry);

    // Create compiler
    auto compiler = NyLang::Compiler::Install(registry);
    compiler->SetTokenizer(registry->context.New<NyLang::Tokenizer>(registry));
    compiler->SetParser(registry->context.New<NyLang::Parser>(registry));
    compiler->SetSemantic(registry->context.New<NyLang::Semantic>(registry));
    compiler->Initialize();

    // Install default extensions and operations
    if (!NyLang::InstallNativeExtensions(registry) || !NyShader::InstallOperations(this->registry, registry) || !NyShader::InstallMaterialOperations(registry)) {
        ErrorLog(this->registry).Write("Failed to compile ny native extensions and operations");
        return COM::kError;
    }

    // Configure
    if (!compiler->Configure()) {
        ErrorLog(this->registry).Write("Failed to configure the ny compiler");
        return COM::kError;
    }

    // ...
    available.Add(registry);

    // Diagnostic
    // InfoLog(this).Write("Initialized");
    return COM::kOK;
}
