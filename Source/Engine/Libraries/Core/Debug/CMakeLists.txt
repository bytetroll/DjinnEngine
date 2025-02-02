cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Core.Debug)

# Debug is used by everything... including the rest of Core.  We CANNOT HAVE DEPS for the debug library.
Djinn_Project_AddDependencies(
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Finalize
Djinn_Finalize()