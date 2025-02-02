cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.GHA.VulkanShader)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.VectorMath
        Libraries.Host
        Libraries.Platform
        Plugins.Data.Asset
        Plugins.Data.Shader
        Plugins.Serialization
        Plugins.GHA.PipelineScheduler
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create links to vulkan
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/vulkan/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/vulkan/windows/lib/vulkan-1.lib"
            "${DJINN_PATH_EXTERN}/vulkan/windows/lib/VkLayer_utils.lib"
            "${DJINN_PATH_EXTERN}/vulkan/windows/lib/VkLayer_core_validation.lib"
            "${DJINN_PATH_EXTERN}/vulkan/windows/lib/VkLayer_object_tracker.lib"
            "${DJINN_PATH_EXTERN}/vulkan/windows/lib/VkLayer_parameter_validation.lib"
            "${DJINN_PATH_EXTERN}/vulkan/windows/lib/VkLayer_threading.lib"
            "${DJINN_PATH_EXTERN}/vulkan/windows/lib/VkLayer_unique_objects.lib"
    )
else ()
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/vulkan/linux/lib/libvulkan.so"
            "${DJINN_PATH_EXTERN}/vulkan/linux/lib/libVkLayer_utils.so"
            "${DJINN_PATH_EXTERN}/vulkan/linux/lib/libVkLayer_core_validation.so"
            "${DJINN_PATH_EXTERN}/vulkan/linux/lib/libVkLayer_object_tracker.so"
            "${DJINN_PATH_EXTERN}/vulkan/linux/lib/libVkLayer_parameter_validation.so"
            "${DJINN_PATH_EXTERN}/vulkan/linux/lib/libVkLayer_threading.so"
            "${DJINN_PATH_EXTERN}/vulkan/linux/lib/libVkLayer_unique_objects.so"
    )
endif ()

# Create links to glslang
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/glslang/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/glslang/windows/libs/glslang.lib"
            "${DJINN_PATH_EXTERN}/glslang/windows/libs/glslang-default-resource-limits.lib"
            "${DJINN_PATH_EXTERN}/glslang/windows/libs/HLSL.lib"
            "${DJINN_PATH_EXTERN}/glslang/windows/libs/OGLCompiler.lib"
            "${DJINN_PATH_EXTERN}/glslang/windows/libs/OSDependent.lib"
            "${DJINN_PATH_EXTERN}/glslang/windows/libs/SPIRV.lib"
            "${DJINN_PATH_EXTERN}/glslang/windows/libs/SPVRemapper.lib"
    )
else ()
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/glslang/linux/lib/libglslang.a"
            "${DJINN_PATH_EXTERN}/glslang/linux/lib/libglslang-default-resource-limits.a"
            "${DJINN_PATH_EXTERN}/glslang/linux/lib/libHLSL.a"
            "${DJINN_PATH_EXTERN}/glslang/linux/lib/libOGLCompiler.a"
            "${DJINN_PATH_EXTERN}/glslang/linux/lib/libOSDependent.a"
            "${DJINN_PATH_EXTERN}/glslang/linux/lib/libSPIRV.a"
            "${DJINN_PATH_EXTERN}/glslang/linux/lib/libSPVRemapper.a"
    )
endif ()

# Create links to lunarglass
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/lunarglass/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/lunarglass/windows/libs/glslangFrontend.lib"
            "${DJINN_PATH_EXTERN}/lunarglass/windows/libs/SpvFrontend.lib"
            "${DJINN_PATH_EXTERN}/lunarglass/windows/libs/GLSLBackend.lib"
            "${DJINN_PATH_EXTERN}/lunarglass/windows/libs/core.lib"
            "${DJINN_PATH_EXTERN}/lunarglass/windows/libs/TgsiBackend.lib"
            "${DJINN_PATH_EXTERN}/lunarglass/windows/libs/DummyBackend.lib"
    )
else ()
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/lunarglass/linux/lib/libglslangFrontend.a"
            "${DJINN_PATH_EXTERN}/lunarglass/linux/lib/libSpvFrontend.a"
            "${DJINN_PATH_EXTERN}/lunarglass/linux/lib/libGLSLBackend.a"
            "${DJINN_PATH_EXTERN}/lunarglass/linux/lib/libcore.a"
            "${DJINN_PATH_EXTERN}/lunarglass/linux/lib/libTgsiBackend.a"
            "${DJINN_PATH_EXTERN}/lunarglass/linux/lib/libDummyBackend.a"
    )
