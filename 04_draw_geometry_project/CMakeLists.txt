cmake_minimum_required(VERSION 3.12)
project(main VERSION 1.0.0)

SET (deps_inc "/opt/homebrew/include/")

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${deps_inc}
    ${PROJECT_SOURCE_DIR}
    )


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)


add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)
link_libraries(main PRIVATE "/opt/homebrew/lib/")
target_link_libraries(main SDL2 SDL2_image)
