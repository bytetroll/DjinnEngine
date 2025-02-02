cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Base.Logging)
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

# Create project
Djinn_CreateProject(Plugins.Base.Logging.Tests)
Djinn_Project_AddDependencies(Plugins.Base.Logging Plugins.Base.Application)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()