# Set global variable
function(Djinn_SetGlobal NAME)
    SET(${NAME} "${ARGN}" CACHE INTERNAL "" FORCE)
endfunction(Djinn_SetGlobal)

# Prefix all
macro(Djinn_Prefix VAR OUT PREFIX)
    set(${OUT} "")
    foreach (F ${VAR})
        list(APPEND ${OUT} "${PREFIX}${F}")
    endforeach (F)
endmacro(Djinn_Prefix)

# Postfix all
macro(Djinn_Postfix VAR OUT POSTFIX)
    set(${OUT} "")
    foreach (F ${VAR})
        list(APPEND ${OUT} "${F}${POSTFIX}")
    endforeach (F)
endmacro(Djinn_Postfix)