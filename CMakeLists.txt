cmake_minimum_required(VERSION 3.20)
project(Projet)

set(CMAKE_C_STANDARD 99)

# Inclusion de la librairie MYSQL Connector C 6.11
set(MYSQL_DIR "C:/Program Files/MySQL/MySQL Connector C 6.1"
        CACHE PATH "The path to the MySQL C API library")
include_directories(${MYSQL_DIR}/include)
find_library(mysql NAMES libmysql PATHS ${MYSQL_DIR}/lib)
message(STATUS "mysql library: " ${mysql})

find_package(PkgConfig REQUIRED)

# Inclusion de la librairie GTK
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

# Inclusion de la librairie curl
pkg_check_modules(CURL libcurl REQUIRED)
include_directories(SYSTEM ${CURL_INCLUDE_DIRS})
message(STATUS "mysql library: " ${CURL_LIBRARY_DIRS})

add_executable(Projet main.c window.c window.h)

target_link_libraries(Projet ${GTK3_LIBRARIES} ${CURL_LIBRARIES})