# Find the scip library: SCIP Solving Constraint Integer Programs
# see http://scip.zib.de/#scipoptsuite
#
# SCIP_FOUND
# SCIP_INCLUDE_DIRS
# SCIP_LIBRARIES

# set(Scip_DEBUG false)
find_library(Scip NAMES scip
    DOC "SCIP Core Library")
find_library(ScipOpt NAMES scipopt
    DOC "SCIP Optimization Suite: Scipopt")

foreach(path ${CMAKE_PREFIX_PATH})
    find_path(SCIP_INCLUDE_DIRS NAMES scip/scip.h
        HINTS ${path}/include
        DOC "SCIP include path")
    if(SCIP_INCLUDE_DIR)
        break()
    endif()
endforeach()

if(Scip AND ScipOpt AND SCIP_INCLUDE_DIRS)
    set(SCIP_FOUND true)
endif()

if(SCIP_FOUND)
    if(Scip_DEBUG)
        message(STATUS "Found installation of scip")
        message(STATUS "  scip library:      " ${Scip})
        message(STATUS "  scipopt library:   " ${ScipOpt})
        message(STATUS "  include directory: " ${SCIP_INCLUDE_DIRS})
    endif()
    list(APPEND SCIP_LIBRARIES ${Scip})
    list(APPEND SCIP_LIBRARIES ${ScipOpt})
    include_directories(${SCIP_INCLUDE_DIRS})
endif()


