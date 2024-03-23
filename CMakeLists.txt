# cmake_minimum_required(VERSION 3.16)

# project(Plin)

# set(CMAKE_CXX_STANDARD 20)

# set(CMAKE_CXX_FLAGS  "-Iinclude -mclflushopt -mclwb -march=native -O3")

# add_executable(${PROJECT_NAME} test.cpp)

# target_link_libraries(${PROJECT_NAME} /usr/local/lib/pmdk_debug/libpmemobj.so.1 -pthread)


cmake_minimum_required(VERSION 3.16)

project(Plin)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS  "-Iinclude -mclflushopt -mclwb -march=native -O0")
add_compile_options(-fopenmp)

# 添加调试信息，测试性能时删掉
set(CMAKE_BUILD_TYPE "DEBUG")
add_definitions("-g")

link_libraries(/usr/local/lib/pmdk_debug/libpmemobj.so.1)
add_link_options(-pthread -fopenmp)

# add_executable(${PROJECT_NAME} test.cpp)

# target_link_libraries(${PROJECT_NAME} /usr/local/lib/pmdk_debug/libpmemobj.so.1 -pthread)

include_directories(include)

add_subdirectory(test)
