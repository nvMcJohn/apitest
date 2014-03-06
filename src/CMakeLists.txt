
include( ${apitest_SOURCE_DIR}/build/CMakeCommon.txt )

set( SHADERS
    ../bin/Shaders/glsl/cubes_gl_bindless_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_bindless_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_bindless_indirect_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_bindless_indirect_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_buffer_range_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_buffer_range_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_buffer_storage_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_buffer_storage_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_dynamic_buffer_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_dynamic_buffer_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_map_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_map_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_multi_draw_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_multi_draw_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_tex_coord_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_tex_coord_fs.glsl
    ../bin/Shaders/glsl/cubes_gl_uniform_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_uniform_fs.glsl
    ../bin/Shaders/glsl/streaming_vb_gl_vs.glsl
    ../bin/Shaders/glsl/streaming_vb_gl_fs.glsl
    ../bin/Shaders/glsl/textures_gl_bindless_vs.glsl
    ../bin/Shaders/glsl/textures_gl_bindless_fs.glsl
    ../bin/Shaders/glsl/textures_gl_bindless_multidraw_vs.glsl
    ../bin/Shaders/glsl/textures_gl_bindless_multidraw_fs.glsl
    ../bin/Shaders/glsl/textures_gl_naive_vs.glsl
    ../bin/Shaders/glsl/textures_gl_naive_fs.glsl
    ../bin/Shaders/glsl/textures_gl_naive_uniform_vs.glsl
    ../bin/Shaders/glsl/textures_gl_naive_uniform_fs.glsl
    ../bin/Shaders/glsl/textures_gl_notex_vs.glsl
    ../bin/Shaders/glsl/textures_gl_notex_fs.glsl
    ../bin/Shaders/glsl/textures_gl_notex_uniform_vs.glsl
    ../bin/Shaders/glsl/textures_gl_notex_uniform_fs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_vs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_fs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_uniform_vs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_uniform_fs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_multidraw_vs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_multidraw_fs.glsl
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_vs.glsl    
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_fs.glsl
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_multidraw_vs.glsl
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_multidraw_fs.glsl

	../bin/Shaders/hlsl/streaming_vb_d3d11_vs.hlsl
    ../bin/Shaders/hlsl/streaming_vb_d3d11_ps.hlsl
)

set( SRCS
    framework/apitest.cpp
	framework/appstate.cpp
	framework/bufferlock.cpp
    framework/console.cpp
    framework/factory.cpp
    framework/gfx_gl.cpp
    framework/options.cpp
    framework/os.cpp
    framework/sparse_bindless_texarray.cpp
    framework/timer.cpp
    framework/wgl.cpp

    problems/dynamicstreaming.cpp
    problems/null.cpp
    problems/problem.cpp
    problems/texturedquads.cpp
    problems/untexturedobjects.cpp

    solutions/dynamicstreamingsoln.cpp
    solutions/nullsoln.cpp
    solutions/solution.cpp
    solutions/texturedquadssoln.cpp
    solutions/untexturedobjectssoln.cpp

	solutions/dynamicstreaming/gl/buffersubdata.cpp
    solutions/dynamicstreaming/gl/mapunsynchronized.cpp
    solutions/dynamicstreaming/gl/mappersistent.cpp

    solutions/texturedquads/gl/bindless.cpp
    solutions/texturedquads/gl/bindlessmultidraw.cpp
    solutions/texturedquads/gl/naive.cpp
    solutions/texturedquads/gl/naiveuniform.cpp
    solutions/texturedquads/gl/notex.cpp
    solutions/texturedquads/gl/notexuniform.cpp
    solutions/texturedquads/gl/texturearray.cpp
    solutions/texturedquads/gl/texturearrayuniform.cpp
    solutions/texturedquads/gl/texturearraymultidraw.cpp
    solutions/texturedquads/gl/sparsebindlesstexturearray.cpp
    solutions/texturedquads/gl/sparsebindlesstexturearraymultidraw.cpp

    solutions/untexturedobjects/gl/bindless.cpp
    solutions/untexturedobjects/gl/bindlessindirect.cpp
    solutions/untexturedobjects/gl/bufferrange.cpp
    solutions/untexturedobjects/gl/bufferstorage.cpp
    solutions/untexturedobjects/gl/dynamicbuffer.cpp
    solutions/untexturedobjects/gl/drawloop.cpp
    solutions/untexturedobjects/gl/mapunsynchronized.cpp
    solutions/untexturedobjects/gl/mappersistent.cpp
    solutions/untexturedobjects/gl/multidraw.cpp
    solutions/untexturedobjects/gl/multidrawbuffer.cpp
    solutions/untexturedobjects/gl/texcoord.cpp
    solutions/untexturedobjects/gl/uniform.cpp

    ${SHADERS}
)

