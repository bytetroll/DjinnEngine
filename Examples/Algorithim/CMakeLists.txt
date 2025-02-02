cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Examples.Algorithim)
Djinn_Project_AddDependencies(Libraries Plugins)
Djinn_Project_SearchSources(Source)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()