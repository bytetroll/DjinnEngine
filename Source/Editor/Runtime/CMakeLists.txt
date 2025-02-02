cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Editor.Runtime)
Djinn_Project_AddDependencies(
        Editor.Layout
        Editor.World
        Editor.Window
        Editor.Graphics
        Editor.Content
        Editor.Level
        Editor.Data
        Editor.Importers
        Editor.Viewport
        Editor.Hierarchy
        Editor.Templates
        Editor.Properties
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()