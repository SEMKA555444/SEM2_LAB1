cmake_minimum_required(VERSION 3.10)
project(dbms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    data_structures/Array.cpp
    data_structures/OneLinkedList.cpp
    data_structures/DoublyLinkedList.cpp
    data_structures/Stack.cpp
    data_structures/Queue.cpp
    data_structures/CompleteBinaryTree.cpp
)

add_executable(dbms ${SOURCES})
target_include_directories(dbms PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
