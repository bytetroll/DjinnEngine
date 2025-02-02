macro(Djinn_ConfigWriteStart VAR)
    set(${VAR} "")
endmacro()

macro(Djinn_ConfigWrite CONFIG NAME)
    set(${CONFIG} "${${CONFIG}}${NAME}\n${ARGN}\n")
endmacro()

function(Djinn_ConfigWriteEnd CONFIG PATH)
    file(WRITE "${PATH}" "${${CONFIG}}")
endfunction()

function(Djinn_ConfigRead PATH PREFIX GLOBAL)
    file(READ "${PATH}" Contents)
    string(REPLACE ";" "½" Contents "${Contents}")
    string(REPLACE "\n" ";" Contents "${Contents}")

    list(LENGTH Contents Length)

    set(i 0)

    set(break OFF)
    while (NOT ${break})
        list(GET Contents ${i} Name)
        math(EXPR i "${i} + 1")
        list(GET Contents ${i} Value)
        string(REPLACE "½" ";" Value "${Value}")
        math(EXPR i "${i} + 1")

        if (GLOBAL)
            Djinn_SetGlobal("${PREFIX}${Name}" ${Value})
        else ()
            set("${PREFIX}${Name}" ${Value})
        endif ()

        math(EXPR j "${i} + 2")
        if (${j} GREATER ${Length})
            set(break ON)
        endif ()
    endwhile ()
endfunction()