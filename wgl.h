// Windows interface to OpenGL

#pragma once

#include "GL/glcorearb.h"
#include "GL/wglext.h"

// --------------------------------------------------------------------------------------------------------------------
// GL 1.0
extern "C" {
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
}

// --------------------------------------------------------------------------------------------------------------------
// GL 1.1
extern "C" {
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
};

// --------------------------------------------------------------------------------------------------------------------
// GL 1.2
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLTEXIMAGE3DPROC glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;

// --------------------------------------------------------------------------------------------------------------------
// GL 1.3
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;

// --------------------------------------------------------------------------------------------------------------------
// GL 1.4
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
extern PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
extern PFNGLPOINTPARAMETERFPROC glPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
extern PFNGLPOINTPARAMETERIPROC glPointParameteri;
extern PFNGLPOINTPARAMETERIVPROC glPointParameteriv;
extern PFNGLBLENDCOLORPROC glBlendColor;
extern PFNGLBLENDEQUATIONPROC glBlendEquation;

// --------------------------------------------------------------------------------------------------------------------
// GL 1.5
extern PFNGLGENQUERIESPROC glGenQueries;
extern PFNGLDELETEQUERIESPROC glDeleteQueries;
extern PFNGLISQUERYPROC glIsQuery;
extern PFNGLBEGINQUERYPROC glBeginQuery;
extern PFNGLENDQUERYPROC glEndQuery;
extern PFNGLGETQUERYIVPROC glGetQueryiv;
extern PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLISBUFFERPROC glIsBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
extern PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;

// --------------------------------------------------------------------------------------------------------------------
// GL 2.0
extern PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
extern PFNGLDRAWBUFFERSPROC glDrawBuffers;
extern PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
extern PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
extern PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
extern PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETSHADERSOURCEPROC glGetShaderSource;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLGETUNIFORMFVPROC glGetUniformfv;
extern PFNGLGETUNIFORMIVPROC glGetUniformiv;
extern PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv;
extern PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
extern PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
extern PFNGLISPROGRAMPROC glIsProgram;
extern PFNGLISSHADERPROC glIsShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM2IPROC glUniform2i;
extern PFNGLUNIFORM3IPROC glUniform3i;
extern PFNGLUNIFORM4IPROC glUniform4i;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORM1IVPROC glUniform1iv;
extern PFNGLUNIFORM2IVPROC glUniform2iv;
extern PFNGLUNIFORM3IVPROC glUniform3iv;
extern PFNGLUNIFORM4IVPROC glUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLVALIDATEPROGRAMPROC glValidateProgram;
extern PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d;
extern PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv;
extern PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
extern PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s;
extern PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv;
extern PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d;
extern PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv;
extern PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
extern PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
extern PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s;
extern PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv;
extern PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d;
extern PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv;
extern PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
extern PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
extern PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s;
extern PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv;
extern PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv;
extern PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv;
extern PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv;
extern PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub;
extern PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv;
extern PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv;
extern PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv;
extern PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv;
extern PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d;
extern PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv;
extern PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
extern PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv;
extern PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s;
extern PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv;
extern PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
extern PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv;
extern PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

