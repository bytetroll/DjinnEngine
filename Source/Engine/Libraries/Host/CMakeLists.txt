cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Host)
Djinn_Project_AddDependencies(
        Libraries.Core.Containers
        Libraries.Core.Reflection
        Libraries.Core.Time
        Libraries.Platform.Library
        Libraries.Platform.IO
        Libraries.COM
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Finalize
Djinn_Finalize()