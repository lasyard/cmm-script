cmake_minimum_required(VERSION 3.21.0)
project(cmm-script VERSION 1.0.0 LANGUAGES C CXX)

include(cmake-common/functions.cmake)

# Determine if cmm-script is built as a subproject (using add_subdirectory) or if it is the main project.
set(MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

add_lib(cmm
    data.cpp
    expression.cpp
    funs.cpp
    operand.cpp
    script.cpp
)

if(MAIN_PROJECT)
    add_executable(cmms cmm.cpp)
    target_link_libraries(cmms PRIVATE cmm)
    using_doctest(doctest)
endif()
