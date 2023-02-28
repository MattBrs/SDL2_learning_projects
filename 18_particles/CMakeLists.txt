cmake_minimum_required(VERSION 3.12)
project(main VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(libs/Texture)
add_subdirectory(libs/Button)
add_subdirectory(libs/Timer)
add_subdirectory(libs/Dot)
add_subdirectory(libs/Utils)
add_subdirectory(libs/Window)
add_subdirectory(libs/Particle)
add_subdirectory(libs/ParticleSystem)
add_subdirectory(libs/Camera)

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
target_include_directories(main PRIVATE libs/Timer)
target_include_directories(main PRIVATE libs/Dot)
target_include_directories(main PRIVATE libs/Constants)
target_include_directories(main PRIVATE libs/Utils)
target_include_directories(main PRIVATE libs/Window)
target_include_directories(main PRIVATE libs/Particle)
target_include_directories(main PRIVATE libs/ParticleSystem)
target_include_directories(main PRIVATE libs/Camera)

target_link_libraries(
    main Texture Button Timer Dot Utils Window Particle ParticleSystem Camera)
