cmake_minimum_required( VERSION 3.28 )
project( "Projekt 3 - Projektowanie i analiza algorytmow" )

find_package(OpenGL REQUIRED)

add_subdirectory( "lib/glfw" )
add_subdirectory( "lib/glad" )

add_executable( a WIN32 "src/main.cpp" )

# target_link_options(a PRIVATE "-lopengl32" )

target_link_libraries( a glfw glad )
