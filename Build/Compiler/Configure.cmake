# Detect the current compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if ("${CMAKE_CXX_COMPILER}" MATCHES "clang-cl.exe$")
        Djinn_SetGlobal(Djinn.Compiler "MSVC")
        Djinn_SetGlobal(Djinn.Compiler.Backend "CLANG")
    else()
        Djinn_SetGlobal(Djinn.Compiler "CLANG")
        Djinn_SetGlobal(Djinn.Compiler.Backend "CLANG")

        Djinn_LogInfo("Detected Platform: ${Djinn.Platform}")
        if (${Djinn.Platform} STREQUAL Linux)
            Djinn_SetGlobal(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld -stdlib=libc++ -Wl,-rpath='$ORIGIN'")
        endif()
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    Djinn_SetGlobal(Djinn.Compiler "GNU")
    Djinn_SetGlobal(Djinn.Compiler.Backend "GNU")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    Djinn_SetGlobal(Djinn.Compiler "Intel")
    Djinn_SetGlobal(Djinn.Compiler.Backend "Intel")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    Djinn_SetGlobal(Djinn.Compiler "MSVC")
    Djinn_SetGlobal(Djinn.Compiler.Backend "MSVC")
else()
    Djinn_LogError("Unknown compiler '${CMAKE_CXX_COMPILER_ID}'")
endif()

# To uppercase
string(TOUPPER ${Djinn.Compiler}         Djinn.Compiler.Uppercase)
string(TOUPPER ${Djinn.Compiler.Backend} Djinn.Compiler.Backend.Uppercase)

# Default preprocessors
Djinn_SetGlobal(Djinn.Compiler.Preprocessors "")

# Includes
include(${DJINN_PATH_BUILD}/Compiler/StandardIncludes.cmake)
