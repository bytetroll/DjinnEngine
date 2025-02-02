cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Examples.Darwin.Sandbox)
Djinn_Project_AddDependencies(
    Libraries.Core
)
Djinn_Project_SearchSources(Code)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()