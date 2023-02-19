cmake_minimum_required(VERSION 3.12)
project(main VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs/Texture)
add_subdirectory(libs/Button)

SET (deps_inc "/opt/homebrew/include/")

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_BINARY_DIR}
    ${deps_inc}
    ${PROJECT_SOURCE_DIR}
    "."
)


find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp)

link_libraries(main PRIVATE "/opt/homebrew/lib/")
target_link_libraries(main SDL2 SDL2_image SDL2_ttf SDL2_mixer)

target_include_directories(main PRIVATE libs/Texture)
target_include_directories(main PRIVATE libs/Button)

target_link_libraries(main Texture Button)
