cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Data.Shader)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.VectorMath
        Libraries.Host
        Libraries.Platform
        Libraries.GHA
        Plugins.Data.Asset
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create links to ny
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/ny/include")
if (${Djinn.Platform} STREQUAL "Windows")
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/ny/windows/libs/Lang.lib")
else ()
    Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/ny/linux/lib/libLang.a")
endif ()

# Create test project
Djinn_CreateProject(Plugins.Data.Shader.Tests)
Djinn_Project_SetBuildPath(Tests/Pipeline)
Djinn_Project_AddDependencies(Plugins.Data.Shader)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()