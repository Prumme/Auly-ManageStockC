cmake_minimum_required(VERSION 3.20)
project(Projet)

set(CMAKE_C_STANDARD 99)

find_package(PkgConfig REQUIRED)

#Inclusion de la librairie GTK
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})
message(STATUS "GTK include dirs: " ${GTK3_INCLUDE_DIRS})
message(STATUS "GTK library dirs: " ${GTK3_LIBRARIES})
message(STATUS "GTK clflags: " ${GTK3_CFLAGS_OTHER})
message(STATUS "")


#Inclusion de la librairie curl
pkg_check_modules(CURL libcurl REQUIRED)
include_directories(SYSTEM ${CURL_INCLUDE_DIRS})
message(STATUS "CURL include dirs: " ${CURL_INCLUDE_DIRS})
message(STATUS "CURL library dirs: " ${CURL_LIBRARIES})
message(STATUS "CURL clflags: " ${CURL_CFLAGS_OTHER})
message(STATUS "")


# Inclusion de la librairie MYSQL Connector C
pkg_check_modules(libmysql REQUIRED mysqlclient)
include_directories(${libmysql_INCLUDE_DIRS})
message(STATUS "libmysql include dirs: " ${libmysql_INCLUDE_DIRS})
message(STATUS "libmysql library dirs: " ${libmysql_LIBRARY_DIRS})
message(STATUS "libmysql clflags: " ${libmysql_CFLAGS_OTHER})
message(STATUS "")

add_executable(Projet main.c)

target_link_libraries(Projet ${GTK3_LIBRARIES} ${CURL_LIBRARIES} /usr/lib/x86_64-linux-gnu/libmysqlclient.so.21)