macro(Djinn_Md_Prepare VAR)
    set(Djinn.Md.Template ${VAR})
endmacro()

macro(Djinn_Md_Header NAME)
    set(${Djinn.Md.Template} "${${Djinn.Md.Template}}\n# ${NAME}\n")
endmacro()

macro(Djinn_Md_Header1 NAME)
    set(${Djinn.Md.Template} "${${Djinn.Md.Template}}\n## ${NAME}\n")
endmacro()

macro(Djinn_Md_Header2 NAME)
    set(${Djinn.Md.Template} "${${Djinn.Md.Template}}\n### ${NAME}\n")
endmacro()

macro(Djinn_Md_Text NAME)
    set(${Djinn.Md.Template} "${${Djinn.Md.Template}}\n${NAME}\n")
endmacro()

macro(Djinn_Md_Option NAME)
    set(${Djinn.Md.Template} "${${Djinn.Md.Template}}* ${NAME}\n")
endmacro()