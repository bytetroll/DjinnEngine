cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Data.Font)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.VectorMath
        Libraries.Host
        Libraries.Platform
        Libraries.GHA
        Plugins.Data.Asset
        Plugins.Data.Texture
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create links to ny
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/freetype/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/freetype/windows/libs/freetype.lib")
else ()
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/freetype/linux/lib/libfreetype.a")
endif ()

# Finalize
Djinn_Finalize()