cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.GHA.Vulkan)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.VectorMath
        Libraries.Host
        Libraries.Platform
        Plugins.Async
        Plugins.Data.Asset
        Plugins.Data.Shader
        Plugins.Data.Texture
        Plugins.GHA.VulkanShader
        Plugins.GHA.PipelineScheduler
)
Djinn_Project_LoadOrder(BEFORE Plugins.Graphics.System)
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
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/vulkan/linux/bin/libvulkan.so.1")
endif ()

# Create test project
#Djinn_CreateProject(Plugins.GHA.Vulkan.Tests1)
#Djinn_Project_SetBuildPath(Tests/Pipeline)
#Djinn_Project_AddDependencies(
#        Plugins.GHA.Vulkan
#        Plugins.GHA.VulkanShader
#        Plugins.Graphics.Graph
#)
#Djinn_Project_SetSources(Tests/main.cpp)
#Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()