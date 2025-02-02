get_property(DJINN_CONFIGURED GLOBAL PROPERTY DJINN_CONFIGURED)

# Already configured?
if (NOT DJINN_CONFIGURED)
    # Root must exist in current directory
    if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/../.root")
        message(FATAL_ERROR "BuildSystem misplaced, expected directory '.../Build' but got '${CMAKE_CURRENT_LIST_DIR}'")
    endif ()

    # Root
    set(DJINN_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." CACHE INTERNAL "FULL_PATH")

    # Clear previously cached djinn variables
    get_cmake_property(CACHED_VARIABLES VARIABLES)
    foreach (Var ${CACHED_VARIABLES})
        string(FIND "${Var}" "Djinn." INDEX)
        if (${INDEX} EQUAL 0)
            unset(${Var} CACHE)
        endif ()
    endforeach ()
    message(STATUS "Cleaned up previous cache")

    # Mark as configured
    set_property(GLOBAL PROPERTY DJINN_CONFIGURED "ON")

    # Configure this
    include(${DJINN_ROOT}/Build/Configure.cmake)

    # Configure sub systems
    include(${DJINN_PATH_BUILD}/CMake/Configure.cmake)
    include(${DJINN_PATH_BUILD}/Platform/Configure.cmake)
    include(${DJINN_PATH_BUILD}/Compiler/Configure.cmake)
    include(${DJINN_PATH_BUILD}/Packages/Configure.cmake)
    include(${DJINN_PATH_BUILD}/Project/Configure.cmake)
    include(${DJINN_PATH_BUILD}/Cxx/Configure.cmake)

    # Done
    Djinn_LogEvent("Build-System ready")
endif ()

# Finalize build system
macro(Djinn_Finalize)
    if ("${CMAKE_CURRENT_LIST_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
        Djinn_Package_Finalize()
        Djinn_Package_SearchAll()
        Djinn_Package_ResolveAll()
        Djinn_Project_ConfigureAll()
        Djinn_LogEvent("Generation complete!")
    endif ()
endmacro(Djinn_Finalize)