cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)

# choose compiler
# set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)
set(CMAKE_CXX_COMPILER /usr/local/bin/g++-12)

# https://stackoverflow.com/questions/14446495/cmake-project-structure-with-unit-tests
project (backtesting)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)