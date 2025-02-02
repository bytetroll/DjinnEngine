project(DjinnEngine)

cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Finalize
Djinn_Finalize()