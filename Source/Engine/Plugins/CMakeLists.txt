cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Plugins)
Djinn_Project_AddDependencies(
        Plugins.Base
        Plugins.Game
        Plugins.Graphics
        Plugins.Data
        Plugins.Reflection
        Plugins.Serialization
        Plugins.GHA
        Plugins.Async
        Plugins.UI
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()