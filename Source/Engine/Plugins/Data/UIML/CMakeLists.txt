cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Data.UIML)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.VectorMath
        Libraries.Host
        Libraries.Platform
        Libraries.GHA
        Plugins.Data.Asset
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create project
Djinn_CreateProject(Plugins.Data.UIML.Tests)
Djinn_Project_AddDependencies(
        Plugins.Data.UIML
        Plugins.Base.Application
)
Djinn_Project_SearchSources(Tests)
Djinn_Project_SetBuildPath(Tests/Pipeline)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()