
include( ${apitest_SOURCE_DIR}/build/CMakeCommon.txt )

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
        gfx_gl.cpp
        sparse_bindless_texarray.cpp
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

if( MSVC )
        set( SRCS ${SRCS} 
                gfx_dx11.cpp
                streaming_vb_dx11.cpp
        )
endif( MSVC )


# Setup PCH
add_msvc_precompiled_header( "pch.h" "pch.cpp" SRCS )

# Add include directories for third party software (SDL)
include_directories( "${SDL_INCLUDE_DIR}" )

# Build our executable
add_executable( apitest ${SRCS} ${HDRS} )

if( MSVC )
        target_link_libraries( apitest ${SDL_LIBRARY_DIR}/SDL2.lib )
        target_link_libraries( apitest ${SDL_LIBRARY_DIR}/SDL2main.lib )
else()
        target_link_libraries( apitest ${SDL_LIBRARY_DIR}/libSDL2.a )
        target_link_libraries( apitest ${SDL_LIBRARY_DIR}/libSDL2main.a )
endif( MSVC )

if( APPLE )
        find_library( LIB_CARBON Carbon )
        find_library( LIB_COCOA Cocoa )
        find_library( LIB_COREAUDIO CoreAudio )        
        find_library( LIB_FORCEFEEDBACK ForceFeedback )
        find_library( LIB_IOKIT IOKit )
        find_library( LIB_OGL OpenGL )
        mark_as_advanced( LIB_COCOA LIB_OGL LIB_IOKIT )
        set( EXTRA_LIBS ${LIB_CARBON} ${LIB_COCOA} ${LIB_COREAUDIO} ${LIB_FORCEFEEDBACK} ${LIB_IOKIT} ${LIB_OGL} ${EXTRA_LIBS} )

        target_link_libraries( apitest ${EXTRA_LIBS} )
elseif( UNIX )
        target_link_libraries( apitest GL pthread dl )
endif( APPLE )

# Don't know why these don't get picked up automatically. Don't care. 
add_dependencies( apitest SDL2 SDL2main )

# Change the debug executable name
set_target_properties( apitest PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG apitest_d )
