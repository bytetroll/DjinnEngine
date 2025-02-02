# Search for package output
macro(Djinn_SearchPackageOutput FULL_PATH COUNT OUT)
    # TODO: Hack
    if (${COUNT} EQUAL 24)
        Djinn_LogError("DjinnEngine path too deep")
        return()
    endif ()

    # Attempt current
    if (EXISTS "${FULL_PATH}/Packages")
        set(${OUT} "${FULL_PATH}/Packages")
    else ()
        # ...
        string(FIND "${FULL_PATH}" "/" _INDEX REVERSE)

        # Empty?
        if (_INDEX EQUAL -1)
            message(FATAL_ERROR "Package output directory could not be found, a Packages folder must reside somewhere down the folder chain")
        else ()
            string(SUBSTRING "${FULL_PATH}" 0 ${_INDEX} NEW_PATH)

            # ...
            math(EXPR NEW_COUNT "${COUNT} + 1")
            Djinn_SearchPackageOutput(${NEW_PATH} ${NEW_COUNT} ${OUT})
        endif ()
    endif ()
endmacro(Djinn_SearchPackageOutput)

# Create package
# @Name: Name of the package
function(Djinn_CreatePackage NAME OUT)
    # Must not exist
    if (NOT "${Djinn.Packages.${NAME}.Name}" STREQUAL "")
        Djinn_LogError("Package with name '${NAME}' already exists")
    endif ()

    # Append
    list(APPEND Djinn.Packages ${NAME})
    Djinn_SetGlobal(Djinn.Packages "${Djinn.Packages}")

    # Find .Package directory
    Djinn_SearchPackageOutput("${CMAKE_CURRENT_BINARY_DIR}" 0 DIR)

    # Initialize variables
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Name" "${NAME}")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Dir" "${CMAKE_CURRENT_LIST_DIR}")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.IncludeDirs" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.ExternIncludeDirs" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Dependencies" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.AllDependencies" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.LinkDirs" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Links" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.ExternLinks" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.ExternBinaries" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Sources" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Installed" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.InstalledType" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Defines" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.LoadBefore" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.LoadAfter" "")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.Desc" "<No description supplied>")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.PackageDir" "${DIR}")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.SearchPaths" "${DJINN_PATH_BUILD_CMAKE_CACHE}/Packages" "${DIR}")
    Djinn_SetGlobal("Djinn.Packages.${NAME}.BuildPath" "")

    # Scoped global
    Djinn_SetGlobal("Djinn.Packages.glocal" "Djinn.Packages.${NAME}")

    # Out?
    if (NOT "${OUT}" STREQUAL "")
        Djinn_SetGlobal(${OUT} "Djinn.Packages.${NAME}")
    endif ()
endfunction(Djinn_CreatePackage)

# Add load orders
function(Djinn_Package_LoadOrder MODE)
    if ("${MODE}" STREQUAL "AFTER")
        list(APPEND ${Djinn.Packages.glocal}.LoadAfter ${ARGN})
        Djinn_SetGlobal(${Djinn.Packages.glocal}.LoadAfter ${${Djinn.Packages.glocal}.LoadAfter})
    elseif ("${MODE}" STREQUAL "BEFORE")
        list(APPEND ${Djinn.Packages.glocal}.LoadBefore ${ARGN})
        Djinn_SetGlobal(${Djinn.Packages.glocal}.LoadBefore ${${Djinn.Packages.glocal}.LoadBefore})
    else ()
        Djinn_LogError("Invalid load order mode, must be either AFTER or BEFORE")
    endif ()
endfunction()

# Add include directories
function(Djinn_Package_AddIncludeDirs)
    list(GET ARGN 0 OptionalOption)
    if (NOT "${OptionalOption}" STREQUAL "GLOBAL")
        Djinn_Prefix("${ARGN}" Dirs "${CMAKE_CURRENT_LIST_DIR}/")
    else ()
        list(REMOVE_AT ARGN 0)
        set(Dirs ${ARGN})
    endif ()
    list(APPEND ${Djinn.Packages.glocal}.IncludeDirs ${Dirs})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.IncludeDirs ${${Djinn.Packages.glocal}.IncludeDirs})
endfunction()

# Add extern include directories
function(Djinn_Package_AddExternIncludeDirs)
    list(GET ARGN 0 OptionalOption)
    if (NOT "${OptionalOption}" STREQUAL "GLOBAL")
        Djinn_Prefix("${ARGN}" Dirs "${CMAKE_CURRENT_LIST_DIR}/")
    else ()
        list(REMOVE_AT ARGN 0)
        set(Dirs ${ARGN})
    endif ()
    list(APPEND ${Djinn.Packages.glocal}.ExternIncludeDirs ${Dirs})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.ExternIncludeDirs ${${Djinn.Packages.glocal}.ExternIncludeDirs})
