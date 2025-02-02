cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.Platform.Window)
Djinn_Project_AddDependencies(
        Libraries.Core.Containers
        Libraries.Core.Reflection
        Libraries.COM
        Libraries.Platform.User
        Libraries.Platform.Threading
        Libraries.VectorMath
)
Djinn_Project_SearchSources(
        Include Source
        WINDOWS SourceWindows
        LINUX SourceLinux
)
Djinn_Project_Install(STATIC)

# Linux
if (${Djinn.Platform} STREQUAL "Linux")
    Djinn_Project_AddExternLinks(X11)
endif ()

# Create test project
Djinn_CreateProject(Libraries.Platform.Window.Tests)
Djinn_Project_AddDependencies(Libraries.Platform.Window)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()