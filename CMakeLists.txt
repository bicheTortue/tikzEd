cmake_minimum_required(VERSION 3.16)
project(tikzed LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

add_executable(tikzed
  src/main.cpp
  src/appwindow.cpp
)

target_include_directories(tikzed PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_libraries(tikzed PRIVATE ${GTKMM_LIBRARIES})
target_compile_options(tikzed PRIVATE ${GTKMM_CFLAGS_OTHER})

