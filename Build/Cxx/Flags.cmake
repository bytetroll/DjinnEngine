# Reset flags
function(Djinn_Cxx_SetFlags OUT BUILD_TYPE)
    set(OUT_CXX      ${OUT}.CxxFlags)
    set(OUT_C        ${OUT}.CFlags)
    set(OUT_CXXCLANG ${OUT}.CxxClangFlags)
    set(OUT_LINKER   ${OUT}.LinkerFlags)

    # Defaults
    # Note: On GCC compilers =gnu++17 is used instead of std++17 for extensions
    if (${Djinn.Compiler} STREQUAL GNU)
        Djinn_SetGlobal(${OUT_CXX} "-std=gnu++17 -W -fPIC -msse -msse2 -msse3 -mmmx")
        Djinn_SetGlobal(${OUT_LINKER} "-fPIC")
    elseif (${Djinn.Compiler} STREQUAL CLANG)
        if (${Djinn.Platform} STREQUAL Windows)
            Djinn_SetGlobal(${OUT_CXX} "-std=c++17 -W -msse -msse2 -msse3 -mmmx")
        else()
            Djinn_SetGlobal(${OUT_CXX} "-std=c++17 -stdlib=libc++ -W -fPIC -msse -msse2 -msse3 -mmmx")
        endif()
    elseif (${Djinn.Compiler} STREQUAL MSVC)
        Djinn_SetGlobal(${OUT_CXX} "/std:c++17 /EHa /MD")
        Djinn_SetGlobal(${OUT_CXXCLANG} "-std=c++17")

        if (${Djinn.Compiler.Backend} STREQUAL CLANG)
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} /clang:-fconstexpr-steps=2048 /clang:-fconstexpr-depth=2048")
            Djinn_SetGlobal(${OUT_CXXCLANG} "${${OUT_CXXCLANG}} -std=c++17 -fconstexpr-steps=2048 -fconstexpr-depth=2048")
        endif()
    else ()
        Djinn_LogError("Unhandled compiler")
    endif ()

    # Configuration types
    # Check to see if the compiler supports the d/D suffix.
    # Note: check_c_source_compiles appears to be missing from newer CMake versions.
#    Djinn_LogInfo("Checking extension: d/D double constant suffix.")
#    CHECK_C_SOURCE_COMPILES(" \
#        #define DBLVAL 12345.12345d \
#        int main() {int i = DBLVAL+1;}" COMPILER_SUPPORTS_D_SUFFIX)
#    IF(COMPILER_SUPPORTS_D_SUFFIX)
#        Djinn_LogInfo("Extension supported.  Enabling d/D double constant suffix.")
        # ADD_DEFINITIONS(-DCOMPILER_SUPPORTS_D_SUFFIX)
