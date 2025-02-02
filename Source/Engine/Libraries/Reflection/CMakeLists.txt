cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Reflection)
Djinn_Project_AddDependencies(
        Libraries.Core.Template
        Libraries.Core.Containers
        Libraries.Core.Reflection
        Libraries.COM
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Create test project
Djinn_CreateProject(Libraries.Reflection.Tests)
Djinn_Project_SetBuildPath(Tests/Pipeline)
Djinn_Project_AddDependencies(
        Libraries.Core.Reflection
)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()