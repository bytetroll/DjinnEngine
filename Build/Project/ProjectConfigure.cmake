# Configure all packages
function(Djinn_Project_ConfigureAll)
    Djinn_LogEvent("Configuring projects...")

    # Foreach project
    Djinn_LogInfo("!! Ignore Warnings !!")
    foreach (Project ${Djinn.Projects})
        set(ArchiveDir "${${Djinn.Projects.${Project}.Package}.BuildPath}")
        set(LibraryDir "${${Djinn.Projects.${Project}.Package}.BuildPath}")
        set(RuntimeDir "${${Djinn.Projects.${Project}.Package}.BuildPath}")
        set(PDBDir     "${${Djinn.Projects.${Project}.Package}.BuildPath}")

        # DHG file cutting
        string(LENGTH "${Djinn.Projects.${Project}.Path}/" BaseLength)

        # Application Interface Path
        set(APIPath "${${Djinn.Projects.${Project}.Name}_BINARY_DIR}")

        # Virtuals do not have installed
        if (NOT "${Djinn.Projects.${Project}.InstallType}" STREQUAL "VIRTUAL")
            # Exec DHG support?
            unset(ExecDHG)
            if ("${Djinn.Projects.${Project}.InstallType}" STREQUAL "EXEC" AND "Libraries.Reflection" IN_LIST ${Djinn.Projects.${Project}.Package}.AllDependencies)
                set(ExecDHG ON)
            endif ()

            # DHG for dynamic of executable targets
            set(DHGAPIPath "${APIPath}/${Djinn.Projects.${Project}.Name}.DHG.cpp")
            if ("${Djinn.Projects.${Project}.InstallType}" STREQUAL "DYNAMIC" OR ExecDHG)
                # Write
                if (NOT EXISTS "${DHGAPIPath}")
                    Djinn_WriteFile("${DHGAPIPath}" "\#error Not generated")
                endif ()
            endif ()

            # DHG Options
            set(DHGOptions "")
            if (WIN32)
                set(DHGOptions "-fconstexpr-depth=1024 -Os -w -s -g0 -fno-var-tracking -ffunction-sections -fdata-sections -Wa,-mbig-obj -fno-exceptions -DDJINN_THROW=;")
            else ()
                set(DHGOptions "-fconstexpr-depth=1024 -Os -w -s -g0 -fno-var-tracking -ffunction-sections -fdata-sections")
            endif ()

            # Find correct type and install final target
            if ("${Djinn.Projects.${Project}.InstallType}" STREQUAL "DYNAMIC")
                set(PluginAPIPath "${APIPath}/Plugin.cpp")

                # Plugin API Helper
                # TODO: Refactor this, move to a generated like format
                set(PluginAPI "/* Generated file */\n")
                set(PluginAPI "${PluginAPI}#include <Core/Common.h>\n")
                set(PluginAPI "${PluginAPI}#include <Host/IRegistry.h>\n")
                set(PluginAPI "${PluginAPI}#include <Host/PluginAPI.h>\n")
                set(PluginAPI "${PluginAPI}#include <Platform/Detail/LibraryMemoryHooks.h>\n\n")
                set(PluginAPI "${PluginAPI}DJINN_C_EXPORT_${Djinn.Projects.${Project}.PreprocessorName} Djinn::COM::Result DJINN_PLUGINAPI_INFO(Djinn::Host::PluginInfo& out) {\n")
                set(PluginAPI "${PluginAPI}\tout.Description(\"${${Djinn.Projects.${Project}.Package}.Desc}\");\n")
                set(PluginAPI "${PluginAPI}\tout.Name(\"${Djinn.Projects.${Project}.Name}\");\n")
                foreach (Dep ${${Djinn.Projects.${Project}.Package}.Dependencies})
                    if ("${Djinn.Projects.${Dep}.InstallType}" STREQUAL "DYNAMIC" AND NOT "${Dep}" IN_LIST ${Djinn.Projects.${Project}.Package}.LoadBefore)
                        set(PluginAPI "${PluginAPI}\tout.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, \"${Dep}\");\n")
                    endif ()
                endforeach ()
                foreach (Dep ${${Djinn.Projects.${Project}.Package}.LoadAfter})
                    set(PluginAPI "${PluginAPI}\tout.AddDependency(Djinn::Host::PluginDependencyMode::eAfter, \"${Dep}\");\n")
                endforeach ()
                foreach (Dep ${${Djinn.Projects.${Project}.Package}.LoadBefore})
                    set(PluginAPI "${PluginAPI}\tout.AddDependency(Djinn::Host::PluginDependencyMode::eBefore, \"${Dep}\");\n")
                endforeach ()
                set(PluginAPI "${PluginAPI}\treturn Djinn::COM::kOK;\n")
                set(PluginAPI "${PluginAPI}}\n\n/* */")

                # Write
                Djinn_WriteFile("${PluginAPIPath}" "${PluginAPI}")

                # Custom DHG options
                set_source_files_properties(${DHGAPIPath} PROPERTIES COMPILE_FLAGS "${DHGOptions}")

                # Add lib
                add_library("${Djinn.Projects.${Project}.Name}" SHARED ${Djinn.Projects.${Project}.Sources} ${PluginAPIPath} ${DHGAPIPath})

                # Remove symbols
                Djinn_GetObjectPath(${Djinn.Projects.${Project}.Name} ${DHGAPIPath} DHGObjPath)

                if (${Djinn.Compiler} STREQUAL GNU)
                    add_custom_command(
                            TARGET "${Djinn.Projects.${Project}.Name}" PRE_LINK
                            COMMAND ${CMAKE_STRIP} --strip-unneeded -w -R *.pdata* -R *.xdata* "${DHGObjPath}"
                    )
                endif()

                # Configure ends
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES PREFIX "")
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES SUFFIX ".dll")
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES IMPORT_PREFIX "")
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES IMPORT_SUFFIX ".lib")
            elseif ("${Djinn.Projects.${Project}.InstallType}" STREQUAL "STATIC")
                add_library("${Djinn.Projects.${Project}.Name}" STATIC ${Djinn.Projects.${Project}.Sources})

                # Configure ends
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES PREFIX "")
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES SUFFIX ".lib")
            elseif ("${Djinn.Projects.${Project}.InstallType}" STREQUAL "EXEC")
                if (ExecDHG)
                    # Custom DHG options
                    set_source_files_properties(${DHGAPIPath} PROPERTIES COMPILE_FLAGS "${DHGOptions}")

                    # ...
                    add_executable("${Djinn.Projects.${Project}.Name}" ${Djinn.Projects.${Project}.Sources} ${DHGAPIPath})

                    # Remove symbols
                    if (${Djinn.Compiler} STREQUAL GNU)
                        Djinn_GetObjectPath(${Djinn.Projects.${Project}.Name} ${DHGAPIPath} DHGObjPath)
                        add_custom_command(
                                TARGET "${Djinn.Projects.${Project}.Name}" PRE_LINK
                                COMMAND ${CMAKE_STRIP} --strip-unneeded -w -R *.pdata* -R *.xdata* "${DHGObjPath}"
                        )
                    endif()
                else ()
                    add_executable("${Djinn.Projects.${Project}.Name}" ${Djinn.Projects.${Project}.Sources})
                endif ()

                # Configure ends
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES PREFIX "")
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES SUFFIX ".exe")

                # Custom name
                set(CustomName "Z_Mirror.${Djinn.Projects.${Project}.Name}")

                # Deduce actual dependencies
                set(ActualDependencies "")
                foreach (Dep ${${Djinn.Projects.${Project}.Package}.AllDependencies})
                endforeach ()

                # Prepare external binaries
                set(ExternBinaries ${${Djinn.Projects.${Project}.Package}.ExternBinaries})
                foreach (Dep ${${Djinn.Projects.${Project}.Package}.AllDependencies})
                    if ("${Djinn.Packages.${Dep}.InstalledType}" STREQUAL "DYNAMIC")
                        list(APPEND ExternBinaries "${Djinn.Packages.${Dep}.BuildPath}${Dep}.dll")
                    endif ()

                    if (${Dep} IN_LIST Djinn.Projects)
                        list(APPEND ActualDependencies ${Dep})
                    endif ()
                endforeach ()
                set(ExternBinaries "${ExternBinaries}")
                string(REPLACE ";" " '" ExternBinaries "${ExternBinaries}")

                # Cmake is a bit odd on string pasting as arguments, so a workaround is needed
                if (NOT "${ActualDependencies}" STREQUAL "")
                    add_custom_target(
                            "${CustomName}"
                            ALL COMMAND ${CMAKE_COMMAND} -DPROJECT="${Djinn.Projects.${Project}.Path}" -DBINARIES="${ExternBinaries}" -DPIPELINE="${DJINN_PATH_PIPELINE}" -DOUTPUT="${${Djinn.Projects.${Project}.Package}.BuildPath}" -P "${DJINN_PATH_BUILD}/Utils/PostExeBuild.cmake"
                            DEPENDS ${ActualDependencies}
                    )
                else ()
                    add_custom_target(
                            "${CustomName}"
                            ALL COMMAND ${CMAKE_COMMAND} -DPROJECT="${Djinn.Projects.${Project}.Path}" -DBINARIES="${ExternBinaries}" -DPIPELINE="${DJINN_PATH_PIPELINE}" -DOUTPUT="${${Djinn.Projects.${Project}.Package}.BuildPath}" -P "${DJINN_PATH_BUILD}/Utils/PostExeBuild.cmake"
                    )
                endif ()

                # Configure post processing dependencies
                add_dependencies("${Djinn.Projects.${Project}.Name}" "${CustomName}")
            else ()
                Djinn_LogError("Could not install project ${Djinn.Projects.${Project}.Name}, unknown option '${Djinn.Projects.${Project}.InstallType}'")
            endif ()

            if (EXISTS "${Djinn.Projects.${Project}.Path}/Runtime")
                # Runtime virtual file system
                file(WRITE "${DJINN_PATH_PIPELINE}/Runtime/${Project}.dvrf" "Runtime:${Djinn.Projects.${Project}.Path}/Runtime")
            endif ()

            # Install name
            set_target_properties(${Djinn.Projects.${Project}.Name} PROPERTIES OUTPUT_NAME "${Djinn.Projects.${Project}.InstallName}")

            # Set flags
            #set_target_properties(
            #        "${Djinn.Projects.${Project}.Name}"
            #        PROPERTIES
            #        COMPILE_FLAGS "${Djinn.Projects.${Project}.CxxFlags} ${Djinn.Projects.${Project}.CFlags}"
            #        LINK_FLAGS "${Djinn.Projects.${Project}.LinkerFlags}"
            #)
            set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES
                    CXX_STANDARD 17
                    CXX_STANDARD_REQUIRED YES
                    )
            #target_compile_features("${Djinn.Projects.${Project}.Name}" PUBLIC cxx_std_14)

            set(CompilerFlags "")

            if (${Djinn.Compiler} STREQUAL GNU)
                set(CompilerFlags "-Wl,--gc-sections")
            endif()

            string(STRIP "${CompilerFlags}" StrippedCompilerFlags)
            set_target_properties(
                    "${Djinn.Projects.${Project}.Name}" PROPERTIES COMPILE_FLAGS
                    "${Djinn.Projects.${Project}.CxxFlags} ${Djinn.Projects.${Project}.CFlags} ${Djinn.Projects.${Project}.LinkerFlags} ${StrippedCompilerFlags}"
            )

            string(STRIP "${Djinn.Projects.${Project}.LinkerFlags} ${StrippedCompilerFlags}" StrippedLinkerFlags)
            set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES LINK_FLAGS "${StrippedLinkerFlags}")

            # Ensure it's always colorized
            set(CompilerOptions "")
            if (${CMAKE_GENERATOR} STREQUAL Ninja)
                if (${Djinn.Compiler} STREQUAL GNU)
                    set(CompilerOptions "-fdiagnostics-color=always")
                elseif (${Djinn.Compiler.Backend} STREQUAL CLANG)
                    set(CompilerOptions "-fcolor-diagnostics")
                endif()
            endif()

            string(STRIP "${CompilerOptions}" StrippedCompilerOptions)
            if (NOT "${StrippedCompilerOptions}" STREQUAL "")
                set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES COMPILE_OPTIONS "${StrippedCompilerOptions}")
            endif()

            #target_compile_options(
            #        "${Djinn.Projects.${Project}.Name}" PUBLIC
            #        ${Djinn.Projects.${Project}.CxxFlags}
            #        ${Djinn.Projects.${Project}.CFlags}
            #        ${Djinn.Projects.${Project}.LinkerFlags}
            #)

            # TODO: Experimental
            set_property(TARGET "${Djinn.Projects.${Project}.Name}" PROPERTY LINK_DEPENDS_NO_SHARED 1)

            # Configure destination
            set_target_properties(
                    "${Djinn.Projects.${Project}.Name}"
                    PROPERTIES
                    ARCHIVE_OUTPUT_DIRECTORY "${ArchiveDir}"
                    LIBRARY_OUTPUT_DIRECTORY "${LibraryDir}"
                    RUNTIME_OUTPUT_DIRECTORY "${RuntimeDir}"
                    COMPILE_PDB_OUTPUT_DIRECTORY "${PDBDir}"
            )

            # ...
            Djinn_SetGlobal(${Djinn.Projects.${Project}.Package}.AllDefines
                    DJINN_API_${Djinn.Projects.${Project}.PreprocessorName}

                    # Paths (Useful for build tools)
                    "DJINN_PATH_PIPELINE=\"${DJINN_PATH_PIPELINE}\""
                    "DJINN_PATH_EXTERN=\"${DJINN_PATH_EXTERN}\""
                    "DJINN_PATH_BUILD=\"${DJINN_PATH_BUILD}\""

                    # Internal export
                    "DJINN_EXPORT=${Djinn.Cxx.DLLExport}"
                    "DJINN_IMPORT=${Djinn.Cxx.DLLImport}"
                    "DJINN_NOEXPORT=${Djinn.Cxx.DLLIgnore}"
                    "DJINN_EXPORT_${Djinn.Projects.${Project}.PreprocessorName}=${Djinn.Cxx.DLLExport}"
                    "DJINN_C_EXPORT_${Djinn.Projects.${Project}.PreprocessorName}=extern \"C\" ${Djinn.Cxx.DLLExport}"

                    # Exception handling
                    "DJINN_THROW=throw"

                    # Platform Specific
                    ${Djinn.Platform.${Djinn.Platform}.Preprocessors}
                    DJINN_PLATFORM_${Djinn.Platform.Uppercase}

                    # Compiler specific
                    DJINN_COMPILER_${Djinn.Compiler.Uppercase}
                    DJINN_COMPILER_BACKEND_${Djinn.Compiler.Backend.Uppercase}
                    ${Djinn.Compiler.Preprocessors}

                    # Cxx
                    ${Djinn.Cxx.Preprocessors}

                    # Defines
                    ${${Djinn.Projects.${Project}.Package}.Defines}
                    )

            # Remove duplicates
            set(IndexMap "")
            set(DefineCopy "")
            foreach (Define ${${Djinn.Projects.${Project}.Package}.AllDefines})
                string(FIND ${Define} "=" EQINDEX)
                if (NOT ${EQINDEX} EQUAL -1)
                    string(SUBSTRING ${Define} 0 ${EQINDEX} DEFNAME)
                    if (NOT ${DEFNAME} IN_LIST IndexMap)
                        list(APPEND DefineCopy ${Define})
                        list(APPEND IndexMap ${DEFNAME})
                    endif ()
                elseif (NOT ${Define} IN_LIST DefineCopy)
                    list(APPEND DefineCopy ${Define})
                endif ()
            endforeach ()
            Djinn_SetGlobal(${Djinn.Projects.${Project}.Package}.AllDefines ${DefineCopy})

            # Preprocessors
            target_compile_definitions(
                    "${Djinn.Projects.${Project}.Name}"

                    # Locals
                    PRIVATE ${${Djinn.Projects.${Project}.Package}.AllDefines}
            )

            # Header Generation
            if ("${Djinn.Projects.${Project}.InstallType}" STREQUAL "DYNAMIC" OR ExecDHG)
                set(DHGConfigPath "${APIPath}/${Djinn.Projects.${Project}.Name}.dhg")

                # Setup DHG config file
                set(DHGConfig "1\n")
                set(DHGConfig "${DHGConfig}${Djinn.Projects.${Project}.Name}\n")
                set(DHGConfig "${DHGConfig}${APIPath}\n")
                set(DHGConfig "${DHGConfig}#include\n")

                string(REPLACE ";" "\n" StandardIncludesNewline "${Djinn.Compiler.StandardIncludesList}")
                set(DHGConfig "${DHGConfig}${StandardIncludesNewline}\n")

                foreach (Define ${${Djinn.Projects.${Project}.Package}.IncludeDirs} ${Djinn.Projects.${Project}.Path}
                        ${${Djinn.Projects.${Project}.Package}.ExternIncludeDirs} ${CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_INCLUDE_DIRS}
                        ${CMAKE_EXTRA_GENERATOR_C_SYSTEM_INCLUDE_DIRS})
                    set(DHGConfig "${DHGConfig}${Define}\n")
                endforeach ()
                set(DHGConfig "${DHGConfig}#preprocessor\n")
                foreach (Define ${${Djinn.Projects.${Project}.Package}.AllDefines})
                    set(DHGConfig "${DHGConfig}${Define}\n")
                endforeach ()
                set(DHGConfig "${DHGConfig}#args\n")
                string(REPLACE " " "\n" DHGFlags "${Djinn.Projects.${Project}.CxxClangFlags}")
                set(DHGConfig "${DHGConfig}${DHGFlags}\n")
                set(Index 0)
                list(LENGTH CMAKE_EXTRA_GENERATOR_C_SYSTEM_DEFINED_MACROS Length)
                while (${Index} LESS ${Length})
                    math(EXPR Index2 "${Index} + 1")
                    list(GET CMAKE_EXTRA_GENERATOR_C_SYSTEM_DEFINED_MACROS ${Index} Name)
                    list(GET CMAKE_EXTRA_GENERATOR_C_SYSTEM_DEFINED_MACROS ${Index2} Value)
                    if ("${Value}" STREQUAL " " OR "${Value}" STREQUAL "")
                        set(Value "1")
                    endif ()
                    set(DHGConfig "${DHGConfig}${Name}=${Value}\n")
                    math(EXPR Index "${Index} + 2")
                endwhile ()
                set(DHGConfig "${DHGConfig}#header\n")
                foreach (Source ${Djinn.Projects.${Project}.Sources})
                    if (${Source} MATCHES "\\.(h|H|(h|H)(p|P)(p|P))$")
                        string(SUBSTRING ${Source} ${BaseLength} -1 Source)
                        set(DHGConfig "${DHGConfig}${Source}\n")
                    endif ()
                endforeach ()

                # Write
                Djinn_WriteFile("${DHGConfigPath}" "${DHGConfig}")

                # DHG Target name
                set(CustomName "Z_DHG.${Djinn.Projects.${Project}.Name}")

                # May be externally built
                set(ToolsDependencies "")
                if ("Tools.DHG" IN_LIST Djinn.Projects)
                    set(ToolsDependencies "Tools.DHG")
                endif ()

                # DHG Pass
                add_custom_target(
                        ${CustomName}
                        ALL COMMAND "${DJINN_PATH_PIPELINE}/Tools.DHG.exe" "${DHGConfigPath}"
                        #DEPENDS ${${Djinn.Projects.${Project}.Package}.Dependencies}
                        DEPENDS ${ToolsDependencies}
                        WORKING_DIRECTORY "${Djinn.Projects.${Project}.Path}"
                )

                # Configure post processing dependencies
                add_dependencies("${Djinn.Projects.${Project}.Name}" "${CustomName}")
            endif ()
        else ()
            # Dummy target
            add_library("${Djinn.Projects.${Project}.Name}" INTERFACE)
            #add_custom_target("${Djinn.Projects.${Project}.Name}")
        endif ()
    endforeach ()
    Djinn_LogInfo("!! /Ignore Warnings !!")

    # Foreach project
    foreach (Project ${Djinn.Projects})
        # Apply deps
        if ("${Djinn.Projects.${Project}.InstallType}" STREQUAL "DYNAMIC" OR ExecDHG)
        else ()
            foreach (Dep ${${Djinn.Projects.${Project}.Package}.Dependencies})
                # Is target?
                if (${Dep} IN_LIST Djinn.Projects)
                    add_dependencies("${Djinn.Projects.${Project}.Name}" "${Dep}")
                endif ()
            endforeach ()
        endif ()

        if (NOT "${Djinn.Projects.${Project}.InstallType}" STREQUAL "VIRTUAL")
            # Set linker language
            set_target_properties("${Djinn.Projects.${Project}.Name}" PROPERTIES LINKER_LANGUAGE CXX)

            # Apply
            if (NOT "${${Djinn.Projects.${Project}.Package}.IncludeDirs}" STREQUAL "")
                target_include_directories("${Djinn.Projects.${Project}.Name}" PUBLIC ${${Djinn.Projects.${Project}.Package}.IncludeDirs} ${Djinn.Projects.${Project}.Path})
            endif ()

            if (NOT "${${Djinn.Projects.${Project}.Package}.ExternIncludeDirs}" STREQUAL "")
                target_include_directories("${Djinn.Projects.${Project}.Name}" SYSTEM PUBLIC ${${Djinn.Projects.${Project}.Package}.ExternIncludeDirs})
            endif ()

            if (NOT "${${Djinn.Projects.${Project}.Package}.LinkDirs}" STREQUAL "")
                # link_directories("${Djinn.Projects.${Project}.Name}" PUBLIC ${${Djinn.Projects.${Project}.Package}.LinkDirs})
            endif ()

            if (NOT "${${Djinn.Projects.${Project}.Package}.Links}" STREQUAL "")
                set(Libs "")
                foreach (DepLib ${${Djinn.Projects.${Project}.Package}.Links})
                    if (NOT ${DepLib} IN_LIST Djinn.Projects)
                        find_library(${DepLib}.FoundPath NAMES "${DepLib}" PATHS ${${Djinn.Projects.${Project}.Package}.LinkDirs})
                        if (${DepLib}.FoundPath STREQUAL "${DepLib}.FoundPath-NOTFOUND")
                            Djinn_LogError("Project ${Project} failed to find library: '${DepLib}'\nAttempted to search in:\n${${Djinn.Projects.${Project}.Package}.LinkDirs}")
                        endif ()
                        list(APPEND Libs ${${DepLib}.FoundPath})
                    else ()
                        # CMake resolves in house dependencies!
                        list(APPEND Libs ${DepLib})
                    endif ()
                endforeach ()

                if (NOT "${Libs}" STREQUAL "")
                    #if (${${Djinn.Projects.${Project}.Package}.Name} STREQUAL "Tools.DHG")
                    #    list(REMOVE_ITEM Libs Libraries.Core.Memory;Libraries.Core.Time;Libraries.Platform.IO;Libraries.Platform.Library;Libraries.Platform.Threading;Libraries.Platform.User;Libraries.Platform.Window;Libraries.VectorMath)
                    #endif()

                    #if (${${Djinn.Projects.${Project}.Package}.Name} STREQUAL "Tools.DHG")
                    #    list(REMOVE_ITEM Libs Libraries.Core.Memory;Libraries.Core.Time;Libraries.Platform.IO;Libraries.Platform.Library;Libraries.Platform.Threading;Libraries.Platform.User;Libraries.Platform.Window;Libraries.VectorMath)
                    #endif()

                    target_link_libraries("${Djinn.Projects.${Project}.Name}" PUBLIC ${Libs})
                    #target_link_libraries("${Djinn.Projects.${Project}.Name}" PUBLIC ${${Djinn.Projects.${Project}.Package}.Links})
                endif()
            endif ()

            if (NOT "${${Djinn.Projects.${Project}.Package}.ExternLinks}" STREQUAL "")
                target_link_libraries("${Djinn.Projects.${Project}.Name}" PUBLIC ${${Djinn.Projects.${Project}.Package}.ExternLinks})
            endif ()

            # Additional links
            if (Djinn.Platform STREQUAL Linux)
                target_link_libraries("${Djinn.Projects.${Project}.Name}" PUBLIC -ldl -pthread)
            endif ()
        endif ()

        # Create common file
        set(LocalProject Djinn.Projects.${Project})
        set(LocalPackage ${Djinn.Projects.${Project}.Package})
        set(TEMPLATE "")
        # file(READ "${DJINN_PATH_BUILD}/Project/Templates/Project.md" TMPL_PROJECTMD)
        include(${DJINN_PATH_BUILD}/Project/Templates/Project.md.cmake)
        # string(REPLACE "{NAME}" "${SAFE_DEFINE}" TMPL_PROJECTMD "${TMPL_PROJECTMD}")
        file(WRITE "${${Djinn.Projects.${Project}.Name}_BINARY_DIR}/${Djinn.Projects.${Project}.Name}.config.md" "${TEMPLATE}")

        # ...
        Djinn_LogInfo("Configured project: ${Project}")
    endforeach ()

    # Build all project
    add_custom_target("All" DEPENDS ${Djinn.Projects})

    # ...
    Djinn_LogInfo("Projects configured!")
endfunction(Djinn_Project_ConfigureAll)