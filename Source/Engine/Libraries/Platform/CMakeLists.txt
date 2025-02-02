cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Libraries.Platform)
Djinn_Project_AddDependencies(
        Libraries.Platform.Library
        Libraries.Platform.Threading
        Libraries.Platform.User
        Libraries.Platform.IO
        Libraries.Platform.Window

)
Djinn_Project_SearchSources(Include)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()