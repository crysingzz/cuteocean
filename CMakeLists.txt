cmake_minimum_required(VERSION 3.28)
project(ocean)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(include)



add_library(entity src/entity.cpp)
add_library(algae src/algae.cpp)
add_library(fish src/fish.cpp)
add_library(herbivore src/herbivore.cpp)
add_library(predator src/predator.cpp)
add_library(global src/global.cpp)
add_library(logger src/logger.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using GCC compiler")
    add_compile_options(-Wall -Wextra -pedantic -O2)
endif()


add_executable(ocean_main src/main.cpp)
target_link_libraries(ocean_main algae herbivore predator2 global  fish logger entity)
target_link_libraries(ocean_main sfml-graphics sfml-window sfml-system)
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites/Monocraft.ttc DESTINATION ${CMAKE_BINARY_DIR})