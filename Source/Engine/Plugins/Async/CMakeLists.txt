cmake_minimum_required(VERSION 3.7)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../Build/BuildSystem.cmake)

# Create project
Djinn_CreateProject(Plugins.Async)
Djinn_Project_AddDependencies(
        Libraries.COM
        Libraries.Host
        Libraries.Core
        Libraries.Platform
        Plugins.Base.Logging
)
Djinn_Project_SearchSources(
        Include Source
        WINDOWS IncludeWindows SourceWindows
        LINUX IncludeLinux SourceLinux
)

# Special jump instructions for user space fibers
if (Djinn.Platform STREQUAL Linux)
    Djinn_Project_SetSources(Asm/jump_x86_64_sysv_elf_gas.S)
    Djinn_Project_SetSources(Asm/make_x86_64_sysv_elf_gas.S)
    Djinn_Project_SetSources(Asm/ontop_x86_64_sysv_elf_gas.S)
endif ()

# ...
Djinn_Project_Install(DYNAMIC)

# Finalize
Djinn_Finalize()