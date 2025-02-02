cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Game.World)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.Host
        Libraries.Platform
        Libraries.VectorMath
        Plugins.Async
        Plugins.Game.System
        Plugins.Data.World
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Finalize
Djinn_Finalize()