// --------------------------------------------------------------------------------------------------------------------
// GL 2.1
extern PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
extern PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
extern PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
extern PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
extern PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
extern PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.0
extern PFNGLCOLORMASKIPROC glColorMaski;
extern PFNGLGETBOOLEANI_VPROC glGetBooleani_v;
extern PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
extern PFNGLENABLEIPROC glEnablei;
extern PFNGLDISABLEIPROC glDisablei;
extern PFNGLISENABLEDIPROC glIsEnabledi;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
extern PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
extern PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
extern PFNGLCLAMPCOLORPROC glClampColor;
extern PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
extern PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender;
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
extern PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv;
extern PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv;
extern PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i;
extern PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i;
extern PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i;
extern PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i;
extern PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui;
extern PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui;
extern PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui;
extern PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui;
extern PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv;
extern PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv;
extern PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv;
extern PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv;
extern PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv;
extern PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv;
extern PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv;
extern PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv;
extern PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv;
extern PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv;
extern PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv;
extern PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv;
extern PFNGLGETUNIFORMUIVPROC glGetUniformuiv;
extern PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
extern PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation;
extern PFNGLUNIFORM1UIPROC glUniform1ui;
extern PFNGLUNIFORM2UIPROC glUniform2ui;
extern PFNGLUNIFORM3UIPROC glUniform3ui;
extern PFNGLUNIFORM4UIPROC glUniform4ui;
extern PFNGLUNIFORM1UIVPROC glUniform1uiv;
extern PFNGLUNIFORM2UIVPROC glUniform2uiv;
extern PFNGLUNIFORM3UIVPROC glUniform3uiv;
extern PFNGLUNIFORM4UIVPROC glUniform4uiv;
extern PFNGLTEXPARAMETERIIVPROC glTexParameterIiv;
extern PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv;
extern PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv;
extern PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv;
extern PFNGLCLEARBUFFERIVPROC glClearBufferiv;
extern PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
extern PFNGLCLEARBUFFERFVPROC glClearBufferfv;
extern PFNGLCLEARBUFFERFIPROC glClearBufferfi;
extern PFNGLGETSTRINGIPROC glGetStringi;

// ARB_framebuffer_object
extern PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
extern PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;

// ARB_map_buffer_range
extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;

// ARB_vertex_array_object
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLISVERTEXARRAYPROC glIsVertexArray;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.1
extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
extern PFNGLTEXBUFFERPROC glTexBuffer;
extern PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex;

// ARB_copy_buffer
extern PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;

// ARB_uniform_buffer_object
extern PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName;
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.2
extern PFNGLGETINTEGER64I_VPROC glGetInteger64i_v;
extern PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v;
extern PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;

// ARB_draw_elements_base_vertex
extern PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex;

// ARB_provoking_vertex
extern PFNGLPROVOKINGVERTEXPROC glProvokingVertex;

// ARB_sync
extern PFNGLFENCESYNCPROC glFenceSync;
extern PFNGLISSYNCPROC glIsSync;
extern PFNGLDELETESYNCPROC glDeleteSync;
extern PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
extern PFNGLWAITSYNCPROC glWaitSync;
extern PFNGLGETINTEGER64VPROC glGetInteger64v;
extern PFNGLGETSYNCIVPROC glGetSynciv;

// ARB_texture_multisample
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample;
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample;
extern PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv;
extern PFNGLSAMPLEMASKIPROC glSampleMaski;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.3
extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;

// ARB_blend_func_extended
extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
extern PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex;

// ARB_sampler_objects
extern PFNGLGENSAMPLERSPROC glGenSamplers;
extern PFNGLDELETESAMPLERSPROC glDeleteSamplers;
extern PFNGLISSAMPLERPROC glIsSampler;
extern PFNGLBINDSAMPLERPROC glBindSampler;
extern PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
extern PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv;
extern PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;
extern PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv;
extern PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv;
extern PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv;
extern PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv;
extern PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv;
extern PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv;

// ARB_timer_query
extern PFNGLQUERYCOUNTERPROC glQueryCounter;
extern PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v;
extern PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v;

// ARB_vertex_type_2_10_10_10_rev
extern PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui;
extern PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv;
extern PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui;
extern PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv;
extern PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui;
extern PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv;
extern PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui;
extern PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.0
extern PFNGLMINSAMPLESHADINGPROC glMinSampleShading;
extern PFNGLBLENDEQUATIONIPROC glBlendEquationi;
extern PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei;
extern PFNGLBLENDFUNCIPROC glBlendFunci;
extern PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei;

// ARB_draw_indirect
extern PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect;
extern PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;

