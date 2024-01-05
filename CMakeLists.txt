cmake_minimum_required(VERSION 3.5.0)
project(ludo VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(ludo
    "src/glad/glad.c"
    "src/animation.cpp"
    "src/board.cpp"
    "src/input.cpp"
    "src/main.cpp"
    "src/scene.cpp"
    "src/scenes/match_scene.cpp"
    "src/scenes/test_scene.cpp"
    "src/screen.cpp"
    "src/sprite.cpp"
    "src/time.cpp"
)

target_include_directories(ludo PRIVATE
    "include"
)

target_link_libraries(ludo
    glfw
)

add_custom_command(TARGET ludo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets"
    "${CMAKE_BINARY_DIR}/assets")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