#    ENDIF(COMPILER_SUPPORTS_D_SUFFIX)

    # Append build architecture
    Djinn_LogInfo("Build type is ${CMAKE_BUILD_TYPE}")
    if (CMAKE_BUILD_TYPE MATCHES "64")
        if (${Djinn.Compiler} STREQUAL GNU OR ${Djinn.Compiler} STREQUAL CLANG)
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -m64 ")
            Djinn_SetGlobal(${OUT_CXXCLANG} "${${OUT_CXXCLANG}} -m64")
            Djinn_SetGlobal(${OUT_C} "${CMAKE_C_FLAGS} -m64 ")
        elseif (${Djinn.Compiler} STREQUAL MSVC)
            # ...
        endif ()

        Djinn_AddGlobalPreprocessor(DJINN_64)
    else ()
        # Djinn_LogError("32-bit not supported.  If this is an error, check CMake target flags in CLion.")

        Djinn_AddGlobalPreprocessor(DJINN_32)
    endif ()

    # Warning Flags
    # Note: "-Wduplicated-branches" was removed due to incorrect behaviour
    #set(CXX_WARNING_FLAGS "")
    if (${Djinn.Compiler} STREQUAL GNU)
        Djinn_LogInfo("Using GCC as current compiler.")
        Djinn_SetGlobal(CXX_WARNING_FLAGS "-Werror -Wall -Wextra -Wduplicated-cond -Wlogical-op -Wnull-dereference -Wformat=0 -Wno-unused-function -Wno-reorder -Wold-style-cast -Wno-maybe-uninitialized -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-null-dereference -Wno-error=cpp -Wno-cast-function-type")
    elseif (${Djinn.Compiler} STREQUAL CLANG)
        Djinn_LogInfo("Using Clang as current compiler.")
        # ! DO NOT INDENT, ADD SPACE AFTER  !
        # Note: no-deprecated-copy and no-unused-value are required for compiling XMM math library on Linux.
        #       no-delete-incomplete is required for compiling the registry system.
        # -Wdelete-non-abstract-non-virtual-dtor is needed for GCC STL compatibility.
        #
        # Special note on unknown attributes.  Required for compiled DLL linkage on Linux.
        # On Windows linkage means osmething, but on Linux it's unused so ignore it.
        # This flag needs to be added, but ideally needs to be added only on Linux.  Another
        # solution is to rewrite/comminze export macros to support this in code.
        Djinn_SetGlobal(CXX_WARNING_FLAGS "\
-D_CRT_SECURE_NO_WARNINGS \
-D__STDC_CONSTANT_MACROS \
-D__STDC_LIMIT_MACROS \
-Werror \
-Wall \
-Wextra \
-Wnull-dereference \
-Wformat=0 \
-Wno-unused-function \
-Wno-reorder \
-Wold-style-cast \
-Wno-uninitialized \
-Wno-unused-parameter \
-Wno-unused-variable \
-Wno-unused-const-variable \
-Wno-unused-parameter \
-Wno-null-dereference \
-Wno-missing-braces \
-Wno-unused-private-field \
-Wno-error=cpp \
-Wno-return-type-c-linkage \
-Wno-bad-function-cast \
-Wno-deprecated-copy \
-Wno-unused-value \
-Wno-delete-incomplete \
-Wno-delete-non-abstract-non-virtual-dtor \
-Wno-unused-command-line-argument \
-Wno-unknown-attributes")
    elseif (${Djinn.Compiler} STREQUAL MSVC)
        # ! DO NOT INDENT, ADD SPACE AFTER  !
        Djinn_SetGlobal(CXX_WARNING_FLAGS "\
-D_CRT_SECURE_NO_WARNINGS \
-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING \
-Wno-return-type-c-linkage")
    endif ()

    # Set intermediate flags
    if (${Djinn.Compiler} STREQUAL GNU)
        if (${BUILD_TYPE} MATCHES "Debug")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -g -O0 -DDEBUG ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} -g")
        elseif (${BUILD_TYPE} MATCHES "Verify")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -g -O0 -DDEBUG ${CXX_WARNING_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} -g -fsanitize=address")
        elseif (${BUILD_TYPE} MATCHES "Release")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -O3 -s ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} -s")
        elseif (${BUILD_TYPE} MATCHES "Profile")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -O3 -g ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} -O3 -g")
        endif ()
    elseif (${Djinn.Compiler} STREQUAL CLANG)
        if (${BUILD_TYPE} MATCHES "Debug")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -g -O0 -DDEBUG ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} -g")
        elseif (${BUILD_TYPE} MATCHES "Verify")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -g -O0 -DDEBUG ${CXX_WARNING_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} -g -fsanitize=address")
        elseif (${BUILD_TYPE} MATCHES "Release")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -O3 ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}}")
        elseif (${BUILD_TYPE} MATCHES "Profile")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} -O3 -g ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} -O3 -g")
        endif ()
    elseif (${Djinn.Compiler} STREQUAL MSVC)
        if (${BUILD_TYPE} MATCHES "Debug")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} /Od /Z7 ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} /DEBUG")
        elseif (${BUILD_TYPE} MATCHES "Verify")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} /Od /Z7 ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} /DEBUG")
        elseif (${BUILD_TYPE} MATCHES "Release")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} /O2 ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}}")
        elseif (${BUILD_TYPE} MATCHES "Profile")
            Djinn_SetGlobal(${OUT_CXX} "${${OUT_CXX}} /O2 /Z7 ${CXX_WARNING_FLAGS}")
            Djinn_SetGlobal(${OUT_LINKER} "${${OUT_LINKER}} /DEBUG")
        endif ()
    endif ()

    if (${Djinn.Compiler} STREQUAL CLANG)
        Djinn_SetGlobal(${OUT_CXXCLANG} ${${OUT_CXX}})
    endif()
endfunction(Djinn_Cxx_SetFlags)