cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Plugins.Base)
Djinn_Project_AddDependencies(
        Plugins.Base.System
        Plugins.Base.Application
        Plugins.Base.Logging
        Plugins.Base.FileSystem
        Plugins.Base.Window
        Plugins.Base.Memory
)
Djinn_Project_SearchSources(Include)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()