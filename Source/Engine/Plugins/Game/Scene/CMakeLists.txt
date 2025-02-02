cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Game.Scene)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.Host
        Libraries.Platform
        Libraries.VectorMath
        Plugins.Base.Window
        Plugins.Game.System
        Plugins.Game.Component
        Plugins.Game.Input
        Plugins.Graphics.Scene
        Plugins.Graphics.Camera
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Finalize
Djinn_Finalize()