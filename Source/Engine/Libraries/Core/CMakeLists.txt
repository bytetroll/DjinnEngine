cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Add directories
Djinn_AddAllDirs()

# Create project
Djinn_CreateProject(Libraries.Core)
Djinn_Project_AddDependencies(
        Libraries.Core.Compiler
        Libraries.Core.Containers
        Libraries.Core.Event
        Libraries.Core.Stream
        Libraries.Core.Hash
        Libraries.Core.Reflection
        Libraries.Core.Template
        Libraries.Core.Math
        Libraries.Core.Test
        Libraries.Core.Memory
        Libraries.Core.Time
		Libraries.Core.Debug
	Libraries.Core.Util
)
Djinn_Project_SearchSources(Include)
Djinn_Project_Install(VIRTUAL)

# Finalize
Djinn_Finalize()
