cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)

# Project name
project (CJTExample2 CXX)
set(CMAKE_CXX_STANDARD 17)

#nhollman
find_package(nlohmann_json REQUIRED NO_DEFAULT_PATH)

# Tbb
find_package(tbb REQUIRED NO_DEFAULT_PATH)

# OpenCascade
find_package(OpenCASCADE REQUIRED NO_DEFAULT_PATH)

# Boost
set(Boost_NO_SYSTEM_PATHS TRUE)
find_package(Boost REQUIRED log)

if(Boost_DIR STREQUAL "Boost_DIR-NOTFOUND")
    if(${CMAKE_VS_PLATFORM_TOOLSET} STREQUAL v142)
        if(${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE} STREQUAL x64)
            if(EXISTS "${Boost_INCLUDE_DIRS}/lib64-msvc-14.2")
                set(Boost_DIR "${Boost_INCLUDE_DIRS}/lib64-msvc-14.2")
            endif()
        endif()
    elseif(${CMAKE_VS_PLATFORM_TOOLSET} STREQUAL v141)
        if(${CMAKE_VS_PLATFORM_TOOLSET_HOST_ARCHITECTURE} STREQUAL x64)
            if(EXISTS "${Boost_INCLUDE_DIRS}/lib64-msvc-14.1")
                set(Boost_DIR "${Boost_INCLUDE_DIRS}/lib64-msvc-14.1")
            endif()
        endif()
    endif()
endif()

# CJT
find_package(CJT)

# Configure C++ compiler's includes dir
include_directories ( 
    ${OpenCASCADE_INCLUDE_DIR} 
	${Boost_INCLUDE_DIRS}
    ${CJT_INCLUDE_DIR}    
)

link_directories(
    ${OpenCASCADE_LIBRARY_DIR}
    ${Boost_DIR}
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
)

foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()


# Adjust runtime environment
get_target_property(TBB_BIN  TBB::tbb IMPORTED_LOCATION_RELEASE)
cmake_path(GET TBB_BIN PARENT_PATH TBB_BINARY_DIR)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%;$<$<CONFIG:DEBUG>:${TBB_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${TBB_BINARY_DIR}>")