// ARB_gpu_shader_fp64
extern PFNGLUNIFORM1DPROC glUniform1d;
extern PFNGLUNIFORM2DPROC glUniform2d;
extern PFNGLUNIFORM3DPROC glUniform3d;
extern PFNGLUNIFORM4DPROC glUniform4d;
extern PFNGLUNIFORM1DVPROC glUniform1dv;
extern PFNGLUNIFORM2DVPROC glUniform2dv;
extern PFNGLUNIFORM3DVPROC glUniform3dv;
extern PFNGLUNIFORM4DVPROC glUniform4dv;
extern PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv;
extern PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv;
extern PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;
extern PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv;
extern PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv;
extern PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv;
extern PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv;
extern PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv;
extern PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv;
extern PFNGLGETUNIFORMDVPROC glGetUniformdv;

// ARB_shader_subroutine
extern PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation;
extern PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex;
extern PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv;
extern PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
extern PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName;
extern PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv;
extern PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv;
extern PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv;

// ARB_tessellation_shader
extern PFNGLPATCHPARAMETERIPROC glPatchParameteri;
extern PFNGLPATCHPARAMETERFVPROC glPatchParameterfv;

// ARB_transform_feedback2
extern PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
extern PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks;
extern PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
extern PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback;
extern PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback;
extern PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback;
extern PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;

// ARB_transform_feedback3
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
extern PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed;
extern PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed;
extern PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.1

// ARB_ES2_compatibility
extern PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
extern PFNGLSHADERBINARYPROC glShaderBinary;
extern PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
extern PFNGLDEPTHRANGEFPROC glDepthRangef;
extern PFNGLCLEARDEPTHFPROC glClearDepthf;

// ARB_get_program_binary
extern PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC glProgramBinary;

// ARB_separate_shader_objects
extern PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
extern PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram;
extern PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
extern PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
extern PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
extern PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
extern PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
extern PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
extern PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
extern PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
extern PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
extern PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
extern PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
extern PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
extern PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
extern PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv;
extern PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
extern PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
extern PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
extern PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
extern PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d;
extern PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
extern PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
extern PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv;
extern PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
extern PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
extern PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
extern PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
extern PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d;
extern PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
extern PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
extern PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
extern PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;
extern PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
extern PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;
extern PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
extern PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d;
extern PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;
extern PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;
extern PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;
extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
extern PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;

// ARB_vertex_attrib_64bit
extern PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d;
extern PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d;
extern PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d;
extern PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d;
extern PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv;
extern PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv;
extern PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv;
extern PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv;
extern PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;
extern PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv;

// ARB_viewport_array
extern PFNGLVIEWPORTARRAYVPROC glViewportArrayv;
extern PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf;
extern PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
extern PFNGLSCISSORARRAYVPROC glScissorArrayv;
extern PFNGLSCISSORINDEXEDPROC glScissorIndexed;
extern PFNGLSCISSORINDEXEDVPROC glScissorIndexedv;
extern PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv;
extern PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
extern PFNGLGETFLOATI_VPROC glGetFloati_v;
extern PFNGLGETDOUBLEI_VPROC glGetDoublei_v;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.2

// ARB_base_instance
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;

// ARB_transform_feedback_instanced
extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced;

// ARB_internalformat_query
extern PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;

// ARB_shader_atomic_counters
extern PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv;

// ARB_shader_image_load_store
extern PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
extern PFNGLMEMORYBARRIERPROC glMemoryBarrier;

// ARB_texture_storage
extern PFNGLTEXSTORAGE1DPROC glTexStorage1D;
extern PFNGLTEXSTORAGE2DPROC glTexStorage2D;
extern PFNGLTEXSTORAGE3DPROC glTexStorage3D;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.3

// ARB_clear_buffer_object
extern PFNGLCLEARBUFFERDATAPROC glClearBufferData;
extern PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData;

