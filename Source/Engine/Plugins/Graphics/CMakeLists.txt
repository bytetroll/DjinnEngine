cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Plugins.Graphics)
Djinn_Project_AddDependencies(
        Plugins.Graphics.Graph
        Plugins.Graphics.System
        Plugins.Graphics.Common
        Plugins.Graphics.Camera
        Plugins.Graphics.Lighting
        Plugins.Graphics.World
        Plugins.Graphics.Scene
        Plugins.Graphics.UI
        Plugins.Graphics.PostProcessing
        Plugins.Graphics.DeferredLighting
        Plugins.Graphics.Output
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()