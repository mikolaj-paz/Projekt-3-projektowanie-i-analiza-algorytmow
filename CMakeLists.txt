cmake_minimum_required( VERSION 3.28 )
project( "Projekt 3 - Projektowanie i analiza algorytmow" )

find_package(OpenGL REQUIRED)

add_subdirectory( "lib/glfw" )
add_subdirectory( "lib/glad" )
add_subdirectory( "lib/glm" )

add_compile_options( -O3 )

add_executable( a WIN32 
                "src/main.cpp"
                "src/precomputed_move_data.cpp"
                "src/board.cpp"
                "src/move_generator.cpp"
                "src/bot.cpp"
                "src/shader.cpp"
                "src/texture.cpp"
                "src/resource_manager.cpp"
                "src/sprite_renderer.cpp"
                "src/game_manager.cpp" )

target_link_libraries( a glfw glad glm )