if( apitest_WITH_D3D11 )
	set( SRCS ${SRCS} 
		framework/gfx_dx11.cpp

		solutions/dynamicstreaming/d3d11/mapnooverwrite.cpp
	)
endif( apitest_WITH_D3D11 )


# Setup PCH
add_msvc_precompiled_header( "pch.h" "pch.cpp" SRCS )

# Add include directories for third party software (SDL)
include_directories( "${SDL_INCLUDE_DIR}" )

# Build our executable
add_executable( apitest ${SRCS} ${HDRS} )

source_group( "Framework" FILES 
    framework/apitest.cpp
	framework/appstate.cpp
	framework/appstate.h
	framework/bufferlock.cpp
	framework/bufferlock.h
    framework/console.cpp
    framework/console.h
    framework/dds_helper.h
    framework/factory.cpp
    framework/factory.h
    framework/gfx.h
    framework/gfx_dx11.cpp
    framework/gfx_dx11.h
    framework/gfx_gl.cpp
    framework/gfx_gl.h
    framework/mathlib.h
    framework/options.cpp
    framework/options.h
    framework/os.cpp
    framework/os.h
    framework/sparse_bindless_texarray.cpp
    framework/sparse_bindless_texarray.h
    framework/timer.cpp
    framework/timer.h
    framework/wgl.cpp
    framework/wgl.h
)

source_group( "GL" FILES
    GL/glcorearb.h
    GL/glext.h
    GL/wglext.h
)

source_group( "PCH" FILES 
    pch.cpp
    pch.h
)

source_group( "Problems" FILES 
    problems/dynamicstreaming.cpp
    problems/dynamicstreaming.h
    problems/null.cpp
    problems/null.h
    problems/problem.cpp
    problems/problem.h
    problems/texturedquads.cpp
    problems/texturedquads.h
    problems/untexturedobjects.cpp
    problems/untexturedobjects.h
)

source_group( "Solutions" FILES 
    solutions/solution.cpp
    solutions/solution.h
)

source_group( "Solutions\\DynamicStreaming" FILES 
    solutions/dynamicstreamingsoln.cpp
    solutions/dynamicstreamingsoln.h
)

source_group( "Solutions\\DynamicStreaming\\D3D11" FILES
    ../bin/Shaders/hlsl/streaming_vb_d3d11_vs.hlsl
    ../bin/Shaders/hlsl/streaming_vb_d3d11_ps.hlsl
)

source_group( "Solutions\\DynamicStreaming\\D3D11\\MapNoOverwrite" FILES
    solutions/dynamicstreaming/d3d11/mapnooverwrite.cpp
    solutions/dynamicstreaming/d3d11/mapnooverwrite.h
)

source_group( "Solutions\\DynamicStreaming\\GL" FILES
    ../bin/Shaders/glsl/streaming_vb_gl_vs.glsl
    ../bin/Shaders/glsl/streaming_vb_gl_fs.glsl
)

source_group( "Solutions\\DynamicStreaming\\GL\\BufferSubData" FILES 
    solutions/dynamicstreaming/gl/buffersubdata.cpp
    solutions/dynamicstreaming/gl/buffersubdata.h
)

