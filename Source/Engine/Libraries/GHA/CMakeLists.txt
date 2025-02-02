cmake_minimum_required(VERSION 3.7)
include(../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Libraries.GHA)
Djinn_Project_AddDependencies(
        Libraries.COM
        Libraries.Host
        Libraries.Core
        Libraries.Platform
)
Djinn_Project_SearchSources(Include Source)
Djinn_Project_Install(STATIC)

# Finalize
Djinn_Finalize()