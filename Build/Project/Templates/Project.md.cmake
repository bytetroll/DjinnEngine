include(${DJINN_PATH_BUILD}/Utils/md.cmake)
Djinn_Md_Prepare(TEMPLATE)

Djinn_Md_Header("${${LocalProject}.Name} - Report")

Djinn_Md_Text("**Path:** `${${LocalPackage}.Dir}`")
Djinn_Md_Text("**Build Path:** `${${${LocalProject}.Package}.BuildPath}`")

# INSTALLED
Djinn_Md_Header1("Installed:")
foreach (Var ${${LocalPackage}.Installed})
    Djinn_Md_Option("`${Var}`")
endforeach ()

# DEFINES
Djinn_Md_Header1("Defines:")
foreach (Var ${${LocalPackage}.AllDefines})
    Djinn_Md_Option("`${Var}`")
endforeach ()

# DEPENDENCIES
Djinn_Md_Header1("Dependencies:")
foreach (Var ${${LocalPackage}.Dependencies})
    Djinn_Md_Option("`${Var}`")
endforeach ()

# INCLUDES
Djinn_Md_Header1("Include directories:")
foreach (Var ${${LocalPackage}.IncludeDirs})
    Djinn_Md_Option("`${Var}`")
endforeach ()

# INCLUDES
Djinn_Md_Header1("External Include directories:")
Djinn_Md_Text("    (Exempt from diagnostics such as extra compiler warnings)")
foreach (Var ${${LocalPackage}.ExternIncludeDirs})
    Djinn_Md_Option("`${Var}`")
endforeach ()

# LINK INCLUDES
Djinn_Md_Header1("Link directories:")
foreach (Var ${${LocalPackage}.LinkDirs})
    Djinn_Md_Option("`${Var}`")
endforeach ()

# LINKS
Djinn_Md_Header1("Links:")
foreach (Var ${${LocalPackage}.Links})
    Djinn_Md_Option("`${Var}`")
endforeach ()

# SOURCES
Djinn_Md_Header1("Sources:")
foreach (Var ${${LocalProject}.Sources})
    Djinn_Md_Option("`${Var}`")
endforeach ()