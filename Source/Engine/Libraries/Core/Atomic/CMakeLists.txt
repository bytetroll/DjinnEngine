cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Core.Atomic)
Djinn_Project_SearchSources(Include)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()