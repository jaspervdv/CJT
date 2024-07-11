cmake_minimum_required(VERSION 3.10)

# Project name
project (CJTExample1 CXX)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Nlohmann
find_package(nlohmann_json CONFIG)
message(STATUS nlohmann_json)

if(nlohmann_json STREQUAL "")
    message(FATAL_ERROR "Unable find Open nlohmann package, specify nlohmann_json packaged manually.")
else()
    message(STATUS "Found Open nlohmann package in:" ${nlohmann_json_INCLUDE_DIR} ) 
endif()

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
include_directories ( SYSTEM 
${CJT_INCLUDE_DIR}  
${nlohmann_json_INCLUDE_DIR} 
)

link_directories(
    ${CJT_LIBRARY_DIR}
)

# Add executable
add_executable (${CMAKE_PROJECT_NAME}
    ${CMAKE_PROJECT_NAME}.cpp
)

# Add linker options
target_link_libraries(
${PROJECT_NAME} 
CJT
nlohmann_json::nlohmann_json 
)