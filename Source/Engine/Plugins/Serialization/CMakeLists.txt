cmake_minimum_required(VERSION 3.7)
include(../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Serialization)
Djinn_Project_AddDependencies(
        Libraries.Core
        Libraries.Host
        Libraries.Platform
        Libraries.VectorMath
        Plugins.Base.Logging
        Plugins.Reflection
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(DYNAMIC)

# Create project
Djinn_CreateProject(Plugins.Serialization.Tests)
Djinn_Project_SetBuildPath(Pipeline)
Djinn_Project_AddDependencies(Plugins.Serialization)
Djinn_Project_SearchSources(Tests)
Djinn_Project_Install(EXEC)

# Finalize
Djinn_Finalize()