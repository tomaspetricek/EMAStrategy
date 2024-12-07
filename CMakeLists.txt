cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)

# https://stackoverflow.com/questions/14446495/cmake-project-structure-with-unit-tests
project (backtesting)
add_subdirectory (src)

enable_testing ()
add_subdirectory (test)
