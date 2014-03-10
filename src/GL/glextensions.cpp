
#include "pch.h"
#include "glextensions.h"

#include <algorithm>
#include <string>

#pragma comment (lib, "opengl32.lib")

// Define the extension variables and function pointer guts.
#define GL_EXTENSION(_ext) bool gHasExtension_##_ext = false;
#define GL_EXTENSION_FUNC(_ext, _procType, _procName) _procType _procName = nullptr;
#include "glextensions.inl"
#undef GL_EXTENSION
#undef GL_EXTENSION_FUNC

void checkExtensions(GLsizei _n, const char** _extensions, GLboolean *_available);

// --------------------------------------------------------------------------------------------------------------------
inline bool ShouldStripExtension(const std::list<std::string>& _strippedExtensions, const char* _testExtension)
{
    return std::find(_strippedExtensions.cbegin(), _strippedExtensions.cend(), std::string(_testExtension)) != _strippedExtensions.cend();
}

// --------------------------------------------------------------------------------------------------------------------
void CheckExtensions(const std::list<std::string>& _strippedExtensions)
{
    // Build a list of strings of extensions we care about.
    const char* myExtensions[] = 
    {
        #define GL_EXTENSION(_ext) "GL_" #_ext,
        #include "glextensions.inl"
        #undef GL_EXTENSION
    };

    GLboolean available[ArraySize(myExtensions)] = { 0 };

    checkExtensions(ArraySize(myExtensions), myExtensions, available);

    // Update the global variables. They are kept in sync by virtue of the fact that we are 
    // including the same definitions over and over again.
    int i = 0;
    #define GL_EXTENSION(_ext) gHasExtension_##_ext = (available[i++] != GL_FALSE) && !ShouldStripExtension(_strippedExtensions, STR(_ext));
    #include "glextensions.inl"
    #undef GL_EXTENSION

    // Hard code this one.
    gHasExtension_OpenGL = true;
}

// --------------------------------------------------------------------------------------------------------------------
void ResolveExtensions()
{
    // Walk the function list, getting the address for functions that we have the extension available for.
    #define GL_EXTENSION_FUNC(_ext, _procType, _procName) if (HasExtension(_ext)) { _procName = (_procType)SDL_GL_GetProcAddress(#_procName); }
    #include "glextensions.inl"
    #undef GL_EXTENSION_FUNC
}

// --------------------------------------------------------------------------------------------------------------------
void ResetExtensions()
{
    // Walk the function list, getting the address for functions that we have the extension available for.
    #define GL_EXTENSION(_ext) gHasExtension_##_ext = false;
    #define GL_EXTENSION_FUNC(_ext, _procType, _procName) _procName = nullptr;
    #include "glextensions.inl"
    #undef GL_EXTENSION_FUNC
    #undef GL_EXTENSION
}

// --------------------------------------------------------------------------------------------------------------------
void checkExtensions(GLsizei _n, const char** _extensions, GLboolean *_available)
{
    // We use local variants of these because we have not resolved those pointers yet (this function is called to 
    // bootstrap everything). 
    const GLubyte* (APIENTRY *glGetStringiLocal)(GLenum, GLuint) = reinterpret_cast<const GLubyte* (APIENTRY *)(GLenum, GLuint)>(SDL_GL_GetProcAddress("glGetStringi"));
    void (APIENTRY *glGetIntegervLocal)(GLenum, GLint *) = reinterpret_cast<void (APIENTRY *)(GLenum, GLint *)>(SDL_GL_GetProcAddress("glGetIntegerv"));
    
    if (!glGetStringiLocal) {
        assert(0);
        return;
    }

    GLint availableExtensions = 0;
    glGetIntegervLocal(GL_NUM_EXTENSIONS, &availableExtensions);

    for (GLsizei j = 0; j < _n; ++j) {
        // Once something is available, skip it
        if (_available[j]) {
            continue;
        }

        for (int i = 0; i < availableExtensions; ++i) {
            const char* ext = reinterpret_cast<const char*>(glGetStringiLocal(GL_EXTENSIONS, i));
            if (strcmp(_extensions[j], ext) == 0) {
                _available[j] = GL_TRUE;
                break;
            }
        }
    }
}
