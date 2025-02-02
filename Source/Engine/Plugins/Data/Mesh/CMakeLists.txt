cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Data.Mesh)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.VectorMath
        Libraries.Host
        Libraries.Platform
        Libraries.GHA
        Plugins.Data.Asset
        Plugins.Data.Material
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create links to assimp
# TODO: Why doesnt the linking directory work?
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/assimp/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/assimp/windows/lib/assimp.lib")
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/assimp/windows/bin/assimp.dll")
else ()
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/assimp/linux/lib/libassimp.so.3.3.1")
    Djinn_Project_AddExternBinaries("${DJINN_PATH_EXTERN}/assimp/linux/bin/libassimp.so.3")
endif ()

# Create test project
Djinn_CreateProject(Plugins.Data.Mesh.Tests)
Djinn_Project_SetBuildPath(Tests/Pipeline)
Djinn_Project_AddDependencies(Plugins.Data.Mesh)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()