cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Plugins.GHA)
Djinn_Project_AddDependencies(
        Plugins.GHA.Vulkan
        Plugins.GHA.PipelineScheduler
        Plugins.GHA.VulkanShader
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()