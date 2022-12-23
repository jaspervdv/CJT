cmake_minimum_required (VERSION 3.0.0 FATAL_ERROR)

# Project name
project (CJTExample1 CXX)
set(CMAKE_CXX_STANDARD 17)

# Tbb
find_package(tbb REQUIRED NO_DEFAULT_PATH)

# OpenCascade
find_package(OpenCASCADE)

# CJT
find_package(CJT)

# Configure C++ compiler's includes dir
include_directories ( 
    ${OpenCASCADE_INCLUDE_DIR} 
    ${CJT_INCLUDE_DIR}    
)

link_directories(
    ${OpenCASCADE_LIBRARY_DIR}
    ${CJT_LIBRARY_DIR}
)

# Add executable
add_executable (${CMAKE_PROJECT_NAME}
    ${CMAKE_PROJECT_NAME}.cpp
)

# Add linker options
target_link_libraries(${CMAKE_PROJECT_NAME}
CJT
)

foreach (LIB ${OpenCASCADE_LIBRARIES})
    target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCASCADE_LIBRARY_DIR}/${LIB}.lib)
endforeach()


# Adjust runtime environment
get_target_property(TBB_BIN  TBB::tbb IMPORTED_LOCATION_RELEASE)
cmake_path(GET TBB_BIN PARENT_PATH TBB_BINARY_DIR)
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<$<CONFIG:DEBUG>:${OpenCASCADE_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${OpenCASCADE_BINARY_DIR}>;%PATH%;$<$<CONFIG:DEBUG>:${TBB_BINARY_DIR}d>$<$<NOT:$<CONFIG:DEBUG>>:${TBB_BINARY_DIR}>")
