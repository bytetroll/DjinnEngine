cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Examples.Async3.0)
Djinn_Project_AddDependencies(Libraries Plugins.Async Plugins.Base)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()