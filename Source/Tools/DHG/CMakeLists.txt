cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Tools.DHG)
Djinn_Project_SetBuildMode(Release)
Djinn_Project_SetAttribute(NoDHG)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.Platform
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(EXEC)

# Create links to libclang
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/clang-c/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/clang-c/windows/libs/libclang.lib")
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/clang-c/windows/bin/libclang.dll")
else ()
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/clang-c/linux/libs/libclang.so.10" tinfo)
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/clang-c/linux/libs/libclang.so.10")
endif ()

# Finalize
Djinn_Finalize()

