
include( ${apitest_SOURCE_DIR}/build/CMakeCommon.txt )

set( SDL_INCLUDE_DIR ${apitest_SOURCE_DIR}/thirdparty/SDL2-2.0.1/include)

find_package(SDL REQUIRED)

set( SRCS
		api_speed_test.cpp
		console.cpp
		cubes_gl_bindless.cpp
        cubes_gl_bindless_indirect.cpp
        cubes_gl_buffer_range.cpp
        cubes_gl_buffer_storage.cpp
        cubes_gl_dynamic_buffer.cpp
        cubes_gl_multi_draw.cpp
        cubes_gl_tex_coord.cpp
        cubes_gl_uniform.cpp
        gfx_dx11.cpp
        gfx_gl.cpp
        sparse_bindless_texarray.cpp
        streaming_vb_dx11.cpp
        streaming_vb_gl.cpp
        textures_gl_bindless.cpp
        textures_gl_bindless_multidraw.cpp
        textures_gl_forward.cpp
        textures_gl_notex.cpp
        textures_gl_sparse_bindless_texture_array.cpp
        textures_gl_sparse_bindless_texture_array_multidraw.cpp
        timer.cpp
        wgl.cpp
)

# Add include directories for third party software (SDL)
include_directories( "${SDL_INCLUDE_DIR}" )

# Build our executable
add_executable( apitest ${SRCS} ${HDRS} )

target_link_libraries( apitest ${SDL_LIBRARY}/Debug/SDL2 )
target_link_libraries( apitest ${SDL_LIBRARY}/Debug/SDL2main )

# Change the debug executable name
set_target_properties( apitest PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG apitest_d )
