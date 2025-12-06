cmake_minimum_required(VERSION 3.27)

set(YEAR_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} )

set(CMAKE_C_COMPILER /opt/homebrew/Cellar/llvm/20.1.8/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/Cellar/llvm/20.1.8/bin/clang++)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2c -Wall")

function(add_puzzle TARGET_NAME )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME})
add_executable(${TARGET_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}/main.cpp")
endfunction()

add_puzzle(01)
add_puzzle(02)
add_puzzle(03)
add_puzzle(04)
add_puzzle(05)