// ARB_compute_shader
extern PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
extern PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect;

// ARB_copy_image
extern PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData;

// ARB_debug_group
extern PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup;
extern PFNGLPOPDEBUGGROUPPROC glPopDebugGroup;

// ARB_debug_label
extern PFNGLOBJECTLABELPROC glObjectLabel;
extern PFNGLGETOBJECTLABELPROC glGetObjectLabel;
extern PFNGLOBJECTPTRLABELPROC glObjectPtrLabel;
extern PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel;

// ARB_debug_output
extern PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl;
extern PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert;
extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
extern PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog;

// ARB_framebuffer_no_attachments
extern PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri;
extern PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv;

// ARB_internalformat_query2
extern PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v;

// ARB_invalidate_subdata
extern PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage;
extern PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage;
extern PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData;
extern PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData;
extern PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer;
extern PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer;

// ARB_multi_draw_indirect
extern PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect;
extern PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect;

// ARB_program_interface_query
extern PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv;
extern PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex;
extern PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName;
extern PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv;
extern PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation;
extern PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;

// ARB_shader_storage_buffer_object
extern PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding;

// ARB_texture_buffer_range
extern PFNGLTEXBUFFERRANGEPROC glTexBufferRange;

// ARB_texture_storage_multisample
extern PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample;
extern PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample;

// ARB_texture_view
extern PFNGLTEXTUREVIEWPROC glTextureView;

// ARB_vertex_attrib_binding
extern PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer;
extern PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat;
extern PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat;
extern PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat;
extern PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding;
extern PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.4

// ARB_buffer_storage
extern PFNGLBUFFERSTORAGEPROC glBufferStorage;

// ARB_clear_texture
extern PFNGLCLEARTEXIMAGEPROC glClearTexImage;
extern PFNGLCLEARTEXSUBIMAGEPROC glClearTexSubImage;

// ARB_multi_bind
extern PFNGLBINDBUFFERSBASEPROC glBindBuffersBase;
extern PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange;
extern PFNGLBINDTEXTURESPROC glBindTextures;
extern PFNGLBINDSAMPLERSPROC glBindSamplers;
extern PFNGLBINDIMAGETEXTURESPROC glBindImageTextures;
extern PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers;

// --------------------------------------------------------------------------------------------------------------------
// WGL

// WGL_ARB_pixel_format
extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

// WGL_EXT_swap_control
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

// WGL_ARB_create_context
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

// --------------------------------------------------------------------------------------------------------------------
// ARB extensions


// --------------------------------------------------------------------------------------------------------------------
// nVidia extensions

// GL_NV_shader_buffer_load
#define GL_BUFFER_GPU_ADDRESS_NV          0x8F1D
#define GL_GPU_ADDRESS_NV                 0x8F34
#define GL_MAX_SHADER_BUFFER_ADDRESS_NV   0x8F35

typedef void (APIENTRYP PFNGLMAKEBUFFERRESIDENTNVPROC) (GLenum target, GLenum access);
typedef void (APIENTRYP PFNGLMAKEBUFFERNONRESIDENTNVPROC) (GLenum target);
typedef GLboolean (APIENTRYP PFNGLISBUFFERRESIDENTNVPROC) (GLenum target);
typedef void (APIENTRYP PFNGLMAKENAMEDBUFFERRESIDENTNVPROC) (GLuint buffer, GLenum access);
typedef void (APIENTRYP PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC) (GLuint buffer);
typedef GLboolean (APIENTRYP PFNGLISNAMEDBUFFERRESIDENTNVPROC) (GLuint buffer);
typedef void (APIENTRYP PFNGLGETBUFFERPARAMETERUI64VNVPROC) (GLenum target, GLenum pname, GLuint64EXT *params);
typedef void (APIENTRYP PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC) (GLuint buffer, GLenum pname, GLuint64EXT *params);
typedef void (APIENTRYP PFNGLGETINTEGERUI64VNVPROC) (GLenum value, GLuint64EXT *result);
typedef void (APIENTRYP PFNGLUNIFORMUI64NVPROC) (GLint location, GLuint64EXT value);
typedef void (APIENTRYP PFNGLUNIFORMUI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT *value);
typedef void (APIENTRYP PFNGLGETUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLuint64EXT *params);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMUI64NVPROC) (GLuint program, GLint location, GLuint64EXT value);
typedef void (APIENTRYP PFNGLPROGRAMUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT *value);

