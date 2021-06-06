cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-std=c++14 -Ofast -g)

include_directories(src)
include_directories(src/include)

add_executable(code
        src/include/BPlusTree.h
        src/include/cache.hpp
        src/include/ErrorMessage.h
        src/include/MyString.hpp
        src/include/StoragePool.h
        src/include/separator.cpp
        src/include/separator.h
        src/include/vector.h

        src/main.cpp
        src/TrainManager.cpp
        src/TrainManager.h
        src/User.h
        src/User.cpp
        src/UserManager.h
        src/UserManager.cpp
        src/Train.h
        src/Train.cpp
        src/ManagementSystem.h
        src/ManagementSystem.cpp
        src/base.h
        )
