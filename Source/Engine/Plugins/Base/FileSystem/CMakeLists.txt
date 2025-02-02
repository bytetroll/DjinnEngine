cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Base.FileSystem)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.Host
        Libraries.COM
        Libraries.Platform
        Plugins.Async
        Plugins.Base.Logging
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create test project
Djinn_CreateProject(Plugins.Base.FileSystem.Tests)
Djinn_Project_AddDependencies(Plugins.Base.FileSystem)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()