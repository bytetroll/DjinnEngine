cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Libraries)
Djinn_Project_AddDependencies(
        Libraries.COM
        Libraries.Core
        Libraries.Host
        Libraries.Platform
        Libraries.GHA
        Libraries.VectorMath
        Libraries.Reflection
)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()