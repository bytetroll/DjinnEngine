
####
## Prepares the platform config
####

### CONFIG

# Available platforms
set(Djinn.Platforms Windows Linux Darwin)

# TODO: Detect
set(Djinn.Platform Windows)
if (WIN32)
    set(Djinn.Platform Windows)
elseif(UNIX)
    set(Djinn.Platform Linux)
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # Can't check if "Apple" is defined -- implies any Apple platform.  Darwin is
    # is specifically macOS.  nyoung; 6.22.2019.
    set(Djinn.Platform Darwin)
endif()

# Windows configuration
set(Djinn.Platform.Windows.Preprocessors WINVER=0x0600 _WIN32_WINNT=0x0600)

### /CONFIG

# To uppercase
set(Djinn.Platforms.Uppercase "")
foreach (Platform ${Djinn.Platforms})
    string(TOUPPER ${Platform} Str)
    list(APPEND Djinn.Platforms.Uppercase ${Str})
endforeach ()

# To uppercase
string(TOUPPER ${Djinn.Platform} Djinn.Platform.Uppercase)

# Includes
include(${DJINN_PATH_BUILD}/Platform/Platform.cmake)