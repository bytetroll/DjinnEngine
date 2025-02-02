cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Platform.Threading)
Djinn_Project_AddDependencies(
        Libraries.Core.Reflection
        Libraries.Core.Containers
)
Djinn_Project_SearchSources(
        Include Source
        WINDOWS IncludeWindows SourceWindows
        LINUX IncludeLinux SourceLinux
)
Djinn_Project_Install(STATIC)

# Finalize
Djinn_Finalize()