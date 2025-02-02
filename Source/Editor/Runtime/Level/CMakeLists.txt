cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Editor.Level)
Djinn_Project_SetBuildPath(${DJINN_ROOT}/Editor)
Djinn_Project_AddDependencies(
        Plugins.UI.Controls
        Editor.World
        Plugins.Game.Scene
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Finalize
Djinn_Finalize()

