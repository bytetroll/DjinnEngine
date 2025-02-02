cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Game.Component)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.Host
        Libraries.Platform
        Plugins.Base.Logging
        Plugins.Game.System
        Plugins.Game.World
        Plugins.Graphics.World
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Finalize
Djinn_Finalize()