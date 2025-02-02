cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Configuration)
Djinn_Project_AddDependencies(
        Libraries.COM
        Libraries.Host
        Libraries.Core
        Libraries.Platform
        Plugins.Base.Logging
)
Djinn_Project_SearchSources(
        Include Source
)

# ...
Djinn_Project_Install(DYNAMIC)

# Finalize
Djinn_Finalize()