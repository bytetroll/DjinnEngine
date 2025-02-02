cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Editor.Importers)
Djinn_Project_AddDependencies(
        Editor.Importers.Mesh
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()