cmake_minimum_required(VERSION 3.19)
project(TicketSystem_2021)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(src/include)

add_executable(TicketSystem_2021
        src/include/BPlusTree.h
        src/include/cache.hpp
        src/include/ErrorMessage.h
        src/include/FileStorage.h
        src/include/HashMap.cpp
        src/include/HashMap.h
        src/include/MyString.hpp
        src/include/StoragePool.h
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
        )
