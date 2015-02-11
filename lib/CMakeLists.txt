cmake_minimum_required(VERSION 2.8)
PROJECT(libkafka)

find_package(ZLIB REQUIRED)
find_package(Snappy REQUIRED)

set(libkafka_ROOT ${PROJECT_SOURCE_DIR}/src)

file(GLOB libkafka_HEADERS
    ${libkafka_ROOT}/*.h
    ${libkafka_ROOT}/fetch/*.h
    ${libkafka_ROOT}/metadata/*.h
    ${libkafka_ROOT}/offset/*.h
    ${libkafka_ROOT}/produce/*.h
)

file(GLOB libkafka_SOURCES
    ${libkafka_ROOT}/*.cc
    ${libkafka_ROOT}/fetch/*.cc
    ${libkafka_ROOT}/metadata/*.cc
    ${libkafka_ROOT}/offset/*.cc
    ${libkafka_ROOT}/produce/*.cc
)

if(MSVC)
add_definitions(/D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${libkafka_ROOT} ${ZLIB_INCLUDE_DIRS} ${SNAPPY_INCLUDE_DIRS})
add_library(kafka STATIC ${libkafka_HEADERS} ${libkafka_SOURCES})

install(TARGETS kafka
    ARCHIVE DESTINATION "lib"
    COMPONENT Runtime)
install(DIRECTORY ${libkafka_ROOT}/
    DESTINATION "include/libkafka"
    FILES_MATCHING PATTERN "*.h")

