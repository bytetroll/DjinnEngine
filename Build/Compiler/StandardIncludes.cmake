Djinn_LogInfo("Compiling dummy program for standard library extraction")

Djinn_LogInfo(${Djinn.Compiler} )

if (${Djinn.Compiler} STREQUAL MSVC)
    set(Args /clang:-x /clang:c++ /clang:-c /clang:-stdlib=libc++ /clang:-v)
else()
    set(Args -x c++ -c -stdlib=libc++ -v)
endif()

# Execute the compiler against dummy
execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${Args} ${DJINN_PATH_BUILD}/Cxx/Dummy.cpp
        OUTPUT_VARIABLE CxxDummyCompilation ERROR_VARIABLE CxxDummyCompilation
        RESULT_VARIABLE CxxDummyResult)

# Succeeded?
if (NOT ${CxxDummyResult} STREQUAL 0)
    Djinn_LogError("Failed to compile dummy configuration program:\n${CxxDummyCompilation}")
endif()

# Find start of standard includes
set(StartSegment "#include <...> search starts here:\n")
string(LENGTH "${StartSegment}" StartLength)

# Find relevant segment
string(FIND "${CxxDummyCompilation}" "${StartSegment}" Begin)
string(FIND "${CxxDummyCompilation}" "End of search list." End)
math(EXPR Begin "${Begin} + ${StartLength}")
math(EXPR Length "${End} - ${Begin}")

# Extract it, strip it and remove whitespaces
string(SUBSTRING "${CxxDummyCompilation}" ${Begin} ${Length} StandardIncludes)
string(REPLACE "\n" ";" ListStandardIncludes "${StandardIncludes}")
string(REGEX REPLACE "[ \t\r\n]" "" ListStandardIncludes "${ListStandardIncludes}")
list(TRANSFORM ListStandardIncludes STRIP)

# OK
Djinn_SetGlobal(Djinn.Compiler.StandardIncludesList "${ListStandardIncludes}")
Djinn_LogInfo("\n${StandardIncludes}")