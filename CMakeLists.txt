cmake_minimum_required( VERSION 2.8 )

project( apitest )
set( SDL_DEPLOY_INTO ${CMAKE_SOURCE_DIR}/bin )

add_subdirectory( thirdparty/SDL2-2.0.1 )
add_subdirectory( src )
