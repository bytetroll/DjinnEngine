cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Core.Reflection)
Djinn_Project_AddDependencies(
        Libraries.Core.Template
        Libraries.Core.Containers
)
Djinn_Project_SearchSources(Include)
Djinn_Project_Install(VIRTUAL)

# Create test project
Djinn_CreateProject(Libraries.Core.Reflection.Tests)
Djinn_Project_SetBuildPath(Tests/Pipeline)
Djinn_Project_AddDependencies(
        Libraries.Core.Reflection
)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()