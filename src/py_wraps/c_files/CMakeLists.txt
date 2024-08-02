cmake_minimum_required (VERSION 3.24)

# Getting my fucking paths, god damn it.
cmake_path(GET CMAKE_CURRENT_SOURCE_DIR  PARENT_PATH PYWRAPS_DIR)
cmake_path(GET PYWRAPS_DIR PARENT_PATH SRC_DIR)
cmake_path(GET SRC_DIR PARENT_PATH YAY_DIR)


find_package(Python3 REQUIRED Development)
if (NOT Python3_FOUND)
  message(FATAL_ERROR "Python not found")
endif()


add_library(pyapi
    python_wrappers.cpp
    ${YAY_DIR}/include/py_wraps
)
target_link_libraries(pyapi Python3::Python)

target_include_directories(pyapi PRIVATE ${YAY_DIR}/include/py_wraps)

message("Building wrappers")