cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.COM)
Djinn_Project_AddDependencies(
        Libraries.Core.Containers
        Libraries.Core.Compiler
        Libraries.Core.Template
        Libraries.Core.Hash
)
Djinn_Project_SearchSources(Include)
Djinn_Project_Install(VIRTUAL)

# Tests
Djinn_CreateTest(Libraries.COM Tests)

# Finalize
Djinn_Finalize()