
include( ${apitest_SOURCE_DIR}/build/CMakeCommon.txt )

set( SRCS
    framework/apitest.cpp
    framework/console.cpp
    framework/factory.cpp
    framework/gfx_gl.cpp
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

    solutions/untexturedobjects/gl/bindless.cpp
    solutions/untexturedobjects/gl/multidraw.cpp
)

if( MSVC )
        set( SRCS ${SRCS} 
                framework/gfx_dx11.cpp
                # streaming_vb_dx11.cpp
        )
endif( MSVC )


# Setup PCH
add_msvc_precompiled_header( "pch.h" "pch.cpp" SRCS )

# Add include directories for third party software (SDL)
include_directories( "${SDL_INCLUDE_DIR}" )

# Build our executable
add_executable( apitest ${SRCS} ${HDRS} )

source_group( "Framework" FILES 
    framework/apitest.cpp
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
    framework/os.cpp
    framework/os.h
    framework/sparse_bindless_texarray.cpp
    framework/sparse_bindless_texarray.h
    framework/timer.cpp
    framework/timer.h
    framework/wgl.cpp
    framework/wgl.h
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

source_group( "Solutions\\Null" FILES 
    solutions/nullsoln.cpp
    solutions/nullsoln.h
)

source_group( "Solutions\\TexturedQuads" FILES 
    solutions/texturedquadssoln.cpp
    solutions/texturedquadssoln.h
)

source_group( "Solutions\\UntexturedObjects" FILES 
    solutions/untexturedobjectssoln.cpp
    solutions/untexturedobjectssoln.h
)

source_group( "Solutions\\UntexturedObjects\\GL\\Bindless" FILES 
    solutions/untexturedobjects/gl/bindless.cpp
    solutions/untexturedobjects/gl/bindless.h
)

source_group( "Solutions\\UntexturedObjects\\GL\\MultiDraw" FILES 
    solutions/untexturedobjects/gl/multidraw.cpp
    solutions/untexturedobjects/gl/multidraw.h
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
add_dependencies( apitest SDL2 SDL2main )

# Change the debug executable name
set_target_properties( apitest PROPERTIES RUNTIME_OUTPUT_NAME_DEBUG apitest_d )
