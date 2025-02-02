//
// Created by Dev on 11/30/2017.
//

#include <GHA/Vulkan/GlassManager.h>
#include <Core/Common.h>
#include <glslang/ShaderLang.h>
#include <lunarglass/Backends/GLSL/GlslTarget.h>
#include <llvm/Analysis/Verifier.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/PassManager.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <Base/DefaultLogs.h>
#include <Host/IRegistry.h>
#include <llvm/Support/raw_ostream.h>
#include <fstream>

DJINN_NS3(GHA, Vulkan);

GlassManager::GlassManager(Host::IRegistry *registry, const Data::ShaderCompileNILDesc &desc) {
    this->registry = registry;

    // Convert stage
    EShLanguage lang;
    switch (desc.stage) {
        case GHA::Stage::eVertex:
            lang = EShLanguage::EShLangVertex;
            break;
        case GHA::Stage::eHull:
            lang = EShLanguage::EShLangTessControl;
            break;
        case GHA::Stage::eDomain:
            lang = EShLanguage::EShLangTessEvaluation;
            break;
        case GHA::Stage::eGeometry:
            lang = EShLanguage::EShLangGeometry;
            break;
        case GHA::Stage::ePixel:
            lang = EShLanguage::EShLangFragment;
            break;
        case GHA::Stage::eCompute:
            lang = EShLanguage::EShLangCompute;
            break;
        default:
            Always("Bad stage");
    }

    // Defaults
    setStage(lang);
    setProfile(EProfile::ENoProfile);
    setVersion(450);

    // Create llvm state
    context = new llvm::LLVMContext();
    module = new llvm::Module("ShaderNILCompiler", getContext());

    // Create backends
    backEnd = gla::GetGlslBackEnd();
    backEndTranslator = gla::GetGlslTranslator(this, false);
}

GlassManager::~GlassManager() {
    //delete backEndTranslator;
    //delete backEnd;
    //delete module;
    //delete context;
}

void GlassManager::createContext() {}

void GlassManager::clear() {
}

COM::Result GlassManager::Compile(Core::String &out) {
    /*{
        // ...
        std::string strv;
        llvm::raw_string_ostream str(strv);
        getModule()->print(str, nullptr);
        std::ofstream ff("debug.llilp");
        ff << strv;
        ff.close();
    }*/

    // Verify module
    /*llvm::PassManager verifier;
    verifier.add(llvm::createVerifierPass(llvm::VerifierFailureAction::PrintMessageAction));
    if (verifier.run(*module)) {
        ErrorLog(registry).Write("Glass llvm module broken, full module:");
        module->dump();
        return COM::kError;
    }*/

    // Force inline all tagged functions
    {
        llvm::PassManager pm;
        pm.add(llvm::createAlwaysInlinerPass());
        pm.run(*module);
    }

    // Remove all tagged functions
    std::vector<llvm::Function *> Bucket;
    for (auto &It : module->getFunctionList()) {
        if (!It.isIntrinsic() && std::string(It.getName().data()) != "main") {
            Bucket.push_back(&It);
        }
    }
    for (llvm::Function *It : Bucket) {
        DJINN_ASSERT(It->getNumUses() == 0, "Attempting to remove a referenced function");
        It->removeFromParent();
    }

    // Perform back end optimizations
    translateTopToBottom();

    // Custom passes
    for (auto fit = module->getFunctionList().begin(); fit != module->getFunctionList().end(); fit++) {
        for (auto bit = fit->getBasicBlockList().begin(); bit != fit->getBasicBlockList().end(); bit++) {
            PatchTextureMapInlining(&*bit);
        }
    }

    // Perform custom passes
    llvm::PassManager optPM;
    optPM.add(llvm::createLoopUnrollPass(500, -1, 1, -1));
    optPM.add(llvm::createFunctionInliningPass());
    optPM.add(llvm::createScalarReplAggregatesPass());
    //optPM.add(llvm::createInstructionCombiningPass());
    optPM.add(llvm::createAggressiveDCEPass());
    optPM.run(*module);
    optPM.run(*module);

    /*{
        // ...
        std::string strv;
        llvm::raw_string_ostream str(strv);
        getModule()->print(str, nullptr);
        std::ofstream ff("debug.llilo");
        ff << strv;
        ff.close();
    }*/

    // Translate backend to glsl
    translateBottomToTarget();

    /*std::string strv;
    llvm::raw_string_ostream str(strv);
    getModule()->print(str, nullptr);
    std::ofstream ff("debug.llil");
    ff << strv;
    ff.close();*/

    // ...
    out = static_cast<gla::GlslTranslator *>(backEndTranslator)->getGeneratedShader();

    // OK
    return COM::kOK;
}

void GlassManager::PatchTextureMapInlining(llvm::BasicBlock *block) {
    for (auto it = block->getInstList().begin(); it != block->getInstList().end(); it++) {
        if (auto alloca = llvm::dyn_cast<llvm::AllocaInst>(&*it)) {
            auto copy = it;
            // Are we allocating a texture map?
            if (std::equal("TextureMap", &"TextureMap" [sizeof("TextureMap")], alloca->getAllocatedType()->getStructName().data())) {
                auto gep = llvm::dyn_cast<llvm::GetElementPtrInst>(++copy);
                if (!gep) {
                    continue;
                }

                auto load = llvm::dyn_cast<llvm::LoadInst>(++copy);
                if (!load) {
                    continue;
                }

                auto store = llvm::dyn_cast<llvm::StoreInst>(++copy);
                if (!store) {
                    continue;
                }

                auto gep2 = llvm::dyn_cast<llvm::GetElementPtrInst>(++copy);
                if (!gep2) {
                    continue;
                }

                auto load2 = llvm::dyn_cast<llvm::LoadInst>(++copy);
                if (!load2) {
                    continue;
                }

                auto index = std::next(gep2->idx_begin(), 2)->get();

                llvm::IRBuilder<true, llvm::ConstantFolder, llvm::IRBuilderDefaultInserter<true>> builder(getContext());
                builder.SetInsertPoint(block);
                builder.SetInsertPoint(load2);

                auto ggep = static_cast<llvm::Instruction *>(builder.CreateLoad(builder.CreateGEP(
                        load->getPointerOperand(),
                        std::vector<llvm::Value *>{
                                llvm::ConstantInt::get(llvm::IntegerType::get(getContext(), 32), 0),
                                index
                        }
                )));

                llvm::ReplaceInstWithInst(load2, ggep);

                store->eraseFromParent();
            }
        }
    }
}