endif ()

# Create links to spirv-tools
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/spirvtools/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/spirvtools/windows/libs/SPIRV-Tools.lib")
else ()
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/spirvtools/linux/lib/libSPIRV-Tools.a")
endif ()

# Create links to lunarglass_llvm34
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/lunarglass_llvm34/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMInterpreter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAsmParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMOption.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMDebugInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMCJIT.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMExecutionEngine.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMRuntimeDyld.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMLTO.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMObjCARCOpts.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMLinker.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMBitWriter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMXCoreDisassembler.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMXCoreCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMXCoreDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMXCoreInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMXCoreAsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMX86Disassembler.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMX86AsmParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMX86CodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMX86Desc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMX86Info.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMX86AsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMX86Utils.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSystemZDisassembler.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSystemZCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSystemZAsmParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSystemZDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSystemZInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSystemZAsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSparcCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSparcDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSparcInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMR600CodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMipo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMVectorize.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMR600Desc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMR600Info.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMR600AsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMPowerPCCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMPowerPCAsmParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMPowerPCDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMPowerPCInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMPowerPCAsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMNVPTXCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMNVPTXDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMNVPTXInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMNVPTXAsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMSP430CodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMSP430Desc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMSP430Info.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMSP430AsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMipsDisassembler.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMipsCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMipsAsmParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMipsDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMipsInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMipsAsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMHexagonCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMHexagonDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMHexagonInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMCppBackendCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMCppBackendInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMARMDisassembler.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMARMCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMARMAsmParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMARMDesc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMARMInfo.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMARMAsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAArch64Disassembler.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMCDisassembler.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAArch64CodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSelectionDAG.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMCodeGen.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMScalarOpts.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMObject.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMBitReader.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMInstCombine.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMTransformUtils.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMipa.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAnalysis.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMTarget.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMCore.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAArch64AsmParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMCParser.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAArch64Desc.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAArch64Info.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAArch64AsmPrinter.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMMC.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMAArch64Utils.lib"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/windows/libs/LLVMSupport.lib"
            imagehlp
    )
else ()
    Djinn_Project_AddExternLinks(
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMInterpreter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAsmParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMOption.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMDebugInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMCJIT.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMExecutionEngine.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMRuntimeDyld.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMLTO.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMObjCARCOpts.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMLinker.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMBitWriter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMXCoreDisassembler.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMXCoreCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMXCoreDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMXCoreInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMXCoreAsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMX86Disassembler.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMX86AsmParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMX86CodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMX86Desc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMX86Info.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMX86AsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMX86Utils.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSystemZDisassembler.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSystemZCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSystemZAsmParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSystemZDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSystemZInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSystemZAsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSparcCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSparcDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSparcInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMR600CodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMipo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMVectorize.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMR600Desc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMR600Info.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMR600AsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMPowerPCCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMPowerPCAsmParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMPowerPCDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMPowerPCInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMPowerPCAsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMNVPTXCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMNVPTXDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMNVPTXInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMNVPTXAsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMSP430CodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMSP430Desc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMSP430Info.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMSP430AsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMipsDisassembler.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMipsCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMipsAsmParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMipsDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMipsInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMipsAsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMHexagonCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMHexagonDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMHexagonInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMCppBackendCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMCppBackendInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMARMDisassembler.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMARMCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMARMAsmParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMARMDesc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMARMInfo.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMARMAsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAArch64Disassembler.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMCDisassembler.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAArch64CodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSelectionDAG.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMCodeGen.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMScalarOpts.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMObject.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMBitReader.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMInstCombine.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMTransformUtils.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMipa.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAnalysis.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMTarget.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMCore.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAArch64AsmParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMCParser.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAArch64Desc.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAArch64Info.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAArch64AsmPrinter.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMMC.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMAArch64Utils.a"
            "${DJINN_PATH_EXTERN}/lunarglass_llvm34/linux/lib/libLLVMSupport.a"
            -ldl
            -pthread
    )
endif ()

# Create links to spirv header
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/spirvheaders/include")

# Create test project
Djinn_CreateProject(Plugins.GHA.VulkanShader.Tests)
Djinn_Project_SetBuildPath(Tests/Pipeline)
Djinn_Project_AddDependencies(
        Plugins.Base.Application
        Plugins.GHA.VulkanShader
)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()