extern PFNGLMAKEBUFFERRESIDENTNVPROC glMakeBufferResidentNV;
extern PFNGLMAKEBUFFERNONRESIDENTNVPROC glMakeBufferNonResidentNV;
extern PFNGLISBUFFERRESIDENTNVPROC glIsBufferResidentNV;
extern PFNGLMAKENAMEDBUFFERRESIDENTNVPROC glMakeNamedBufferResidentNV;
extern PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC glMakeNamedBufferNonResidentNV;
extern PFNGLISNAMEDBUFFERRESIDENTNVPROC glIsNamedBufferResidentNV;
extern PFNGLGETBUFFERPARAMETERUI64VNVPROC glGetBufferParameterui64vNV;
extern PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC glGetNamedBufferParameterui64vNV;
extern PFNGLGETINTEGERUI64VNVPROC glGetIntegerui64vNV;
extern PFNGLUNIFORMUI64NVPROC glUniformui64NV;
extern PFNGLUNIFORMUI64VNVPROC glUniformui64vNV;
extern PFNGLGETUNIFORMUI64VNVPROC glGetUniformui64vNV;
extern PFNGLPROGRAMUNIFORMUI64NVPROC glProgramUniformui64NV;
extern PFNGLPROGRAMUNIFORMUI64VNVPROC glProgramUniformui64vNV;

// GL_NV_vertex_buffer_unified_memory
#define GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV 0x8F1E
#define GL_ELEMENT_ARRAY_UNIFIED_NV       0x8F1F
#define GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV 0x8F20
#define GL_ELEMENT_ARRAY_ADDRESS_NV       0x8F29
#define GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV  0x8F2A
#define GL_ELEMENT_ARRAY_LENGTH_NV        0x8F33
#define GL_DRAW_INDIRECT_UNIFIED_NV       0x8F40
#define GL_DRAW_INDIRECT_ADDRESS_NV       0x8F41
#define GL_DRAW_INDIRECT_LENGTH_NV        0x8F42

typedef void (APIENTRYP PFNGLBUFFERADDRESSRANGENVPROC) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
typedef void (APIENTRYP PFNGLVERTEXFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLVERTEXATTRIBFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
typedef void (APIENTRYP PFNGLVERTEXATTRIBIFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);
typedef void (APIENTRYP PFNGLGETINTEGERUI64I_VNVPROC) (GLenum value, GLuint index, GLuint64EXT *result);

extern PFNGLBUFFERADDRESSRANGENVPROC glBufferAddressRangeNV;
extern PFNGLVERTEXATTRIBFORMATNVPROC glVertexAttribFormatNV;
extern PFNGLVERTEXATTRIBIFORMATNVPROC glVertexAttribIFormatNV;
extern PFNGLGETINTEGERUI64I_VNVPROC glGetIntegerui64i_vNV;

// --------------------------------------------------------------------------------------------------------------------
// Common extensions

// EXT_texture_compression_s3tc
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

// EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

// EXT_texture_mirror_clamp
#define GL_MIRROR_CLAMP_EXT               0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT       0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT     0x8912

// EXT_texture_sRGB
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT  0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F

// --------------------------------------------------------------------------------------------------------------------
// Indirect structures
struct DrawElementsIndirectCommand
{
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

// --------------------------------------------------------------------------------------------------------------------
namespace wgl
{
    void bind_wgl();
    void bind_gl();
}
