cmake_minimum_required(VERSION 4.0.0)
project(TTF_Reader LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED YES)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_RELEASE "-O3")

option(ONLY_LIBS "" ON)
add_subdirectory(third-party/glew)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# Needed because glfw doesn't work with wayland
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(third-party/glfw)

add_executable("TTF_Reader" 
    src/main.c
    src/misc/data_read.c
    src/misc/ttf_types.c
    src/tables/table_cmap.c
    src/tables/table_directories.c
    src/tables/table_glyph.c
    src/tables/table_head.c
    src/tables/table_hhea.c
    src/tables/table_loca.c
    src/tables/table_maxp.c
    src/tables/table_name.c
    src/tables/table_offset_subtable.c
    )

target_include_directories("TTF_Reader" PRIVATE include)
target_link_libraries("TTF_Reader" glfw GL libglew_static)


