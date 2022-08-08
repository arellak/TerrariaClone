cmake_minimum_required(VERSION 3.22)
project(TerrariaKlon)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib 4.0 QUIET)

if(NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if(NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

file(GLOB resources "res/*")
file(COPY ${resources} DESTINATION "${CMAKE_BINARY_DIR}/res")


add_executable(TerrariaKlon src/main.cpp src/Game.cpp src/Game.h)
target_link_libraries(TerrariaKlon raylib)