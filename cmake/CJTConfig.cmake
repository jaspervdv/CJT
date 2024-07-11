get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

if(EXISTS "${PARENT_DIR}/CJT/inc")
    set(CJT_INCLUDE_DIR ${PARENT_DIR}/CJT/inc)
else()
    message(WARNING  "CJT include directory not found at expected location")
endif()

if(EXISTS "${PARENT_DIR}/Build")
    set(CJT_LIBRARY_DIR ${PARENT_DIR}/Build)
elseif(EXISTS "${PARENT_DIR}/build")
    set(CJT_LIBRARY_DIR ${PARENT_DIR}/build)
else()
    message(WARNING "CJT library directory not found at expected location")
endif()

# set the libary end paths for easy access
set (CJT_Foundation_LIBRARIES)
foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
    if(EXISTS "${CJT_LIBRARY_DIR}/${CONFIG}")
        list(APPEND CJT_Foundation_LIBRARIES "${CONFIG}/CJT")
    else()
        message(STATUS "CJT lib with config '${CONFIG}' can not be found, is passed over" )
    endif() 
endforeach()


