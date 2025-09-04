cmake_minimum_required(VERSION 3.21.0)
project(cmm-script VERSION 1.0.0 LANGUAGES C CXX)
set(LIB_NAME cmm-script)
set(CLI_NAME cmm)

add_library(${LIB_NAME} STATIC)
target_compile_features(${LIB_NAME} PUBLIC cxx_std_20)
target_compile_options(${LIB_NAME} PUBLIC -Werror -Wall -Wextra)
target_sources(${LIB_NAME} PRIVATE
    data.cpp
    expression.cpp
    funs.cpp
    operand.cpp
    script.cpp
)

add_executable(${CLI_NAME} cmm.cpp)
target_compile_features(${CLI_NAME} PUBLIC cxx_std_20)
target_compile_options(${CLI_NAME} PUBLIC -Werror -Wall -Wextra)
target_link_libraries(${CLI_NAME} PRIVATE ${LIB_NAME})
