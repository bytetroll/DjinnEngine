cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Examples.Tetris)
Djinn_Project_AddDependencies(
        Libraries
        Plugins.Graphics.Camera
        Plugins.Graphics.Common
        Plugins.Graphics.DeferredLighting
        Plugins.Graphics.Graph
        Plugins.Graphics.Lighting
        Plugins.Graphics.Output
        Plugins.Graphics.PostProcessing
        Plugins.Graphics.Scene
        Plugins.Graphics.System
        Plugins.Graphics.World
        Plugins.Serialization
        Plugins.Reflection
        Plugins.GHA
        Plugins.Async
        Plugins.Base
        Plugins.Configuration
        Plugins.Data.Asset
        Plugins.Data.Font
        Plugins.Data.Material
        Plugins.Data.Mesh
        Plugins.Data.Shader
        Plugins.Data.Texture
        Plugins.Data.World
        Plugins.Game.Component
        Plugins.Game.Input
        Plugins.Game.Scene
        Plugins.Game.System
        Plugins.Game.World
        Plugins.Data.UIML
)
Djinn_Project_SearchSources(Code)
Djinn_Project_SetBuildPath(Pipeline)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()
