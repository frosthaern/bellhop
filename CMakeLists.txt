cmake_minimum_required(VERSION 4.0)
project(bellhop
  VERSION 0.1
  DESCRIPTION "Bellhop - A dbus service"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
find_package(SQLite3 REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
include_directories(bellhop
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${DBUS_INCLUDE_DIRS}
  ${SQLITE3_INCLUDE_DIRS}
)

add_executable(bellhop ${SOURCES})
target_link_libraries(bellhop PRIVATE
  ${DBUS_LIBRARIES}
  ${SQLITE3_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${DBUS_INCLUDE_DIRS}
    ${SQLITE3_INCLUDE_DIRS}
)
