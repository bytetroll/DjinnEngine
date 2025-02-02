cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.VectorMath)
Djinn_Project_AddDependencies(
        Libraries.Core.Containers
        Libraries.Core.Time
        Libraries.Core.Math
        Libraries.Core.Compiler
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Create project
Djinn_CreateProject(Libraries.VectorMath.Tests)
Djinn_Project_AddDependencies(Libraries.VectorMath)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()