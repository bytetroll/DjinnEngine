cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Core.Memory)
Djinn_Project_AddDependencies(
        Libraries.Core.Idioms
        Libraries.Core.Atomic
        Libraries.Core.Template
        Libraries.Core.Compiler
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Create tests
Djinn_CreateTest(Libraries.Core.Memory Tests)

# Finalize
Djinn_Finalize()
