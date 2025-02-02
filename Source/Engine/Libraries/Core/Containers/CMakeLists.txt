cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Core.Containers)
Djinn_Project_AddDependencies(
        Libraries.Core.Compiler
        Libraries.Core.Atomic
        Libraries.Core.Template
        Libraries.Core.Memory
        Libraries.Core.Hash
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Create tests
Djinn_CreateProject(Libraries.Core.Containers.Tests)
Djinn_Project_SearchSources(Tests)
Djinn_Project_AddDependencies(Libraries.Core.Containers Libraries.Core.Test)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()