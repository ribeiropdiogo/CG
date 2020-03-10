cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(generator)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/engine" "${PROJECT_SOURCE_DIR}/engine/bin")
add_subdirectory("${PROJECT_SOURCE_DIR}/parser" "${PROJECT_SOURCE_DIR}/parser/bin")
add_subdirectory("${PROJECT_SOURCE_DIR}/generator" "${PROJECT_SOURCE_DIR}/generator/bin")