source_group( "Solutions\\DynamicStreaming\\GL\\MapUnsynchronized" FILES 
    solutions/dynamicstreaming/gl/mapunsynchronized.cpp
    solutions/dynamicstreaming/gl/mapunsynchronized.h
)

source_group( "Solutions\\DynamicStreaming\\GL\\MapPersistent" FILES 
    solutions/dynamicstreaming/gl/mappersistent.cpp
    solutions/dynamicstreaming/gl/mappersistent.h
)

source_group( "Solutions\\Null" FILES 
    solutions/nullsoln.cpp
    solutions/nullsoln.h
)

source_group( "Solutions\\TexturedQuads" FILES 
    solutions/texturedquadssoln.cpp
    solutions/texturedquadssoln.h
)

source_group( "Solutions\\TexturedQuads\\GL\\Bindless" FILES 
    solutions/texturedquads/gl/bindless.cpp
    solutions/texturedquads/gl/bindless.h
    ../bin/Shaders/glsl/textures_gl_bindless_vs.glsl
    ../bin/Shaders/glsl/textures_gl_bindless_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\BindlessMultidraw" FILES 
    solutions/texturedquads/gl/bindlessmultidraw.cpp
    solutions/texturedquads/gl/bindlessmultidraw.h
    ../bin/Shaders/glsl/textures_gl_bindless_multidraw_vs.glsl
    ../bin/Shaders/glsl/textures_gl_bindless_multidraw_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\Naive" FILES 
    solutions/texturedquads/gl/naive.cpp
    solutions/texturedquads/gl/naive.h
    ../bin/Shaders/glsl/textures_gl_naive_vs.glsl
    ../bin/Shaders/glsl/textures_gl_naive_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\NaiveUniform" FILES 
    solutions/texturedquads/gl/naiveuniform.cpp
    solutions/texturedquads/gl/naiveuniform.h
    ../bin/Shaders/glsl/textures_gl_naive_uniform_vs.glsl
    ../bin/Shaders/glsl/textures_gl_naive_uniform_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\Notex (SOL)" FILES 
    solutions/texturedquads/gl/notex.cpp
    solutions/texturedquads/gl/notex.h
    ../bin/Shaders/glsl/textures_gl_notex_vs.glsl
    ../bin/Shaders/glsl/textures_gl_notex_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\NotexUniform" FILES 
    solutions/texturedquads/gl/notexuniform.cpp
    solutions/texturedquads/gl/notexuniform.h
    ../bin/Shaders/glsl/textures_gl_notex_uniform_vs.glsl
    ../bin/Shaders/glsl/textures_gl_notex_uniform_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\TextureArray" FILES 
    solutions/texturedquads/gl/texturearray.cpp
    solutions/texturedquads/gl/texturearray.h
    ../bin/Shaders/glsl/textures_gl_texture_array_vs.glsl    
    ../bin/Shaders/glsl/textures_gl_texture_array_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\TextureArrayUniform" FILES 
    solutions/texturedquads/gl/texturearrayuniform.cpp
    solutions/texturedquads/gl/texturearrayuniform.h
    ../bin/Shaders/glsl/textures_gl_texture_array_uniform_vs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_uniform_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\TextureArrayMultidraw" FILES 
    solutions/texturedquads/gl/texturearraymultidraw.cpp
    solutions/texturedquads/gl/texturearraymultidraw.h
    ../bin/Shaders/glsl/textures_gl_texture_array_multidraw_vs.glsl
    ../bin/Shaders/glsl/textures_gl_texture_array_multidraw_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\SparseBindlessTextureArray" FILES 
    solutions/texturedquads/gl/sparsebindlesstexturearray.cpp
    solutions/texturedquads/gl/sparsebindlesstexturearray.h
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_vs.glsl    
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_fs.glsl
)

source_group( "Solutions\\TexturedQuads\\GL\\SparseBindlessTextureArrayMultidraw" FILES 
    solutions/texturedquads/gl/sparsebindlesstexturearraymultidraw.cpp
    solutions/texturedquads/gl/sparsebindlesstexturearraymultidraw.h
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_multidraw_vs.glsl
    ../bin/Shaders/glsl/textures_gl_sparse_bindless_texture_array_multidraw_fs.glsl
)

