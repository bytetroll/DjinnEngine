cmake_minimum_required(VERSION 3.7)

####
## Loads the build system and performs any needed configurations
####

enable_language(ASM)

# Assign build system
set(DJINN_BUILD_SYSTEM ${DJINN_ROOT}/Build/BuildSystem.cmake)

# Assign common paths
set(DJINN_PATH_PIPELINE ${DJINN_ROOT}/Pipeline/${CMAKE_BUILD_TYPE})
set(DJINN_PATH_EXTERN ${DJINN_ROOT}/External)
set(DJINN_PATH_BUILD ${DJINN_ROOT}/Build)
set(DJINN_PATH_BUILD_CMAKE_CACHE ${CMAKE_BINARY_DIR})

# --------- #
#  Bug fix  #
# --------- #

string(SUBSTRING ${DJINN_BUILD_SYSTEM} 0 3 DJINN_BUILD_SYSTEM_DRIVE)
string(SUBSTRING ${DJINN_PATH_BUILD_CMAKE_CACHE} 3 -1 DJINN_PATH_BUILD_CMAKE_CACHE_DRIVELESS)
set(DJINN_PATH_BUILD_CMAKE_CACHE "${DJINN_BUILD_SYSTEM_DRIVE}${DJINN_PATH_BUILD_CMAKE_CACHE_DRIVELESS}")