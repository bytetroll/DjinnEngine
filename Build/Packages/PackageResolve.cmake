# Resolve package
function(Djinn_Package_Resolve NAME)
    Djinn_SetGlobal(Djinn.Packages.${NAME}.AllDependencies ${Djinn.Packages.${NAME}.Dependencies})

    # Resolve packages
    foreach (Package ${Djinn.Packages.${NAME}.Dependencies})
        # Exists?
        if (NOT Djinn.Packages.${Package}.Name)
            Djinn_LogError("Could not find package '${Package}', package may be corrupted")
        endif ()

        # Add defines
        list(APPEND Djinn.Packages.${NAME}.AllDependencies ${Djinn.Packages.${Package}.AllDependencies})
        list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.AllDependencies)
        Djinn_SetGlobal(Djinn.Packages.${NAME}.AllDependencies ${Djinn.Packages.${NAME}.AllDependencies})

        # Add defines
        list(APPEND Djinn.Packages.${NAME}.Defines ${Djinn.Packages.${Package}.Defines})
        list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.Defines)
        Djinn_SetGlobal(Djinn.Packages.${NAME}.Defines ${Djinn.Packages.${NAME}.Defines})

        # Add include dirs
        list(APPEND Djinn.Packages.${NAME}.IncludeDirs ${Djinn.Packages.${Package}.IncludeDirs})
        list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.IncludeDirs)
        Djinn_SetGlobal(Djinn.Packages.${NAME}.IncludeDirs ${Djinn.Packages.${NAME}.IncludeDirs})

        # Add extern include dirs
        list(APPEND Djinn.Packages.${NAME}.ExternIncludeDirs ${Djinn.Packages.${Package}.ExternIncludeDirs})
        list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.ExternIncludeDirs)
        Djinn_SetGlobal(Djinn.Packages.${NAME}.ExternIncludeDirs ${Djinn.Packages.${NAME}.ExternIncludeDirs})

        # Add link dirs
        list(APPEND Djinn.Packages.${NAME}.LinkDirs ${Djinn.Packages.${Package}.LinkDirs})
        list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.LinkDirs)
        Djinn_SetGlobal(Djinn.Packages.${NAME}.LinkDirs ${Djinn.Packages.${NAME}.LinkDirs})

        # Add libs
        list(APPEND Djinn.Packages.${NAME}.Links ${Djinn.Packages.${Package}.Links})
        if (NOT ${Djinn.Packages.${Package}.InstalledType} STREQUAL "EXEC")
            list(APPEND Djinn.Packages.${NAME}.Links ${Djinn.Packages.${Package}.Installed})
        endif ()
        list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.Links)
        Djinn_SetGlobal(Djinn.Packages.${NAME}.Links ${Djinn.Packages.${NAME}.Links})

        # Add extern libs
        #list(APPEND Djinn.Packages.${NAME}.ExternLinks ${Djinn.Packages.${Package}.ExternLinks})
        #list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.ExternLinks)
        #Djinn_SetGlobal(Djinn.Packages.${NAME}.ExternLinks ${Djinn.Packages.${NAME}.ExternLinks})

        # Add extern binaries
        list(APPEND Djinn.Packages.${NAME}.ExternBinaries ${Djinn.Packages.${Package}.ExternBinaries})
        list(REMOVE_DUPLICATES Djinn.Packages.${NAME}.ExternBinaries)
        Djinn_SetGlobal(Djinn.Packages.${NAME}.ExternBinaries ${Djinn.Packages.${NAME}.ExternBinaries})
    endforeach ()
endfunction(Djinn_Package_Resolve)

# Resolver pass
macro(Djinn_Package_ResolverPass PACKAGES OUT_DEPTH)
    unset(DidResolve)
    foreach (Package ${${PACKAGES}})
        # Foreach dependency
        unset(Invalid)
        foreach (Dep ${Djinn.Packages.${Package}.Dependencies})
            # Does primary list contain this dependency?
            list(FIND ${PACKAGES} ${Dep} Index)
            if (NOT Index EQUAL -1)
                set(Invalid "ON")
                break()
            endif ()
        endforeach ()

        # Invalid?
        if (Invalid)
            continue()
        endif ()

        # Resolve this package
        Djinn_Package_Resolve(${Package})
        Djinn_LogInfo("Resolved package: ${Package}")
        set(DidResolve "ON")

        # Remove
        list(REMOVE_ITEM ${PACKAGES} ${Package})
    endforeach ()

    # Resolved any?
    if (NOT DidResolve)
        # No package resolved, must be either empty or circular dependency
        list(LENGTH ${PACKAGES} Length)
        if (NOT Length EQUAL 0)
            Djinn_LogError("Circular or invalid dependency detected")
        endif ()
    else ()
        # Next pass
        math(EXPR ${OUT_DEPTH} "${${OUT_DEPTH}} + 1")
        Djinn_Package_ResolverPass(${PACKAGES} ${OUT_DEPTH})
    endif ()
endmacro(Djinn_Package_ResolverPass)

# Resolve all packages
function(Djinn_Package_ResolveAll)
    Djinn_LogEvent("Resolving packages...")

    # Copy list
    set(Packages ${Djinn.Packages})

    # Resolve
    set(Depth 0)
    Djinn_Package_ResolverPass(Packages Depth)

    # ...
    Djinn_LogInfo("Packages resolved, ${Depth} passes!")
endfunction()

# Search all packages
function(Djinn_Package_SearchAll)
    Djinn_LogEvent("Appending external packages [[${CMAKE_CURRENT_LIST_DIR}]]")

    # While there are packages to include
    set(_ResolvedAny ON)
    while (_ResolvedAny)
        unset(_ResolvedAny)

        set(Packages ${Djinn.Packages})

        # Foreach packageSearchPaths
        foreach (Package ${Djinn.Packages})
            foreach (Dep ${Djinn.Packages.${Package}.Dependencies})

                list(FIND Packages ${Dep} Index)
                if (Index EQUAL -1)
                    # Attempt to find dependency
                    unset(_PackagePath)
                    foreach (Path ${Djinn.Packages.${Package}.SearchPaths})
                        if (EXISTS "${Path}/${Dep}.${CMAKE_BUILD_TYPE}.pck")
                            # Read configuration
                            Djinn_ConfigRead("${Path}/${Dep}.${CMAKE_BUILD_TYPE}.pck" "Djinn.Packages.${Dep}." ON)

                            # Append
                            list(APPEND Packages ${Djinn.Packages.${Dep}.Name})
                            set(_PackagePath ${Path}/${Dep}.pck)
                            break()
                        endif ()
                    endforeach ()

                    # Found?
                    if (NOT _PackagePath)
                        Djinn_LogError("Could not find package '${Dep}' requested by '${Package}', search paths: \"${Djinn.Packages.${Package}.SearchPaths}\". Package may not be configured.")
                    endif ()

                    # Load
                    Djinn_LogInfo("External package found: ${Dep}")
                    # include("${_PackagePath}/CMakeLists.txt")

                    # Flag
                    set(_ResolvedAny ON)
                endif ()
            endforeach ()
        endforeach ()

        # Set
        Djinn_SetGlobal(Djinn.Packages "${Packages}")
    endwhile ()
endfunction()