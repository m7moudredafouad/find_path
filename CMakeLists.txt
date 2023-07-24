cmake_minimum_required(VERSION 3.2)
project(2dgame)

file(GLOB SRC_FILES
     "src/*.cpp"
)

add_executable(2dgame ${SRC_FILES})
target_include_directories(2dgame PUBLIC include/)

target_link_libraries(2dgame PUBLIC sfml-graphics sfml-window sfml-system X11)