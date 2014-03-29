
#pragma once
#include <list>

#define XSTR(_s)    #_s
#define STR(_str)   XSTR(_str)

// This is done in two pieces. First you call CheckExtensions. Then you call ResolveFunctions.
// The value in this being in two pieces is that an application can easily call CheckExtensions
// and then override (to false) an extension to simulate it not being available on a given driver stack.
void CheckExtensions(const std::list<std::string>& _strippedExtensions);
void ResolveExtensions();
void ResetExtensions();

// A helper to tell you whether a particular extension is available.
#define HasExtension(_ext) (gHasExtension_##_ext)

// Pre-declare externed globals that specify whether an extension exists and also the function pointers for extension functions.
#define GL_EXTENSION(_ext) extern bool gHasExtension_##_ext;
#define GL_CORE_EXTENSION(_ext, _major, _minor) extern bool gHasExtension_##_ext;
#define GL_EXTENSION_FUNC(_ext, _procType, _procName) extern _procType _procName;
#include "glextensions.inl"

// These functions are available on all platforms as direct exports from the libraries. 
extern "C" {
    // 1.0 
    GLAPI void APIENTRY glCullFace (GLenum);
    GLAPI void APIENTRY glFrontFace (GLenum);
    GLAPI void APIENTRY glHint (GLenum, GLenum);
    GLAPI void APIENTRY glLineWidth (GLfloat);
    GLAPI void APIENTRY glPointSize (GLfloat);
    GLAPI void APIENTRY glPolygonMode (GLenum, GLenum);
    GLAPI void APIENTRY glScissor (GLint, GLint, GLsizei, GLsizei);
    GLAPI void APIENTRY glTexParameterf (GLenum, GLenum, GLfloat);
    GLAPI void APIENTRY glTexParameterfv (GLenum, GLenum, const GLfloat *);
    GLAPI void APIENTRY glTexParameteri (GLenum, GLenum, GLint);
    GLAPI void APIENTRY glTexParameteriv (GLenum, GLenum, const GLint *);
    GLAPI void APIENTRY glTexImage1D (GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
    GLAPI void APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
    GLAPI void APIENTRY glDrawBuffer (GLenum);
    GLAPI void APIENTRY glClear (GLbitfield);
    GLAPI void APIENTRY glClearColor (GLclampf, GLclampf, GLclampf, GLclampf);
    GLAPI void APIENTRY glClearStencil (GLint);
    GLAPI void APIENTRY glClearDepth (GLclampd);
    GLAPI void APIENTRY glStencilMask (GLuint);
    GLAPI void APIENTRY glColorMask (GLboolean, GLboolean, GLboolean, GLboolean);
    GLAPI void APIENTRY glDepthMask (GLboolean);
    GLAPI void APIENTRY glDisable (GLenum);
    GLAPI void APIENTRY glEnable (GLenum);
    GLAPI void APIENTRY glFinish (void);
    GLAPI void APIENTRY glFlush (void);
    GLAPI void APIENTRY glBlendFunc (GLenum, GLenum);
    GLAPI void APIENTRY glLogicOp (GLenum);
    GLAPI void APIENTRY glStencilFunc (GLenum, GLint, GLuint);
    GLAPI void APIENTRY glStencilOp (GLenum, GLenum, GLenum);
    GLAPI void APIENTRY glDepthFunc (GLenum);
    GLAPI void APIENTRY glPixelStoref (GLenum, GLfloat);
    GLAPI void APIENTRY glPixelStorei (GLenum, GLint);
    GLAPI void APIENTRY glReadBuffer (GLenum);
    GLAPI void APIENTRY glReadPixels (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *);
    GLAPI void APIENTRY glGetBooleanv (GLenum, GLboolean *);
    GLAPI void APIENTRY glGetDoublev (GLenum, GLdouble *);
    GLAPI GLenum APIENTRY glGetError (void);
    GLAPI void APIENTRY glGetFloatv (GLenum, GLfloat *);
    GLAPI void APIENTRY glGetIntegerv (GLenum, GLint *);
    GLAPI const GLubyte * APIENTRY glGetString (GLenum);
    GLAPI void APIENTRY glGetTexImage (GLenum, GLint, GLenum, GLenum, GLvoid *);
    GLAPI void APIENTRY glGetTexParameterfv (GLenum, GLenum, GLfloat *);
    GLAPI void APIENTRY glGetTexParameteriv (GLenum, GLenum, GLint *);
    GLAPI void APIENTRY glGetTexLevelParameterfv (GLenum, GLint, GLenum, GLfloat *);
    GLAPI void APIENTRY glGetTexLevelParameteriv (GLenum, GLint, GLenum, GLint *);
    GLAPI GLboolean APIENTRY glIsEnabled (GLenum);
    GLAPI void APIENTRY glDepthRange (GLclampd, GLclampd);
    GLAPI void APIENTRY glViewport (GLint, GLint, GLsizei, GLsizei);

    // 1.1
    GLAPI void APIENTRY glDrawArrays (GLenum, GLint, GLsizei);
    GLAPI void APIENTRY glDrawElements (GLenum, GLsizei, GLenum, const GLvoid *);
    GLAPI void APIENTRY glGetPointerv (GLenum, GLvoid* *);
    GLAPI void APIENTRY glPolygonOffset (GLfloat, GLfloat);
    GLAPI void APIENTRY glCopyTexImage1D (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
    GLAPI void APIENTRY glCopyTexImage2D (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
    GLAPI void APIENTRY glCopyTexSubImage1D (GLenum, GLint, GLint, GLint, GLint, GLsizei);
    GLAPI void APIENTRY glCopyTexSubImage2D (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
    GLAPI void APIENTRY glTexSubImage1D (GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *);
    GLAPI void APIENTRY glTexSubImage2D (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
    GLAPI void APIENTRY glBindTexture (GLenum, GLuint);
    GLAPI void APIENTRY glDeleteTextures (GLsizei, const GLuint *);
    GLAPI void APIENTRY glGenTextures (GLsizei, GLuint *);
    GLAPI GLboolean APIENTRY glIsTexture (GLuint);

    // Needed for nVidia bindless
    GLAPI void APIENTRY glEnableClientState (GLenum);
    GLAPI void APIENTRY glDisableClientState(GLenum);
};

// --------------------------------------------------------------------------------------------------------------------
// Indirect structures
struct DrawArraysIndirectCommand
{
    GLuint count;
    GLuint primCount;
    GLuint first;
    GLuint baseInstance;
};

// --------------------------------------------------------------------------------------------------------------------
struct DrawElementsIndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

// --------------------------------------------------------------------------------------------------------------------
struct BindlessPtrNV
{
    GLuint   index;
    GLuint   reserved; 
    GLuint64 address;
    GLuint64 length;
};

// --------------------------------------------------------------------------------------------------------------------
struct DrawArraysIndirectBindlessCommandNV
{
    DrawArraysIndirectCommand   cmd;
    GLuint                      reserved; 
    //BindlessPtrNV               vertexBuffers[];
};

// --------------------------------------------------------------------------------------------------------------------
struct DrawElementsIndirectBindlessCommandNV
{
    DrawElementsIndirectCommand cmd;
    GLuint                      reserved; 
    BindlessPtrNV               indexBuffer;
    //BindlessPtrNV               vertexBuffers[];
};
