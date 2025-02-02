cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Editor.Application)
Djinn_Project_SetBuildPath(${DJINN_ROOT}/Editor)
Djinn_Project_SetBuildName(Editor)
Djinn_Project_AddDependencies(
        Plugins.Base.Application
        Editor.Runtime
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()

