# Add all subdirectories to the build system
function(Djinn_AddAllDirs)
    file(GLOB Files *)
    foreach (File ${Files})
        if (IS_DIRECTORY ${File} AND NOT ${File} MATCHES "^${CMAKE_SOURCE_DIR}/\\." AND EXISTS ${File}/CMakeLists.txt)
            add_subdirectory(${File})
        endif ()
    endforeach ()
endfunction(Djinn_AddAllDirs)

# Add directories to the build system
function(Djinn_AddDirs)
    foreach (Dir ${ARGN})
        add_subdirectory(${Dir})
    endforeach ()
endfunction(Djinn_AddDirs)