source_group( "Solutions\\UntexturedObjects" FILES 
    solutions/untexturedobjectssoln.cpp
    solutions/untexturedobjectssoln.h
)

source_group( "Solutions\\UntexturedObjects\\GL\\Bindless" FILES 
    solutions/untexturedobjects/gl/bindless.cpp
    solutions/untexturedobjects/gl/bindless.h
    ../bin/Shaders/glsl/cubes_gl_bindless_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_bindless_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\BindlessIndirect" FILES 
    solutions/untexturedobjects/gl/bindlessindirect.cpp
	solutions/untexturedobjects/gl/bindlessindirect.h
    ../bin/Shaders/glsl/cubes_gl_bindless_indirect_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_bindless_indirect_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\BufferRange" FILES 
    solutions/untexturedobjects/gl/bufferrange.cpp
    solutions/untexturedobjects/gl/bufferrange.h
    ../bin/Shaders/glsl/cubes_gl_buffer_range_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_buffer_range_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\BufferStorage" FILES 
    solutions/untexturedobjects/gl/bufferstorage.cpp
    solutions/untexturedobjects/gl/bufferstorage.h
    ../bin/Shaders/glsl/cubes_gl_buffer_storage_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_buffer_storage_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\DynamicBuffer" FILES 
    solutions/untexturedobjects/gl/dynamicbuffer.cpp
    solutions/untexturedobjects/gl/dynamicbuffer.h
    ../bin/Shaders/glsl/cubes_gl_dynamic_buffer_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_dynamic_buffer_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\MapUnsynchronized" FILES 
    solutions/untexturedobjects/gl/mapunsynchronized.cpp
    solutions/untexturedobjects/gl/mapunsynchronized.h
    ../bin/Shaders/glsl/cubes_gl_map_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_map_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\MapPersistent" FILES 
    solutions/untexturedobjects/gl/mappersistent.cpp
    solutions/untexturedobjects/gl/mappersistent.h
)

source_group( "Solutions\\UntexturedObjects\\GL\\DrawLoop" FILES 
    solutions/untexturedobjects/gl/drawloop.cpp
    solutions/untexturedobjects/gl/drawloop.h
)

source_group( "Solutions\\UntexturedObjects\\GL\\MultiDraw" FILES 
    solutions/untexturedobjects/gl/multidraw.cpp
    solutions/untexturedobjects/gl/multidraw.h
    ../bin/Shaders/glsl/cubes_gl_multi_draw_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_multi_draw_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\MultiDrawBuffer" FILES 
    solutions/untexturedobjects/gl/multidrawbuffer.cpp
    solutions/untexturedobjects/gl/multidrawbuffer.h
)

source_group( "Solutions\\UntexturedObjects\\GL\\TexCoord" FILES 
    solutions/untexturedobjects/gl/texcoord.cpp
    solutions/untexturedobjects/gl/texcoord.h
    ../bin/Shaders/glsl/cubes_gl_tex_coord_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_tex_coord_fs.glsl
)

source_group( "Solutions\\UntexturedObjects\\GL\\Uniforms" FILES 
    solutions/untexturedobjects/gl/uniform.cpp
    solutions/untexturedobjects/gl/uniform.h
    ../bin/Shaders/glsl/cubes_gl_uniform_vs.glsl
    ../bin/Shaders/glsl/cubes_gl_uniform_fs.glsl
)

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
add_dependencies( apitest SDL2 SDL2main SDL2deployed )

# Change the debug executable name
set_target_properties( apitest PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG apitest_d )

if( apitest_WITH_D3D11 )
	add_custom_target(
		CopyRedist
		COMMAND ${CMAKE_COMMAND}
		-E copy_if_different "${apitest_SOURCE_DIR}/thirdparty/Microsoft/D3D/x86/d3dcompiler_46.dll" "${apitest_BIN_DIR}"
	)
	add_dependencies(apitest CopyRedist)
endif()
