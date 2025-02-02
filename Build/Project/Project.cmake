# Create project
# @Name: Name of the project
function(Djinn_CreateProject NAME)
    # Must not exist
    if (NOT "${Djinn.Projects.${NAME}.Name}" STREQUAL "")
        Djinn_LogError("Project with name '${NAME}' already exists")
    endif ()
    Djinn_LogInfo("Creating project ${NAME}")

    # Append
    list(APPEND Djinn.Projects ${NAME})
    Djinn_SetGlobal(Djinn.Projects "${Djinn.Projects}")

    # Initialize variables
    Djinn_SetGlobal("Djinn.Projects.${NAME}.Name" "${NAME}")
    Djinn_SetGlobal("Djinn.Projects.${NAME}.InstallName" "${NAME}")
    Djinn_SetGlobal("Djinn.Projects.${NAME}.Deps" "")
    Djinn_SetGlobal("Djinn.Projects.${NAME}.Path" "${CMAKE_CURRENT_LIST_DIR}")

    # Create preprocessor name
    string(REPLACE "." "_" SAFE_DEFINE "${NAME}")
    string(TOUPPER "${SAFE_DEFINE}" SAFE_DEFINE)
    Djinn_SetGlobal("Djinn.Projects.${NAME}.PreprocessorName" "${SAFE_DEFINE}")

    # Create package
    Djinn_CreatePackage(${NAME} "Djinn.Projects.${NAME}.Package")
    Djinn_Package_SetBuildPath("${DJINN_PATH_PIPELINE}/")

    # Scoped global
    Djinn_SetGlobal("Djinn.Projects.glocal" "Djinn.Projects.${NAME}")

    # Create project
    project("${Djinn.Projects.${NAME}.Name}" CXX)

    # Default flags
    Djinn_Cxx_SetFlags(Djinn.Projects.${NAME} ${CMAKE_BUILD_TYPE})

    # Get platform include path
    Djinn_Platform_IncludePath(PlatformIncludePath)

    # All projects are defaulted to /Include/ & /Include<PLATFORM>/
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/Include")
        Djinn_Project_AddIncludeDirs(Include)
    endif ()
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${PlatformIncludePath}")
        Djinn_Project_AddIncludeDirs("${PlatformIncludePath}")
    endif ()

    # All projects are defaulted to /Pipeline/
    Djinn_Project_AddLinkDirs(${DJINN_PATH_PIPELINE})

    # Default dll import
    Djinn_Project_AddDefines(DJINN_EXPORT_${Djinn.Projects.${NAME}.PreprocessorName}=${Djinn.Cxx.DLLImport})
    Djinn_Project_AddDefines("DJINN_C_EXPORT_${Djinn.Projects.${NAME}.PreprocessorName}=extern \"C\" ${Djinn.Cxx.DLLImport}")
endfunction(Djinn_CreateProject)

function(Djinn_CreateTest PROJECT)
    Djinn_CreateProject(${PROJECT}.Test)
    Djinn_Project_AddDependencies(${PROJECT})
    Djinn_Project_SearchSources(${ARGN})
    Djinn_Project_Install(EXEC)
endfunction()

function(Djinn_Project_SetAttribute NAME)
    Djinn_SetGlobal("${Djinn.Projects.glocal}.Attr${NAME}")
endfunction()

function(Djinn_Project_SetBuildMode MODE)
    # Default flags
    Djinn_Cxx_SetFlags(${Djinn.Projects.glocal} ${MODE})
endfunction()

# Add define
function(Djinn_Project_AddDefines)
    Djinn_Package_AddDefines(${ARGN})
endfunction()

# Set build name
function(Djinn_Project_SetBuildName NAME)
    Djinn_SetGlobal("${Djinn.Projects.glocal}.InstallName" "${NAME}")
endfunction()

# Add include directories
function(Djinn_Project_AddIncludeDirs)
    Djinn_Package_AddIncludeDirs(${ARGN})
endfunction()

# Add extern include directories
function(Djinn_Project_AddExternIncludeDirs)
    Djinn_Package_AddExternIncludeDirs(${ARGN})
endfunction()

# Add extern links
function(Djinn_Project_AddExternLinks)
    Djinn_Package_AddExternLinks(${ARGN})
endfunction()

# Add extern binaries
function(Djinn_Project_AddExternBinaries)
    Djinn_Package_AddExternBinaries(${ARGN})
endfunction()

# Add library directories
function(Djinn_Project_AddLinkDirs)
    Djinn_Package_AddLinkDirs(${ARGN})
endfunction()

# Add links
function(Djinn_Project_AddLinks)
    Djinn_Package_AddLinks(${ARGN})
