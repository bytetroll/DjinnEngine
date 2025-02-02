cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Base.Memory)
Djinn_Project_AddDependencies(
        Libraries.Core.Compiler
        Libraries.Core.Template
        Libraries.Core.Stream
        Libraries.COM
        Libraries.Host
        Libraries.Platform.Threading
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Minhook
Djinn_Project_AddExternIncludeDirs(GLOBAL "${DJINN_PATH_EXTERN}/minhook/include")
Djinn_Project_AddExternLinks("${DJINN_PATH_EXTERN}/minhook/libs/windows/MinHook.x64.lib")

# Finalize
Djinn_Finalize()