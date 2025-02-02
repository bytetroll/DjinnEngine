cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Plugins.Data)
Djinn_Project_AddDependencies(
        Plugins.Data.Asset
        Plugins.Data.Mesh
        Plugins.Data.Shader
        Plugins.Data.Font
        Plugins.Data.World
        Plugins.Data.UIML
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()