endfunction()

# Add extern include directories
function(Djinn_Package_AddExternLinks)
    list(APPEND ${Djinn.Packages.glocal}.ExternLinks ${ARGN})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.ExternLinks ${${Djinn.Packages.glocal}.ExternLinks})
endfunction()

# Add extern binaries
function(Djinn_Package_AddExternBinaries)
    list(APPEND ${Djinn.Packages.glocal}.ExternBinaries ${ARGN})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.ExternBinaries ${${Djinn.Packages.glocal}.ExternBinaries})
endfunction()

# Add define
function(Djinn_Package_AddDefines)
    list(APPEND ${Djinn.Packages.glocal}.Defines ${ARGN})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.Defines ${${Djinn.Packages.glocal}.Defines})
endfunction()

# Add library directories
function(Djinn_Package_AddLinkDirs)
    list(APPEND ${Djinn.Packages.glocal}.LinkDirs ${ARGN})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.LinkDirs ${${Djinn.Packages.glocal}.LinkDirs})
endfunction()

# Add package dependencies
function(Djinn_Package_AddDependencies)
    list(APPEND ${Djinn.Packages.glocal}.Dependencies ${ARGN})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.Dependencies ${${Djinn.Packages.glocal}.Dependencies})
endfunction(Djinn_Package_AddDependencies)

# Add links
function(Djinn_Package_AddLinks)
    list(APPEND ${Djinn.Packages.glocal}.Links ${ARGN})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.Links ${${Djinn.Packages.glocal}.Links})
endfunction()

# Add installed
function(Djinn_Package_SetInstalled Installed Type)
    Djinn_SetGlobal(${Djinn.Packages.glocal}.Installed ${Installed})
    Djinn_SetGlobal(${Djinn.Packages.glocal}.InstalledType ${Type})
endfunction()

# Set description
function(Djinn_Package_SetDesc DESC)
    Djinn_SetGlobal(${Djinn.Packages.glocal}.Desc "${DESC}")
endfunction()

# Set build path
function(Djinn_Package_SetBuildPath PATH)
    Djinn_SetGlobal(${Djinn.Packages.glocal}.BuildPath "${PATH}")
endfunction()

# Finalize packages
function(Djinn_Package_Finalize)
    foreach (Package ${Djinn.Packages})
        # Sanitize definitions
        string(REPLACE "\"" "\\\"" Defines "${Djinn.Packages.${Package}.Defines}")

        # Write config
        Djinn_ConfigWriteStart(Config)
        Djinn_ConfigWrite(Config Name ${Djinn.Packages.${Package}.Name})
        Djinn_ConfigWrite(Config Dir ${Djinn.Packages.${Package}.Dir})
        Djinn_ConfigWrite(Config IncludeDirs ${Djinn.Packages.${Package}.IncludeDirs})
        Djinn_ConfigWrite(Config ExternIncludeDirs ${Djinn.Packages.${Package}.ExternIncludeDirs})
        Djinn_ConfigWrite(Config Dependencies ${Djinn.Packages.${Package}.Dependencies})
        Djinn_ConfigWrite(Config LinkDirs ${Djinn.Packages.${Package}.LinkDirs})
        Djinn_ConfigWrite(Config Links ${Djinn.Packages.${Package}.Links})
        Djinn_ConfigWrite(Config ExternLinks ${Djinn.Packages.${Package}.ExternLinks})
        Djinn_ConfigWrite(Config ExternBinaries ${Djinn.Packages.${Package}.ExternBinaries})
        Djinn_ConfigWrite(Config Sources ${Djinn.Packages.${Package}.Sources})
        Djinn_ConfigWrite(Config Installed ${Djinn.Packages.${Package}.Installed})
        Djinn_ConfigWrite(Config InstalledType ${Djinn.Packages.${Package}.InstalledType})
        Djinn_ConfigWrite(Config Defines ${Djinn.Packages.${Package}.Defines})
        Djinn_ConfigWrite(Config LoadBefore ${Djinn.Packages.${Package}.LoadBefore})
        Djinn_ConfigWrite(Config LoadAfter ${Djinn.Packages.${Package}.LoadAfter})
        Djinn_ConfigWrite(Config Desc ${Djinn.Packages.${Package}.Desc})
        Djinn_ConfigWrite(Config SearchPaths ${Djinn.Packages.${Package}.SearchPaths})
        Djinn_ConfigWrite(Config BuildPath ${Djinn.Packages.${Package}.BuildPath})
        Djinn_ConfigWriteEnd(Config "${Djinn.Packages.${Package}.PackageDir}/${Djinn.Packages.${Package}.Name}.${CMAKE_BUILD_TYPE}.pck")
    endforeach ()
endfunction()
