cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Data.Texture)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.VectorMath
        Libraries.Host
        Libraries.GHA
        Libraries.Platform
        Plugins.Async
        Plugins.Data.Asset
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create links to devil
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/devil/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/devil/windows/lib/DevIL.lib")
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/devil/windows/bin/DevIL.dll")
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/devil/windows/bin/ILU.dll")
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/devil/windows/bin/ILUT.dll")
else ()
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/devil/linux/lib/libIL.so")
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/devil/linux/bin/libIL.so")
endif ()

# Create links to amd compressonator
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/compressonator/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/compressonator/windows/lib/Compressonator.lib")
else ()
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/compressonator/linux/lib/libCompressonator.a")
endif ()

# Create test project
Djinn_CreateProject(Plugins.Data.Texture.Tests)
Djinn_Project_SetBuildPath(Tests/Pipeline)
Djinn_Project_AddDependencies(
        Plugins.Data.Texture
        Plugins.Base.Application
)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()