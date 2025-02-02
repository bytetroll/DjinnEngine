
####
## Prepares the internal package system
####

# Package List
set(Djinn.Packages "" CACHE INTERNAL "" FORCE)

# Includes
include(${DJINN_PATH_BUILD}/Packages/Package.cmake)
include(${DJINN_PATH_BUILD}/Packages/PackageResolve.cmake)

# Ensure the root package folder exists
file(MAKE_DIRECTORY ${DJINN_PATH_BUILD_CMAKE_CACHE}/Packages)