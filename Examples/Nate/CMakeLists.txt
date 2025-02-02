cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Examples.Nate)
Djinn_Project_AddDependencies(
        Libraries
        Plugins.Base
        Plugins.Game
        Plugins.Reflection
        Plugins.Serialization
        Plugins.Async
        Plugins.Configuration
)
Djinn_Project_SearchSources(Code)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()