//
// Created by Dev on 11/30/2017.
//
#pragma once

#include <COM/IUnknown.h>
#include <Core/HashMap.h>
#include <Core/SharedPtr.h>
#include <Data/ShaderCompilerNILDesc.h>
#include <lunarglass/Core/metadata.h>
#include <Core/List.h>
#include <GHA/PipelineScheduler/ICompiledShaderBlob.h>
#include <Core/HashSet.h>
#include <NyLang/Core/Host/Registry.h>
#include "AnnotationChain.h"

namespace gla {
    class Metadata;
}

namespace NyLang::IL {
    class Program;
    class Class;
    class IType;
    class Function;
    class IValue;
    class Block;
    class Container;
    class ConstantMetaData;
    class Member;
}

namespace llvm {
    class StructType;
    class Type;
    class Function;
    class BasicBlock;
    class Value;
    class MDNode;
    class DataLayout;
}

namespace Djinn::GHA::Vulkan {
    class GlassManager;
    class CompiledShaderBlob;

    class GlassCompiler {
    public:
        /// Construct
        /// \param manager
        GlassCompiler(Host::IRegistry* registry, GlassManager* manager);

        /// Compile program to manager
        /// \param desc
        /// \param program
        /// \return
        COM::Result Compile(NyLang::Registry* registry, const Data::ShaderCompileNILDesc& desc, NyLang::IL::Program *program, GHA::ShaderBlob* shaderBlob, Vulkan::CompiledShaderBlob* blob);

    private:
        COM::Result FlattenInput(llvm::BasicBlock* block, NyLang::IL::IType* type, NyLang::IL::IValue* ilValue, llvm::Value* ptr, AnnotationChain chain, const Core::String& decoration = "var");
        COM::Result FlattenOutput(llvm::BasicBlock* block, NyLang::IL::IType* type, NyLang::IL::IValue* ilValue, llvm::Value* value, AnnotationChain chain, const Core::String& decoration, int& offset);
        COM::Result FlattenExtern(Vulkan::CompiledShaderBlob* blob, NyLang::IL::IType* type, NyLang::IL::IValue* value, AnnotationChain chain, bool isVisible, llvm::Value** out, const Core::String& decoration = "var", const Core::String& baseName = "var");
        COM::Result GetAggregateMD(NyLang::IL::Class *container, llvm::MDNode* layout, llvm::MDNode** out, const Core::String& decoration = "var");

        /// Get type
        /// \param type
        /// \return
        COM::Result GetType(NyLang::IL::IType* type, llvm::Type** out);

        /// Get value
        /// \param value
        /// \param out
        /// \return
        COM::Result GetValue(NyLang::IL::IValue* value, llvm::BasicBlock* block, llvm::Value** out);

        /// Compile function
        /// \param ilFunction
        /// \return
        COM::Result CompileFunction(NyLang::IL::Container* ilContainer, NyLang::IL::Function* ilFunction, bool isMain);

        /// Get register
        /// \param _register
        /// \param set
        /// \return
        UInt32 GetLocation(UInt32 _register, UInt32 set = 0);

        llvm::Value* GetTypeProxy(llvm::Type* type, const Core::String& name);

        Core::String SanitizeName(const Core::String& str);

        COM::Result GetBuiltin(const AnnotationChain& chain, bool isInput, gla::EMdBuiltIn& out);

        COM::Result GetInput(const AnnotationChain& chain, const Core::String& name, GHA::PipelineScheduler::CompiledDescriptor& out);

        Core::String Unique(const char* str);

        void CompileFunctionList(NyLang::IL::Function* function);

        bool IsVisible(NyLang::IL::IValue* value);

    private:
        Host::IRegistry *registry;
        GlassManager* manager;
        llvm::DataLayout* layout;
        GHA::ShaderBlob* shaderBlob;
        Data::ShaderCompileNILDesc desc;

        llvm::StructType* dynamicStorageType = nullptr;
        llvm::Value* dynamicStorage = nullptr;
        Core::Array<llvm::Type*> dynamicTypes;
        Core::Array<NyLang::IL::Member*> dynamicValues;

        // Maps
        Core::HashSet<NyLang::IL::Function*> functionList;
        Core::HashSet<NyLang::IL::Function*> isFramed;
        Core::HashMap<NyLang::IL::Class *, llvm::StructType *> typeMap;
        Core::HashMap<NyLang::IL::Function *, llvm::Function *> functionMap;
        Core::HashMap<NyLang::IL::IValue *, Core::Pair<llvm::Value *, llvm::Function*>> functionFrameMap;
        Core::HashMap<NyLang::IL::Block *, llvm::BasicBlock *> blockMap;
        Core::HashMap<NyLang::IL::IValue *, llvm::Value *> valueMap;
        Core::HashMap<int, llvm::Value *> returnValueMap;
        Core::SharedPtr<gla::Metadata> metaData;
        Core::HashMap<Core::String, UInt32> uniqueMap;
        Core::HashMap<NyLang::IL::Container*, llvm::MDNode*> aggregateMDs;
        UInt32 registerOffsets[32] = {};

        struct ValueProxy {
            llvm::Type* classType;
            Core::Array<llvm::Value*> values;
        };

        struct PackedProxy {
            llvm::Value* source;
            UInt32 index;
        };

        COM::Result ExpandValueProxy(llvm::BasicBlock* block, const ValueProxy& proxy, llvm::Value** out);

        Core::HashMap<llvm::Value *, ValueProxy> proxyValueMap;
        Core::HashMap<NyLang::IL::IValue *, PackedProxy> packedValueMap;

        // Current registers
        UInt32 inputRegister = 0;
        UInt32 outputRegister = 0;
        UInt32 lookupRegister = 0;
    };
}
