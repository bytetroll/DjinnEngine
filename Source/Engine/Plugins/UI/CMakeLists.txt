cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Plugins.UI)
Djinn_Project_AddDependencies(
        Plugins.UI.Core
        Plugins.UI.Controls
        Plugins.UI.Desktop
        Plugins.UI.Diagnostic
        Plugins.UI.UIML
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()