Djinn_SetGlobal(Djinn.Cxx.Preprocessors "")

# Custom configuration types
Djinn_SetGlobal(CMAKE_CONFIGURATION_TYPES "Debug64;Release64;Profile64;Verify64")

# Add global preprocessor
function(Djinn_AddGlobalPreprocessor STR)
    Djinn_SetGlobal(Djinn.Cxx.Preprocessors "${Djinn.Cxx.Preprocessors};${STR}")
endfunction()

# Profile mode?
# Profile also enables debug mode
if (CMAKE_BUILD_TYPE MATCHES "Profile")
    Djinn_AddGlobalPreprocessor(DJINN_PROFILE=1)
    Djinn_AddGlobalPreprocessor(DJINN_DEBUG=1)
endif ()

# Verify mode?
# Verify also enables debug mode
if (CMAKE_BUILD_TYPE MATCHES "Verify")
    Djinn_AddGlobalPreprocessor(DJINN_VERIFY=1)
    Djinn_AddGlobalPreprocessor(DJINN_PROFILE=1)
    Djinn_AddGlobalPreprocessor(DJINN_DEBUG=1)
endif ()

# Debug mode?
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    Djinn_AddGlobalPreprocessor(DJINN_DEBUG=1)
endif ()

# Release mode?
if (CMAKE_BUILD_TYPE MATCHES "Release")
    Djinn_AddGlobalPreprocessor(DJINN_RELEASE=1)
endif ()

# DLL Import
# TODO: VC++ dllXXX
if (${Djinn.Compiler} STREQUAL MSVC)
    Djinn_LogInfo("Using MSVC compatible dll linkage")
    Djinn_SetGlobal(Djinn.Cxx.DLLImport "__declspec(dllimport)")
    Djinn_SetGlobal(Djinn.Cxx.DLLExport "__declspec(dllexport)")
    Djinn_SetGlobal(Djinn.Cxx.DLLIgnore "__declspec(dllexport)")

    Djinn_SetGlobal(MSVC_COMPILE_FLAGS "${MSVC_COMPILE_FLAGS} /MP")
else ()
    if (Djinn.Platform STREQUAL Windows)
        Djinn_LogInfo("Using Windows compatible dll linkage")
        Djinn_SetGlobal(Djinn.Cxx.DLLExport "__attribute__ ((dllexport))")
        Djinn_SetGlobal(Djinn.Cxx.DLLImport "__attribute__ ((dllimport))")
        Djinn_SetGlobal(Djinn.Cxx.DLLIgnore "__attribute__ ((visibility (\"hidden\")))")
    elseif(Djinn.Platform STREQUAL Linux)
        Djinn_LogInfo("Using Linux compatible dll linkage")
        Djinn_SetGlobal(Djinn.Cxx.DLLExport "__attribute__ ((visibility (\"default\")))")
        Djinn_SetGlobal(Djinn.Cxx.DLLImport "__attribute__ ((visibility (\"default\")))")
        Djinn_SetGlobal(Djinn.Cxx.DLLIgnore "__attribute__ ((visibility (\"hidden\")))")
    elseif(Djinn.Platform STREQUAL Darwin)
        Djinn_LogInfo("Using Darwin compatible dll linkage")
        Djinn_SetGlobal(Djinn.Cxx.DLLExport "__attribute__ ((visibility (\"default\")))")
        Djinn_SetGlobal(Djinn.Cxx.DLLImport "__attribute__ ((visibility (\"default\")))")
        Djinn_SetGlobal(Djinn.Cxx.DLLIgnore "__attribute__ ((visibility (\"hidden\")))")
    else()
        Djinn_LogError("Unknown target platform")
    endif ()
endif ()

# Includes
include(${DJINN_PATH_BUILD}/Cxx/Flags.cmake)
