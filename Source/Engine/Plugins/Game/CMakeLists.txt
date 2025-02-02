cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Plugins.Game)
Djinn_Project_AddDependencies(
        Plugins.Game.Component
        Plugins.Game.System
        Plugins.Game.Input
        Plugins.Game.Scene
        Plugins.Game.World
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()