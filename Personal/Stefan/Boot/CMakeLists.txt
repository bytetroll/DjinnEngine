cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../Build/BuildSystem.cmake)

# This is an extremely simple project setup
# This does not reflect the full capabilities of the buildsystem, if you have more questions just annoy me.

# Create project
# This sets up basic parameters required for all projects
Djinn_CreateProject(Personal.Stefan.Boot)

# Add buildsystem dependencies
# ? Note that you can add entire "directories" of plugins
Djinn_Project_AddDependencies(
        # Common set of containers
        Libraries.Core

        # Registry functionality
        Libraries.COM

        # Example: Math
        Libraries.VectorMath

        # Example: Asynchronous
        Plugins.Async

        # Example: Filesystem
        Plugins.Base

        # Example: UI
        Plugins.UI        # Pull in common sets of UI functionality
        Plugins.Graphics  # Pull in common sets of graphical functionality, this is really over-pulling but w/e
        Plugins.GHA       # Pull in the API backends like Vulkan
)

# Next up you tell the buildsystem where to find the source code
# ? By default if you are a library the Include/ directory is always exported as an include
# ? This can also be platform specific, if you type:
#       Djinn_Project_SearchSources(
#           Source
#           WINDOWS WindowsOnlySource
#           LINUX   LinuxOnlySource
#       )
#       It'll only pull those folders in if on the appropriate platform.
Djinn_Project_SearchSources(Source)

# The build path is just the output
# By default it uses the root Pipeline folder, but since we usually load all neighbouring (to the .exe) Plugins we want a custom folder
Djinn_Project_SetBuildPath(Pipeline)

# Finally, all projects need to be installed
# They are one of VIRTUAL, DYNAMIC, STATIC or EXEC
Djinn_Project_Install(EXEC)

# Finalize
# A bit of a quirk, but this setup ensures that you can open just THIS project
# So you dont have to let CLion (or whatever IDE) load everything... Very neat!
Djinn_Finalize()