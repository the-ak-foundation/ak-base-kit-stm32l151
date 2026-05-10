cmake_minimum_required(VERSION 3.10)
project(ThreeKingdomsSim)

set(CMAKE_CXX_STANDARD 17)

# Add define so code knows it's running on PC
add_compile_definitions(PC_SIMULATOR)

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../application/sources/app/game_rts
)

# Source files
set(SOURCES
    main.cpp
    scr_map_mock.cpp
    ../application/sources/app/game_rts/game_logic.cpp
    ../application/sources/app/game_rts/task_game.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# Link SDL2 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2)
