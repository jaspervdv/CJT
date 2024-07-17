cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)

# Project name
project (CJTExample3 CXX)
set(CMAKE_CXX_STANDARD 17)

#nhollman
find_package(nlohmann_json REQUIRED)

# OpenCascade
if("${OCC_INCLUDE_DIR}" STREQUAL "")
    find_path(OCC_INCLUDE_DIR Standard_Version.hxx
    PATHS
        /usr/include/occt
        /usr/include/oce
        /usr/include/opencascade
    )

    if(OCC_INCLUDE_DIR)
        message(STATUS "Found Open CASCADE include files in: ${OCC_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "Unable to find Open CASCADE include directory. Specify OCC_INCLUDE_DIR manually")
    endif()
else()
    set(OCC_INCLUDE_DIR ${OCC_INCLUDE_DIR} CACHE FILEPATH "Open CASCADE header files")
    message(STATUS "Looking for Open CASCADE include files in: ${OCC_INCLUDE_DIR}")
endif()

# get OCCT version
if(OCC_INCLUDE_DIR)
    file(STRINGS ${OCC_INCLUDE_DIR}/Standard_Version.hxx OCC_VERSION
        REGEX "#define OCC_VERSION_COMPLETE*"
    )
    string(REGEX MATCH "[0-9.]+" OCC_VERSION_STRING ${OCC_VERSION})
    
    message(STATUS "Open CASCADE Version: " ${OCC_VERSION_STRING})
endif(OCC_INCLUDE_DIR)

set(OPENCASCADE_LIBRARY_NAMES
TKernel TKMath TKBRep TKGeomBase TKGeomAlgo TKG3d TKG2d TKShHealing TKTopAlgo TKMesh TKPrim TKBool TKBO
TKFillet TKXSBase TKOffset TKHLR TKBin
)

if(OCC_VERSION_STRING VERSION_LESS 7.8.0)
    list(APPEND OPENCASCADE_LIBRARY_NAMES  TKIGES TKSTEPBase TKSTEPAttr TKSTEP209 TKSTEP)
else(OCC_VERSION_STRING VERSION_LESS 7.8.0)
    list(APPEND OPENCASCADE_LIBRARY_NAMES TKDESTEP TKDEIGES)
endif(OCC_VERSION_STRING VERSION_LESS 7.8.0)

if("${OCC_LIBRARY_DIR}" STREQUAL "")
    if(WIN32)
        find_path(OCC_LIBRARY_DIR TKernel)
    else()
        find_library(OCC_LIBRARY TKernel
            PATHS
            /usr/lib
    )
    endif()

    if(OCC_LIBRARY_DIR)
        message(STATUS "Found Open CASCADE library files in: ${OCC_LIBRARY_DIR}")
    elseif(OCC_LIBRARY)
        GET_FILENAME_COMPONENT(OCC_LIBRARY_DIR ${OCC_LIBRARY} PATH)
        message(STATUS "Found Open CASCADE library files in: ${OCC_LIBRARY_DIR}")
    else()
        message(FATAL_ERROR "Unable find Open CASCADE library directory, specify OCC_LIBRARY_DIR manually.")
    endif()
else()
    set(OCC_LIBRARY_DIR ${OCC_LIBRARY_DIR} CACHE FILEPATH "Open CASCADE library files")
    message(STATUS "Looking for Open CASCADE library files in: ${OCC_LIBRARY_DIR}")
endif()

mark_as_advanced(libTKernel)
find_library(libTKernel NAMES TKernel TKerneld PATHS ${OCC_LIBRARY_DIR})

if(libTKernel)
    message(STATUS "Required Open Cascade Library files found")
else()
    message(FATAL_ERROR "Unable to find Open Cascade library files, aborting")
endif()

# Use the found libTKernel as a template for all other OCC libraries
foreach(lib ${OPENCASCADE_LIBRARY_NAMES})
    string(REPLACE TKerneld "${lib}" lib_path "${libTKernel}")
    string(REPLACE TKernel "${lib}" lib_path "${lib_path}")
    list(APPEND OPENCASCADE_LIBRARIES "${lib_path}")
endforeach()

if(WIN32)
    list(APPEND OPENCASCADE_LIBRARIES ws2_32.lib)
endif()

get_filename_component(libTKernelExt ${libTKernel} EXT)
if("${libTKernelExt}" STREQUAL ".a")
    set(OCCT_STATIC ON)
endif()

if(OCCT_STATIC)
    find_package(Threads)
    if (NOT WIN32)
        set(OPENCASCADE_LIBRARIES ${OPENCASCADE_LIBRARIES} "dl")
    endif()
endif()

# Boost
if(WIN32 AND("$ENV{CONDA_BUILD}" STREQUAL ""))
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    set(Boost_USE_MULTITHREADED ON)

    # Disable Boost's autolinking as the libraries to be linked to are supplied
    # already by CMake, and wrong libraries would be asked for when code is
    # compiled with a toolset different from default.
    if(MSVC)
        add_definitions(-DBOOST_ALL_NO_LIB)

        # Necessary for boost version >= 1.67
        set(BCRYPT_LIBRARIES "bcrypt.lib")
    endif()
else()
    # Disable Boost's autolinking as the libraries to be linked to are supplied
    # already by CMake, and it's going to conflict if there are multiple, as is
    # the case in conda-forge's libboost feedstock.
    add_definitions(-DBOOST_ALL_NO_LIB)

    if(WIN32)
        # Necessary for boost version >= 1.67
        set(BCRYPT_LIBRARIES "bcrypt.lib")
    endif()
endif()

if(USE_MMAP)
    if(MSVC)
        # filesystem is necessary for the utf-16 wpath
        set(BOOST_COMPONENTS ${BOOST_COMPONENTS} iostreams filesystem)
    else()
        set(BOOST_COMPONENTS ${BOOST_COMPONENTS} iostreams)
    endif()

    add_definitions(-DUSE_MMAP)
endif()

find_package(Boost REQUIRED COMPONENTS ${BOOST_COMPONENTS})
message(STATUS "Boost include files found in ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries found in ${Boost_LIBRARY_DIRS}")

# CJT
# search default locations for config file
if("${CJT_DIR}" STREQUAL "")
    find_path(CJT_DIR CJTConfig.cmake
        PATHS
        ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/
        /usr/local/lib/cmake/CJT/
        /usr/local/share/cmake/CJT/
        /usr/share/cmake/CJT/
        NO_DEFAULT_PATH
    )
endif()

if(CJT_DIR)
    message(STATUS "Found CJT in: ${CJT_DIR}")
    set(CMAKE_PREFIX_PATH ${CJT_DIR} ${CMAKE_PREFIX_PATH})
    find_package(CJT REQUIRED)
else()
    message(ERROR "Unable to find CJT. Specify CJT location manually")
endif()

# Configure C++ compiler's includes dir
include_directories ( 
    ${OCC_INCLUDE_DIR} 
	${Boost_INCLUDE_DIRS}
    ${CJT_INCLUDE_DIR}    
)

link_directories(
    ${CJT_LIBRARY_DIR}
)

# Add executable
add_executable (${CMAKE_PROJECT_NAME}
    ${CMAKE_PROJECT_NAME}.cpp
)

# Add linker options
target_link_libraries(${CMAKE_PROJECT_NAME}
CJT
nlohmann_json::nlohmann_json
${Boost_LIBRARIES}
${OPENCASCADE_LIBRARIES}
)