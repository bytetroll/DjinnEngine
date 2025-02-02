cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Core.Compiler)
Djinn_Project_SearchSources(
        Include Source
)
Djinn_Project_Install(STATIC)

# External dependencies
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks(winmm dbghelp shlwapi version)
endif ()

# Finalize
Djinn_Finalize()