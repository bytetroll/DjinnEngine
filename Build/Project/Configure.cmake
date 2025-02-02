
####
## Prepares the internal project system
####

# Project List
set(Djinn.Projects "" CACHE INTERNAL "" FORCE)

# Includes
include(${DJINN_PATH_BUILD}/Project/Project.cmake)
include(${DJINN_PATH_BUILD}/Project/ProjectConfigure.cmake)