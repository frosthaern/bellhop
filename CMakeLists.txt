cmake_minimum_required(VERSION 4.0)
project(bellhop
  VERSION 0.1
  DESCRIPTION "Bellhop - A dbus service"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
find_package(SQLite3 REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(bellhop ${SOURCES})

target_link_libraries(bellhop PRIVATE
  ${DBUS_LIBRARIES}
  ${SQLITE3_LIBRARIES}
)
target_include_directories(bellhop PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${DBUS_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
)

target_compile_options(bellhop PRIVATE ${DBUS_CFLAGS_OTHER})

if(DBUS_LIBRARY_DIRS)
  target_link_directories(bellhop PRIVATE ${DBUS_LIBRARY_DIRS})
endif()