endfunction()

# Set sources
function(Djinn_Project_SetSources)
    # To absolute
    Djinn_Prefix("${ARGN}" Absolute "${CMAKE_CURRENT_LIST_DIR}/")

    # Add
    Djinn_SetGlobal("${Djinn.Projects.glocal}.Sources" "${${Djinn.Projects.glocal}.Sources};${Absolute}")
endfunction()

# Set project output path
function(Djinn_Project_SetBuildPath PATH)
    # Path must end with \/
    if (NOT "${PATH}" MATCHES "[\\\\\\/]$")
        set(PATH "${PATH}/")
    endif ()

    # IS_ABSOLUTE is somewhat unreliable
    if ("${PATH}" MATCHES "^[A-z]:[\\\\\\/]|^[\\\\\\/]")
        Djinn_Package_SetBuildPath("${PATH}${CMAKE_BUILD_TYPE}/")
    else ()
        Djinn_Package_SetBuildPath("${${Djinn.Projects.glocal}.Path}/${PATH}${CMAKE_BUILD_TYPE}/")
    endif ()

    if (NOT EXISTS "${${Djinn.Packages.glocal}.BuildPath}")
        Djinn_LogInfo("Created output directory ${${Djinn.Packages.glocal}.BuildPath}")
        file(MAKE_DIRECTORY "${${Djinn.Packages.glocal}.BuildPath}")
    endif ()
endfunction()

# Set sources by searching inside directories given
# Can specify platform specific by:
# WINDOWS Path1 Path2 PathN
function(Djinn_Project_SearchSources)
    # Parse arguments
    cmake_parse_arguments(ARG "" "" "${Djinn.Platforms.Uppercase}" ${ARGN})

    # Compose folders
    set(Folders ${ARG_UNPARSED_ARGUMENTS})
    foreach (Platform ${Djinn.Platforms.Uppercase})
        if (${Platform} STREQUAL ${Djinn.Platform.Uppercase})
            list(APPEND Folders ${ARG_${Platform}})
        endif ()
    endforeach ()

    # To absolute
    Djinn_Prefix("${Folders}" AbsoluteFolders "${CMAKE_CURRENT_LIST_DIR}/")

    # Apply
    Djinn_Postfix("${AbsoluteFolders}" Headers "/*.h")
    Djinn_Postfix("${AbsoluteFolders}" Sources "/*.cpp")
    Djinn_Postfix("${AbsoluteFolders}" Asm "/*.asm")
    Djinn_Postfix("${AbsoluteFolders}" AsmS "/*.S")
    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${Headers} ${Sources} ${Asm} ${AsmS})
    Djinn_SetGlobal("${Djinn.Projects.glocal}.Sources" "${${Djinn.Projects.glocal}.Sources};${SOURCES}")
endfunction()

function(Djinn_Project_LoadOrder MODE)
    Djinn_Package_LoadOrder(${MODE} ${ARGN})
endfunction()

# Add project dependencies
function(Djinn_Project_AddDependencies)
    Djinn_Package_AddDependencies(${ARGN})
endfunction(Djinn_Project_AddDependencies)

# Install project
# Options:
#   VIRTUAL : Does not produce an output and is instead handled internally
#   DYNAMIC : Produces a dynamic linked library
#   STATIC  : Produces a staticically linked library
#   EXEC    : Executable
function(Djinn_Project_Install TYPE)
    set(HasSource OFF)
    foreach (Item ${${Djinn.Projects.glocal}.Sources})
        if (NOT "${Item}" MATCHES "(.h$)|(.inl$)|(.hpp$)")
            set(HasSource ON)
            break()
        endif()
    endforeach()

    if (NOT ${HasSource})
        set(TYPE VIRTUAL)
        Djinn_LogInfo("Downgraded ${${Djinn.Projects.glocal}.Name} to virtual as there are no source files")
    endif()

    Djinn_SetGlobal(${Djinn.Projects.glocal}.InstallType ${TYPE})

    # All dynamic libraries depend on the header generator and reflection
    if ("${TYPE}" STREQUAL "DYNAMIC")
        Djinn_Project_AddDependencies(Libraries.Reflection)
        Djinn_Project_AddDependencies(Tools.DHG)
    endif ()

    # Set installed
    if (NOT "${TYPE}" STREQUAL "VIRTUAL")
        Djinn_Package_SetInstalled("${${Djinn.Projects.glocal}.Name}" "${TYPE}")
    endif ()
endfunction(Djinn_Project_Install)

# Set project description
function(Djinn_Project_SetDesc DESC)
    Djinn_Package_SetDesc(${DESC})
endfunction(Djinn_Project_SetDesc)