cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Core.Stream)
Djinn_Project_AddDependencies(
        Libraries.Core.Compiler
        Libraries.Core.Template
        Libraries.Core.Containers
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Finalize
Djinn_Finalize()