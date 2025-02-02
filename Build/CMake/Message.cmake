# Error
function(Djinn_LogError MESSAGE)
    message(FATAL_ERROR "(BuildSystem) Error, ${MESSAGE}")
endfunction(Djinn_LogError)

# Warning
function(Djinn_LogWarning MESSAGE)
    message(WARNING "(BuildSystem) Warning, ${MESSAGE}")
endfunction(Djinn_LogWarning)

# Info
function(Djinn_LogInfo MESSAGE)
    message(STATUS "(BuildSystem) Info, ${MESSAGE}")
endfunction(Djinn_LogInfo)

# Event
function(Djinn_LogEvent MESSAGE)
    message(STATUS "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=")
    Djinn_LogInfo("${MESSAGE}")
    message(STATUS "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=")
endfunction()