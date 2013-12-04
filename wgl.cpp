// Windows interface to OpenGL

#include "gfx.h"

#if defined(USE_GL4)

#include "wgl.h"

#pragma comment (lib, "opengl32.lib")

// --------------------------------------------------------------------------------------------------------------------
// GL 1.2
PFNGLBLENDCOLORPROC glBlendColor;
PFNGLBLENDEQUATIONPROC glBlendEquation;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
PFNGLTEXIMAGE3DPROC glTexImage3D;
PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D;
PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;

// --------------------------------------------------------------------------------------------------------------------
// GL 1.3
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLSAMPLECOVERAGEPROC glSampleCoverage;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glCompressedTexImage3D;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glCompressedTexImage1D;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glGetCompressedTexImage;

// --------------------------------------------------------------------------------------------------------------------
// GL 1.4
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
PFNGLMULTIDRAWARRAYSPROC glMultiDrawArrays;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
PFNGLPOINTPARAMETERFPROC glPointParameterf;
PFNGLPOINTPARAMETERFVPROC glPointParameterfv;
PFNGLPOINTPARAMETERIPROC glPointParameteri;
PFNGLPOINTPARAMETERIVPROC glPointParameteriv;

// --------------------------------------------------------------------------------------------------------------------
// GL 1.5
PFNGLGENQUERIESPROC glGenQueries;
PFNGLDELETEQUERIESPROC glDeleteQueries;
PFNGLISQUERYPROC glIsQuery;
PFNGLBEGINQUERYPROC glBeginQuery;
PFNGLENDQUERYPROC glEndQuery;
PFNGLGETQUERYIVPROC glGetQueryiv;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLISBUFFERPROC glIsBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData;
PFNGLMAPBUFFERPROC glMapBuffer;
PFNGLUNMAPBUFFERPROC glUnmapBuffer;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
PFNGLGETBUFFERPOINTERVPROC glGetBufferPointerv;

// --------------------------------------------------------------------------------------------------------------------
// GL 2.0
PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERSOURCEPROC glGetShaderSource;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETUNIFORMFVPROC glGetUniformfv;
PFNGLGETUNIFORMIVPROC glGetUniformiv;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
PFNGLISPROGRAMPROC glIsProgram;
PFNGLISSHADERPROC glIsShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM2IPROC glUniform2i;
PFNGLUNIFORM3IPROC glUniform3i;
PFNGLUNIFORM4IPROC glUniform4i;
PFNGLUNIFORM1FVPROC glUniform1fv;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORM1IVPROC glUniform1iv;
PFNGLUNIFORM2IVPROC glUniform2iv;
PFNGLUNIFORM3IVPROC glUniform3iv;
PFNGLUNIFORM4IVPROC glUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLVALIDATEPROGRAMPROC glValidateProgram;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

// --------------------------------------------------------------------------------------------------------------------
// GL 2.1
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.0
PFNGLCOLORMASKIPROC glColorMaski;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
PFNGLENABLEIPROC glEnablei;
PFNGLDISABLEIPROC glDisablei;
PFNGLISENABLEDIPROC glIsEnabledi;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
PFNGLCLAMPCOLORPROC glClampColor;
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation;
PFNGLUNIFORM1UIPROC glUniform1ui;
PFNGLUNIFORM2UIPROC glUniform2ui;
PFNGLUNIFORM3UIPROC glUniform3ui;
PFNGLUNIFORM4UIPROC glUniform4ui;
PFNGLUNIFORM1UIVPROC glUniform1uiv;
PFNGLUNIFORM2UIVPROC glUniform2uiv;
PFNGLUNIFORM3UIVPROC glUniform3uiv;
PFNGLUNIFORM4UIVPROC glUniform4uiv;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv;
PFNGLCLEARBUFFERIVPROC glClearBufferiv;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
PFNGLCLEARBUFFERFVPROC glClearBufferfv;
PFNGLCLEARBUFFERFIPROC glClearBufferfi;
PFNGLGETSTRINGIPROC glGetStringi;

// ARB_framebuffer_object
PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;

// ARB_map_buffer_range
PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;

// ARB_vertex_array_object
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLISVERTEXARRAYPROC glIsVertexArray;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.1
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
PFNGLTEXBUFFERPROC glTexBuffer;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex;

// ARB_copy_buffer
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;

// ARB_uniform_buffer_object
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.2
PFNGLGETINTEGER64I_VPROC glGetInteger64i_v;
PFNGLGETBUFFERPARAMETERI64VPROC glGetBufferParameteri64v;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;

// ARB_draw_elements_base_vertex
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glDrawRangeElementsBaseVertex;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex;

// ARB_provoking_vertex
PFNGLPROVOKINGVERTEXPROC glProvokingVertex;

// ARB_sync
PFNGLFENCESYNCPROC glFenceSync;
PFNGLISSYNCPROC glIsSync;
PFNGLDELETESYNCPROC glDeleteSync;
PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
PFNGLWAITSYNCPROC glWaitSync;
PFNGLGETINTEGER64VPROC glGetInteger64v;
PFNGLGETSYNCIVPROC glGetSynciv;

// ARB_texture_multisample
PFNGLTEXIMAGE2DMULTISAMPLEPROC glTexImage2DMultisample;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glTexImage3DMultisample;
PFNGLGETMULTISAMPLEFVPROC glGetMultisamplefv;
PFNGLSAMPLEMASKIPROC glSampleMaski;

// --------------------------------------------------------------------------------------------------------------------
// GL 3.3

PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;

// ARB_blend_func_extended
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
PFNGLGETFRAGDATAINDEXPROC glGetFragDataIndex;

// ARB_sampler_objects
PFNGLGENSAMPLERSPROC glGenSamplers;
PFNGLDELETESAMPLERSPROC glDeleteSamplers;
PFNGLISSAMPLERPROC glIsSampler;
PFNGLBINDSAMPLERPROC glBindSampler;
PFNGLSAMPLERPARAMETERIPROC glSamplerParameteri;
PFNGLSAMPLERPARAMETERIVPROC glSamplerParameteriv;
PFNGLSAMPLERPARAMETERFPROC glSamplerParameterf;
PFNGLSAMPLERPARAMETERFVPROC glSamplerParameterfv;
PFNGLSAMPLERPARAMETERIIVPROC glSamplerParameterIiv;
PFNGLSAMPLERPARAMETERIUIVPROC glSamplerParameterIuiv;
PFNGLGETSAMPLERPARAMETERIVPROC glGetSamplerParameteriv;
PFNGLGETSAMPLERPARAMETERIIVPROC glGetSamplerParameterIiv;
PFNGLGETSAMPLERPARAMETERFVPROC glGetSamplerParameterfv;
PFNGLGETSAMPLERPARAMETERIUIVPROC glGetSamplerParameterIuiv;

// ARB_timer_query
PFNGLQUERYCOUNTERPROC glQueryCounter;
PFNGLGETQUERYOBJECTI64VPROC glGetQueryObjecti64v;
PFNGLGETQUERYOBJECTUI64VPROC glGetQueryObjectui64v;

// ARB_vertex_type_2_10_10_10_rev
PFNGLVERTEXATTRIBP1UIPROC glVertexAttribP1ui;
PFNGLVERTEXATTRIBP1UIVPROC glVertexAttribP1uiv;
PFNGLVERTEXATTRIBP2UIPROC glVertexAttribP2ui;
PFNGLVERTEXATTRIBP2UIVPROC glVertexAttribP2uiv;
PFNGLVERTEXATTRIBP3UIPROC glVertexAttribP3ui;
PFNGLVERTEXATTRIBP3UIVPROC glVertexAttribP3uiv;
PFNGLVERTEXATTRIBP4UIPROC glVertexAttribP4ui;
PFNGLVERTEXATTRIBP4UIVPROC glVertexAttribP4uiv;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.0
PFNGLMINSAMPLESHADINGPROC glMinSampleShading;
PFNGLBLENDEQUATIONIPROC glBlendEquationi;
PFNGLBLENDEQUATIONSEPARATEIPROC glBlendEquationSeparatei;
PFNGLBLENDFUNCIPROC glBlendFunci;
PFNGLBLENDFUNCSEPARATEIPROC glBlendFuncSeparatei;

// ARB_draw_indirect
PFNGLDRAWARRAYSINDIRECTPROC glDrawArraysIndirect;
PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;

// ARB_gpu_shader_fp64
PFNGLUNIFORM1DPROC glUniform1d;
PFNGLUNIFORM2DPROC glUniform2d;
PFNGLUNIFORM3DPROC glUniform3d;
PFNGLUNIFORM4DPROC glUniform4d;
PFNGLUNIFORM1DVPROC glUniform1dv;
PFNGLUNIFORM2DVPROC glUniform2dv;
PFNGLUNIFORM3DVPROC glUniform3dv;
PFNGLUNIFORM4DVPROC glUniform4dv;
PFNGLUNIFORMMATRIX2DVPROC glUniformMatrix2dv;
PFNGLUNIFORMMATRIX3DVPROC glUniformMatrix3dv;
PFNGLUNIFORMMATRIX4DVPROC glUniformMatrix4dv;
PFNGLUNIFORMMATRIX2X3DVPROC glUniformMatrix2x3dv;
PFNGLUNIFORMMATRIX2X4DVPROC glUniformMatrix2x4dv;
PFNGLUNIFORMMATRIX3X2DVPROC glUniformMatrix3x2dv;
PFNGLUNIFORMMATRIX3X4DVPROC glUniformMatrix3x4dv;
PFNGLUNIFORMMATRIX4X2DVPROC glUniformMatrix4x2dv;
PFNGLUNIFORMMATRIX4X3DVPROC glUniformMatrix4x3dv;
PFNGLGETUNIFORMDVPROC glGetUniformdv;

// ARB_shader_subroutine
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC glGetSubroutineUniformLocation;
PFNGLGETSUBROUTINEINDEXPROC glGetSubroutineIndex;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC glGetActiveSubroutineUniformiv;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
PFNGLGETACTIVESUBROUTINENAMEPROC glGetActiveSubroutineName;
PFNGLUNIFORMSUBROUTINESUIVPROC glUniformSubroutinesuiv;
PFNGLGETUNIFORMSUBROUTINEUIVPROC glGetUniformSubroutineuiv;
PFNGLGETPROGRAMSTAGEIVPROC glGetProgramStageiv;

// ARB_tessellation_shader
PFNGLPATCHPARAMETERIPROC glPatchParameteri;
PFNGLPATCHPARAMETERFVPROC glPatchParameterfv;

// ARB_transform_feedback2
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks;
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback;
PFNGLPAUSETRANSFORMFEEDBACKPROC glPauseTransformFeedback;
PFNGLRESUMETRANSFORMFEEDBACKPROC glResumeTransformFeedback;
PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;

// ARB_transform_feedback3
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC glDrawTransformFeedbackStream;
PFNGLBEGINQUERYINDEXEDPROC glBeginQueryIndexed;
PFNGLENDQUERYINDEXEDPROC glEndQueryIndexed;
PFNGLGETQUERYINDEXEDIVPROC glGetQueryIndexediv;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.1

// ARB_ES2_compatibility
PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
PFNGLSHADERBINARYPROC glShaderBinary;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
PFNGLDEPTHRANGEFPROC glDepthRangef;
PFNGLCLEARDEPTHFPROC glClearDepthf;

// ARB_get_program_binary
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
PFNGLPROGRAMBINARYPROC glProgramBinary;

// ARB_separate_shader_objects
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;

// ARB_vertex_attrib_64bit
PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d;
PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d;
PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d;
PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d;
PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv;
PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv;
PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv;
PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;
PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv;

// ARB_viewport_array
PFNGLVIEWPORTARRAYVPROC glViewportArrayv;
PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf;
PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
PFNGLSCISSORARRAYVPROC glScissorArrayv;
PFNGLSCISSORINDEXEDPROC glScissorIndexed;
PFNGLSCISSORINDEXEDVPROC glScissorIndexedv;
PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv;
PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
PFNGLGETFLOATI_VPROC glGetFloati_v;
PFNGLGETDOUBLEI_VPROC glGetDoublei_v;

// --------------------------------------------------------------------------------------------------------------------
// GL 4.2

// ARB_base_instance
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;

// ARB_transform_feedback_instanced
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced;

// ARB_internalformat_query
PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;

// ARB_shader_atomic_counters
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv;

// ARB_shader_image_load_store
PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
PFNGLMEMORYBARRIERPROC glMemoryBarrier;

// ARB_texture_storage
PFNGLTEXSTORAGE1DPROC glTexStorage1D;
PFNGLTEXSTORAGE2DPROC glTexStorage2D;
PFNGLTEXSTORAGE3DPROC glTexStorage3D;
PFNGLTEXTURESTORAGE1DEXTPROC glTextureStorage1DEXT;
PFNGLTEXTURESTORAGE2DEXTPROC glTextureStorage2DEXT;
PFNGLTEXTURESTORAGE3DEXTPROC glTextureStorage3DEXT;

// --------------------------------------------------------------------------------------------------------------------
// WGL

// WGL_ARB_pixel_format
PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

// WGL_EXT_swap_control
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

// WGL_ARB_create_context
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

// --------------------------------------------------------------------------------------------------------------------
template<class T>
void bind(T& x, const char* name)
{
    x = reinterpret_cast<T>(wglGetProcAddress(name));
}

void wgl::bind_wgl()
{
    // WGL_ARB_pixel_format
    bind(wglGetPixelFormatAttribivARB, "wglGetPixelFormatAttribivARB");
    bind(wglGetPixelFormatAttribfvARB, "wglGetPixelFormatAttribfvARB");
    bind(wglChoosePixelFormatARB, "wglChoosePixelFormatARB");

    // WGL_EXT_swap_control
    bind(wglSwapIntervalEXT, "wglSwapIntervalEXT");
    bind(wglGetSwapIntervalEXT, "wglGetSwapIntervalEXT");

    // WGL_ARB_create_context
    bind(wglCreateContextAttribsARB, "wglCreateContextAttribsARB");
}

void wgl::bind_gl()
{
    // ------------------------------------------------------------------------------------------------
    // GL 1.2
    bind(glBlendColor, "glBlendColor");
    bind(glBlendEquation, "glBlendEquation");
    bind(glDrawRangeElements, "glDrawRangeElements");
    bind(glTexImage3D, "glTexImage3D");
    bind(glTexSubImage3D, "glTexSubImage3D");
    bind(glCopyTexSubImage3D, "glCopyTexSubImage3D");

    // ------------------------------------------------------------------------------------------------
    // GL 1.3
    bind(glActiveTexture, "glActiveTexture");
    bind(glSampleCoverage, "glSampleCoverage");
    bind(glCompressedTexImage3D, "glCompressedTexImage3D");
    bind(glCompressedTexImage2D , "glCompressedTexImage2D");
    bind(glCompressedTexImage1D, "glCompressedTexImage1D");
    bind(glCompressedTexSubImage3D, "glCompressedTexSubImage3D");
    bind(glCompressedTexSubImage2D, "glCompressedTexSubImage2D");
    bind(glCompressedTexSubImage1D, "glCompressedTexSubImage1D");
    bind(glGetCompressedTexImage, "glGetCompressedTexImage");

    // ------------------------------------------------------------------------------------------------
    // GL 1.4
    bind(glBlendFuncSeparate, "glBlendFuncSeparate");
    bind(glMultiDrawArrays, "glMultiDrawArrays");
    bind(glMultiDrawElements, "glMultiDrawElements");
    bind(glPointParameterf, "glPointParameterf");
    bind(glPointParameterfv, "glPointParameterfv");
    bind(glPointParameteri, "glPointParameteri");
    bind(glPointParameteriv, "glPointParameteriv");

    // ------------------------------------------------------------------------------------------------
    // GL 1.5
    bind(glGenQueries, "glGenQueries");
    bind(glDeleteQueries, "glDeleteQueries");
    bind(glIsQuery, "glIsQuery");
    bind(glBeginQuery, "glBeginQuery");
    bind(glEndQuery, "glEndQuery");
    bind(glGetQueryiv, "glGetQueryiv");
    bind(glGetQueryObjectiv, "glGetQueryObjectiv");
    bind(glGetQueryObjectuiv, "glGetQueryObjectuiv");
    bind(glBindBuffer, "glBindBuffer");
    bind(glDeleteBuffers, "glDeleteBuffers");
    bind(glGenBuffers, "glGenBuffers");
    bind(glIsBuffer, "glIsBuffer");
    bind(glBufferData, "glBufferData");
    bind(glBufferSubData, "glBufferSubData");
    bind(glGetBufferSubData, "glGetBufferSubData");
    bind(glMapBuffer, "glMapBuffer");
    bind(glUnmapBuffer, "glUnmapBuffer");
    bind(glGetBufferParameteriv, "glGetBufferParameteriv");
    bind(glGetBufferPointerv, "glGetBufferPointerv");

    // ------------------------------------------------------------------------------------------------
    // GL 2.0
    bind(glBlendEquationSeparate, "glBlendEquationSeparate");
    bind(glDrawBuffers, "glDrawBuffers");
    bind(glStencilOpSeparate, "glStencilOpSeparate");
    bind(glStencilFuncSeparate, "glStencilFuncSeparate");
    bind(glStencilMaskSeparate, "glStencilMaskSeparate");
    bind(glAttachShader, "glAttachShader");
    bind(glBindAttribLocation, "glBindAttribLocation");
    bind(glCompileShader, "glCompileShader");
    bind(glCreateProgram, "glCreateProgram");
    bind(glCreateShader, "glCreateShader");
    bind(glDeleteProgram, "glDeleteProgram");
    bind(glDeleteShader, "glDeleteShader");
    bind(glDetachShader, "glDetachShader");
    bind(glDisableVertexAttribArray, "glDisableVertexAttribArray");
    bind(glEnableVertexAttribArray, "glEnableVertexAttribArray");
    bind(glGetActiveAttrib, "glGetActiveAttrib");
    bind(glGetActiveUniform, "glGetActiveUniform");
    bind(glGetAttachedShaders, "glGetAttachedShaders");
    bind(glGetAttribLocation, "glGetAttribLocation");
    bind(glGetProgramiv, "glGetProgramiv");
    bind(glGetProgramInfoLog, "glGetProgramInfoLog");
    bind(glGetShaderiv, "glGetShaderiv");
    bind(glGetShaderInfoLog, "glGetShaderInfoLog");
    bind(glGetShaderSource, "glGetShaderSource");
    bind(glGetUniformLocation, "glGetUniformLocation");
    bind(glGetUniformfv, "glGetUniformfv");
    bind(glGetUniformiv, "glGetUniformiv");
    bind(glGetVertexAttribdv, "glGetVertexAttribdv");
    bind(glGetVertexAttribfv, "glGetVertexAttribfv");
    bind(glGetVertexAttribiv, "glGetVertexAttribiv");
    bind(glGetVertexAttribPointerv, "glGetVertexAttribPointerv");
    bind(glIsProgram, "glIsProgram");
    bind(glIsShader, "glIsShader");
    bind(glLinkProgram, "glLinkProgram");
    bind(glShaderSource, "glShaderSource");
    bind(glUseProgram, "glUseProgram");
    bind(glUniform1f, "glUniform1f");
    bind(glUniform2f, "glUniform2f");
    bind(glUniform3f, "glUniform3f");
    bind(glUniform4f, "glUniform4f");
    bind(glUniform1i, "glUniform1i");
    bind(glUniform2i, "glUniform2i");
    bind(glUniform3i, "glUniform3i");
    bind(glUniform4i, "glUniform4i");
    bind(glUniform1fv, "glUniform1fv");
    bind(glUniform2fv, "glUniform2fv");
    bind(glUniform3fv, "glUniform3fv");
    bind(glUniform4fv, "glUniform4fv");
    bind(glUniform1iv, "glUniform1iv");
    bind(glUniform2iv, "glUniform2iv");
    bind(glUniform3iv, "glUniform3iv");
    bind(glUniform4iv, "glUniform4iv");
    bind(glUniformMatrix2fv, "glUniformMatrix2fv");
    bind(glUniformMatrix3fv, "glUniformMatrix3fv");
    bind(glUniformMatrix4fv, "glUniformMatrix4fv");
    bind(glValidateProgram, "glValidateProgram");
    bind(glVertexAttrib1d, "glVertexAttrib1d");
    bind(glVertexAttrib1dv, "glVertexAttrib1dv");
    bind(glVertexAttrib1f, "glVertexAttrib1f");
    bind(glVertexAttrib1fv, "glVertexAttrib1fv");
    bind(glVertexAttrib1s, "glVertexAttrib1s");
    bind(glVertexAttrib1sv, "glVertexAttrib1sv");
    bind(glVertexAttrib2d, "glVertexAttrib2d");
    bind(glVertexAttrib2dv, "glVertexAttrib2dv");
    bind(glVertexAttrib2f, "glVertexAttrib2f");
    bind(glVertexAttrib2fv, "glVertexAttrib2fv");
    bind(glVertexAttrib2s, "glVertexAttrib2s");
    bind(glVertexAttrib2sv, "glVertexAttrib2sv");
    bind(glVertexAttrib3d, "glVertexAttrib3d");
    bind(glVertexAttrib3dv, "glVertexAttrib3dv");
    bind(glVertexAttrib3f, "glVertexAttrib3f");
    bind(glVertexAttrib3fv, "glVertexAttrib3fv");
    bind(glVertexAttrib3s, "glVertexAttrib3s");
    bind(glVertexAttrib3sv, "glVertexAttrib3sv");
    bind(glVertexAttrib4Nbv, "glVertexAttrib4Nbv");
    bind(glVertexAttrib4Niv, "glVertexAttrib4Niv");
    bind(glVertexAttrib4Nsv, "glVertexAttrib4Nsv");
    bind(glVertexAttrib4Nub, "glVertexAttrib4Nub");
    bind(glVertexAttrib4Nubv, "glVertexAttrib4Nubv");
    bind(glVertexAttrib4Nuiv, "glVertexAttrib4Nuiv");
    bind(glVertexAttrib4Nusv, "glVertexAttrib4Nusv");
    bind(glVertexAttrib4bv, "glVertexAttrib4bv");
    bind(glVertexAttrib4d, "glVertexAttrib4d");
    bind(glVertexAttrib4dv, "glVertexAttrib4dv");
    bind(glVertexAttrib4f, "glVertexAttrib4f");
    bind(glVertexAttrib4fv, "glVertexAttrib4fv");
    bind(glVertexAttrib4iv, "glVertexAttrib4iv");
    bind(glVertexAttrib4s, "glVertexAttrib4s");
    bind(glVertexAttrib4sv, "glVertexAttrib4sv");
    bind(glVertexAttrib4ubv, "glVertexAttrib4ubv");
    bind(glVertexAttrib4uiv, "glVertexAttrib4uiv");
    bind(glVertexAttrib4usv, "glVertexAttrib4usv");
    bind(glVertexAttribPointer, "glVertexAttribPointer");

    // ------------------------------------------------------------------------------------------------
    // GL 2.1
    bind(glUniformMatrix2x3fv, "glUniformMatrix2x3fv");
    bind(glUniformMatrix3x2fv, "glUniformMatrix3x2fv");
    bind(glUniformMatrix2x4fv, "glUniformMatrix2x4fv");
    bind(glUniformMatrix4x2fv, "glUniformMatrix4x2fv");
    bind(glUniformMatrix3x4fv, "glUniformMatrix3x4fv");
    bind(glUniformMatrix4x3fv, "glUniformMatrix4x3fv");

    // ------------------------------------------------------------------------------------------------
    // GL 3.0
    bind(glColorMaski, "glColorMaski");
    bind(glGetBooleani_v, "glGetBooleani_v");
    bind(glGetIntegeri_v, "glGetIntegeri_v");
    bind(glEnablei, "glEnablei");
    bind(glDisablei, "glDisablei");
    bind(glIsEnabledi, "glIsEnabledi");
    bind(glBeginTransformFeedback, "glBeginTransformFeedback");
    bind(glEndTransformFeedback, "glEndTransformFeedback");
    bind(glBindBufferRange, "glBindBufferRange");
    bind(glBindBufferBase, "glBindBufferBase");
    bind(glTransformFeedbackVaryings, "glTransformFeedbackVaryings");
    bind(glGetTransformFeedbackVarying, "glGetTransformFeedbackVarying");
    bind(glClampColor, "glClampColor");
    bind(glBeginConditionalRender, "glBeginConditionalRender");
    bind(glEndConditionalRender, "glEndConditionalRender");
    bind(glVertexAttribIPointer, "glVertexAttribIPointer");
    bind(glGetVertexAttribIiv, "glGetVertexAttribIiv");
    bind(glGetVertexAttribIuiv, "glGetVertexAttribIuiv");
    bind(glVertexAttribI1i, "glVertexAttribI1i");
    bind(glVertexAttribI2i, "glVertexAttribI2i");
    bind(glVertexAttribI3i, "glVertexAttribI3i");
    bind(glVertexAttribI4i, "glVertexAttribI4i");
    bind(glVertexAttribI1ui, "glVertexAttribI1ui");
    bind(glVertexAttribI2ui, "glVertexAttribI2ui");
    bind(glVertexAttribI3ui, "glVertexAttribI3ui");
    bind(glVertexAttribI4ui, "glVertexAttribI4ui");
    bind(glVertexAttribI1iv, "glVertexAttribI1iv");
    bind(glVertexAttribI2iv, "glVertexAttribI2iv");
    bind(glVertexAttribI3iv, "glVertexAttribI3iv");
    bind(glVertexAttribI4iv, "glVertexAttribI4iv");
    bind(glVertexAttribI1uiv, "glVertexAttribI1uiv");
    bind(glVertexAttribI2uiv, "glVertexAttribI2uiv");
    bind(glVertexAttribI3uiv, "glVertexAttribI3uiv");
    bind(glVertexAttribI4uiv, "glVertexAttribI4uiv");
    bind(glVertexAttribI4bv, "glVertexAttribI4bv");
    bind(glVertexAttribI4sv, "glVertexAttribI4sv");
    bind(glVertexAttribI4ubv, "glVertexAttribI4ubv");
    bind(glVertexAttribI4usv, "glVertexAttribI4usv");
    bind(glGetUniformuiv, "glGetUniformuiv");
    bind(glBindFragDataLocation, "glBindFragDataLocation");
    bind(glGetFragDataLocation, "glGetFragDataLocation");
    bind(glUniform1ui, "glUniform1ui");
    bind(glUniform2ui, "glUniform2ui");
    bind(glUniform3ui, "glUniform3ui");
    bind(glUniform4ui, "glUniform4ui");
    bind(glUniform1uiv, "glUniform1uiv");
    bind(glUniform2uiv, "glUniform2uiv");
    bind(glUniform3uiv, "glUniform3uiv");
    bind(glUniform4uiv, "glUniform4uiv");
    bind(glTexParameterIiv, "glTexParameterIiv");
    bind(glTexParameterIuiv, "glTexParameterIuiv");
    bind(glGetTexParameterIiv, "glGetTexParameterIiv");
    bind(glGetTexParameterIuiv, "glGetTexParameterIuiv");
    bind(glClearBufferiv, "glClearBufferiv");
    bind(glClearBufferuiv, "glClearBufferuiv");
    bind(glClearBufferfv, "glClearBufferfv");
    bind(glClearBufferfi, "glClearBufferfi");
    bind(glGetStringi, "glGetStringi");

    // ARB_framebuffer_object
    bind(glIsRenderbuffer, "glIsRenderbuffer");
    bind(glBindRenderbuffer, "glBindRenderbuffer");
    bind(glDeleteRenderbuffers, "glDeleteRenderbuffers");
    bind(glGenRenderbuffers, "glGenRenderbuffers");
    bind(glRenderbufferStorage, "glRenderbufferStorage");
    bind(glGetRenderbufferParameteriv, "glGetRenderbufferParameteriv");
    bind(glIsFramebuffer, "glIsFramebuffer");
    bind(glBindFramebuffer, "glBindFramebuffer");
    bind(glDeleteFramebuffers, "glDeleteFramebuffers");
    bind(glGenFramebuffers, "glGenFramebuffers");
    bind(glCheckFramebufferStatus, "glCheckFramebufferStatus");
    bind(glFramebufferTexture1D, "glFramebufferTexture1D");
    bind(glFramebufferTexture2D, "glFramebufferTexture2D");
    bind(glFramebufferTexture3D, "glFramebufferTexture3D");
    bind(glFramebufferRenderbuffer, "glFramebufferRenderbuffer");
    bind(glGetFramebufferAttachmentParameteriv, "glGetFramebufferAttachmentParameteriv");
    bind(glGenerateMipmap, "glGenerateMipmap");
    bind(glBlitFramebuffer, "glBlitFramebuffer");
    bind(glRenderbufferStorageMultisample, "glRenderbufferStorageMultisample");
    bind(glFramebufferTextureLayer, "glFramebufferTextureLayer");

    // ARB_map_buffer_range
    bind(glMapBufferRange, "glMapBufferRange");
    bind(glFlushMappedBufferRange, "glFlushMappedBufferRange");

    // ARB_vertex_array_object
    bind(glBindVertexArray, "glBindVertexArray");
    bind(glDeleteVertexArrays, "glDeleteVertexArrays");
    bind(glGenVertexArrays, "glGenVertexArrays");
    bind(glIsVertexArray, "glIsVertexArray");

    // ------------------------------------------------------------------------------------------------
    // GL 3.1
    bind(glDrawArraysInstanced, "glDrawArraysInstanced");
    bind(glDrawElementsInstanced, "glDrawElementsInstanced");
    bind(glTexBuffer, "glTexBuffer");
    bind(glPrimitiveRestartIndex, "glPrimitiveRestartIndex");

    // ARB_copy_buffer
    bind(glCopyBufferSubData, "glCopyBufferSubData");

    // ARB_uniform_buffer_object
    bind(glGetUniformIndices, "glGetUniformIndices");
    bind(glGetActiveUniformsiv, "glGetActiveUniformsiv");
    bind(glGetActiveUniformName, "glGetActiveUniformName");
    bind(glGetUniformBlockIndex, "glGetUniformBlockIndex");
    bind(glGetActiveUniformBlockiv, "glGetActiveUniformBlockiv");
    bind(glGetActiveUniformBlockName, "glGetActiveUniformBlockName");
    bind(glUniformBlockBinding, "glUniformBlockBinding");

    // ------------------------------------------------------------------------------------------------
    // GL 3.2
    bind(glGetInteger64i_v, "glGetInteger64i_v");
    bind(glGetBufferParameteri64v, "glGetBufferParameteri64v");
    bind(glFramebufferTexture, "glFramebufferTexture");

    // ARB_draw_elements_base_vertex
    bind(glDrawElementsBaseVertex, "glDrawElementsBaseVertex");
    bind(glDrawRangeElementsBaseVertex, "glDrawRangeElementsBaseVertex");
    bind(glDrawElementsInstancedBaseVertex, "glDrawElementsInstancedBaseVertex");
    bind(glMultiDrawElementsBaseVertex, "glMultiDrawElementsBaseVertex");

    // ARB_provoking_vertex
    bind(glProvokingVertex, "glProvokingVertex");

    // ARB_sync
    bind(glFenceSync, "glFenceSync");
    bind(glIsSync, "glIsSync");
    bind(glDeleteSync, "glDeleteSync");
    bind(glClientWaitSync, "glClientWaitSync");
    bind(glWaitSync, "glWaitSync");
    bind(glGetInteger64v, "glGetInteger64v");
    bind(glGetSynciv, "glGetSynciv");

    // ARB_texture_multisample
    bind(glTexImage2DMultisample, "glTexImage2DMultisample");
    bind(glTexImage3DMultisample, "glTexImage3DMultisample");
    bind(glGetMultisamplefv, "glGetMultisamplefv");
    bind(glSampleMaski, "glSampleMaski");

    // ------------------------------------------------------------------------------------------------
    // GL 3.3
    bind(glVertexAttribDivisor, "glVertexAttribDivisor");

    // ARB_blend_func_extended
    bind(glBindFragDataLocationIndexed, "glBindFragDataLocationIndexed");
    bind(glGetFragDataIndex, "glGetFragDataIndex");

    // ARB_sampler_objects
    bind(glGenSamplers, "glGenSamplers");
    bind(glDeleteSamplers, "glDeleteSamplers");
    bind(glIsSampler, "glIsSampler");
    bind(glBindSampler, "glBindSampler");
    bind(glSamplerParameteri, "glSamplerParameteri");
    bind(glSamplerParameteriv, "glSamplerParameteriv");
    bind(glSamplerParameterf, "glSamplerParameterf");
    bind(glSamplerParameterfv, "glSamplerParameterfv");
    bind(glSamplerParameterIiv, "glSamplerParameterIiv");
    bind(glSamplerParameterIuiv, "glSamplerParameterIuiv");
    bind(glGetSamplerParameteriv, "glGetSamplerParameteriv");
    bind(glGetSamplerParameterIiv, "glGetSamplerParameterIiv");
    bind(glGetSamplerParameterfv, "glGetSamplerParameterfv");
    bind(glGetSamplerParameterIuiv, "glGetSamplerParameterIuiv");

    // ARB_timer_query
    bind(glQueryCounter, "glQueryCounter");
    bind(glGetQueryObjecti64v, "glGetQueryObjecti64v");
    bind(glGetQueryObjectui64v, "glGetQueryObjectui64v");

    // ARB_vertex_type_2_10_10_10_rev
    bind(glVertexAttribP1ui, "glVertexAttribP1ui");
    bind(glVertexAttribP1uiv, "glVertexAttribP1uiv");
    bind(glVertexAttribP2ui, "glVertexAttribP2ui");
    bind(glVertexAttribP2uiv, "glVertexAttribP2uiv");
    bind(glVertexAttribP3ui, "glVertexAttribP3ui");
    bind(glVertexAttribP3uiv, "glVertexAttribP3uiv");
    bind(glVertexAttribP4ui, "glVertexAttribP4ui");
    bind(glVertexAttribP4uiv, "glVertexAttribP4uiv");

    // ------------------------------------------------------------------------------------------------
    // GL 4.0
    bind(glMinSampleShading, "glMinSampleShading");
    bind(glBlendEquationi, "glBlendEquationi");
    bind(glBlendEquationSeparatei, "glBlendEquationSeparatei");
    bind(glBlendFunci, "glBlendFunci");
    bind(glBlendFuncSeparatei, "glBlendFuncSeparatei");

    // ARB_draw_indirect
    bind(glDrawArraysIndirect, "glDrawArraysIndirect");
    bind(glDrawElementsIndirect, "glDrawElementsIndirect");

    // ARB_gpu_shader_fp64
    bind(glUniform1d, "glUniform1d");
    bind(glUniform2d, "glUniform2d");
    bind(glUniform3d, "glUniform3d");
    bind(glUniform4d, "glUniform4d");
    bind(glUniform1dv, "glUniform1dv");
    bind(glUniform2dv, "glUniform2dv");
    bind(glUniform3dv, "glUniform3dv");
    bind(glUniform4dv, "glUniform4dv");
    bind(glUniformMatrix2dv, "glUniformMatrix2dv");
    bind(glUniformMatrix3dv, "glUniformMatrix3dv");
    bind(glUniformMatrix4dv, "glUniformMatrix4dv");
    bind(glUniformMatrix2x3dv, "glUniformMatrix2x3dv");
    bind(glUniformMatrix2x4dv, "glUniformMatrix2x4dv");
    bind(glUniformMatrix3x2dv, "glUniformMatrix3x2dv");
    bind(glUniformMatrix3x4dv, "glUniformMatrix3x4dv");
    bind(glUniformMatrix4x2dv, "glUniformMatrix4x2dv");
    bind(glUniformMatrix4x3dv, "glUniformMatrix4x3dv");
    bind(glGetUniformdv, "glGetUniformdv");

    // ARB_shader_subroutine
    bind(glGetSubroutineUniformLocation, "glGetSubroutineUniformLocation");
    bind(glGetSubroutineIndex, "glGetSubroutineIndex");
    bind(glGetActiveSubroutineUniformiv, "glGetActiveSubroutineUniformiv");
    bind(glGetActiveSubroutineUniformName, "glGetActiveSubroutineUniformName");
    bind(glGetActiveSubroutineName, "glGetActiveSubroutineName");
    bind(glUniformSubroutinesuiv, "glUniformSubroutinesuiv");
    bind(glGetUniformSubroutineuiv, "glGetUniformSubroutineuiv");
    bind(glGetProgramStageiv, "glGetProgramStageiv");

    // ARB_tessellation_shader
    bind(glPatchParameteri, "glPatchParameteri");
    bind(glPatchParameterfv, "glPatchParameterfv");

    // ARB_transform_feedback2
    bind(glBindTransformFeedback, "glBindTransformFeedback");
    bind(glDeleteTransformFeedbacks, "glDeleteTransformFeedbacks");
    bind(glGenTransformFeedbacks, "glGenTransformFeedbacks");
    bind(glIsTransformFeedback, "glIsTransformFeedback");
    bind(glPauseTransformFeedback, "glPauseTransformFeedback");
    bind(glResumeTransformFeedback, "glResumeTransformFeedback");
    bind(glDrawTransformFeedback, "glDrawTransformFeedback");

    // ARB_transform_feedback3
    bind(glDrawTransformFeedbackStream, "glDrawTransformFeedbackStream");
    bind(glBeginQueryIndexed, "glBeginQueryIndexed");
    bind(glEndQueryIndexed, "glEndQueryIndexed");
    bind(glGetQueryIndexediv, "glGetQueryIndexediv");

    // ------------------------------------------------------------------------------------------------
    // GL 4.1

    // ARB_ES2_compatibility
    bind(glReleaseShaderCompiler, "glReleaseShaderCompiler");
    bind(glShaderBinary, "glShaderBinary");
    bind(glGetShaderPrecisionFormat, "glGetShaderPrecisionFormat");
    bind(glDepthRangef, "glDepthRangef");
    bind(glClearDepthf, "glClearDepthf");

    // ARB_get_program_binary
    bind(glGetProgramBinary, "glGetProgramBinary");
    bind(glProgramBinary, "glProgramBinary");

    // ARB_separate_shader_objects
    bind(glUseProgramStages, "glUseProgramStages");
    bind(glActiveShaderProgram, "glActiveShaderProgram");
    bind(glCreateShaderProgramv, "glCreateShaderProgramv");
    bind(glBindProgramPipeline, "glBindProgramPipeline");
    bind(glDeleteProgramPipelines, "glDeleteProgramPipelines");
    bind(glGenProgramPipelines, "glGenProgramPipelines");
    bind(glIsProgramPipeline, "glIsProgramPipeline");
    bind(glGetProgramPipelineiv, "glGetProgramPipelineiv");
    bind(glProgramUniform1i, "glProgramUniform1i");
    bind(glProgramUniform1iv, "glProgramUniform1iv");
    bind(glProgramUniform1f, "glProgramUniform1f");
    bind(glProgramUniform1fv, "glProgramUniform1fv");
    bind(glProgramUniform1d, "glProgramUniform1d");
    bind(glProgramUniform1dv, "glProgramUniform1dv");
    bind(glProgramUniform1ui, "glProgramUniform1ui");
    bind(glProgramUniform1uiv, "glProgramUniform1uiv");
    bind(glProgramUniform2i, "glProgramUniform2i");
    bind(glProgramUniform2iv, "glProgramUniform2iv");
    bind(glProgramUniform2f, "glProgramUniform2f");
    bind(glProgramUniform2fv, "glProgramUniform2fv");
    bind(glProgramUniform2d, "glProgramUniform2d");
    bind(glProgramUniform2dv, "glProgramUniform2dv");
    bind(glProgramUniform2ui, "glProgramUniform2ui");
    bind(glProgramUniform2uiv, "glProgramUniform2uiv");
    bind(glProgramUniform3i, "glProgramUniform3i");
    bind(glProgramUniform3iv, "glProgramUniform3iv");
    bind(glProgramUniform3f, "glProgramUniform3f");
    bind(glProgramUniform3fv, "glProgramUniform3fv");
    bind(glProgramUniform3d, "glProgramUniform3d");
    bind(glProgramUniform3dv, "glProgramUniform3dv");
    bind(glProgramUniform3ui, "glProgramUniform3ui");
    bind(glProgramUniform3uiv, "glProgramUniform3uiv");
    bind(glProgramUniform4i, "glProgramUniform4i");
    bind(glProgramUniform4iv, "glProgramUniform4iv");
    bind(glProgramUniform4f, "glProgramUniform4f");
    bind(glProgramUniform4fv, "glProgramUniform4fv");
    bind(glProgramUniform4d, "glProgramUniform4d");
    bind(glProgramUniform4dv, "glProgramUniform4dv");
    bind(glProgramUniform4ui, "glProgramUniform4ui");
    bind(glProgramUniform4uiv, "glProgramUniform4uiv");
    bind(glProgramUniformMatrix2fv, "glProgramUniformMatrix2fv");
    bind(glProgramUniformMatrix3fv, "glProgramUniformMatrix3fv");
    bind(glProgramUniformMatrix4fv, "glProgramUniformMatrix4fv");
    bind(glProgramUniformMatrix2dv, "glProgramUniformMatrix2dv");
    bind(glProgramUniformMatrix3dv, "glProgramUniformMatrix3dv");
    bind(glProgramUniformMatrix4dv, "glProgramUniformMatrix4dv");
    bind(glProgramUniformMatrix2x3fv, "glProgramUniformMatrix2x3fv");
    bind(glProgramUniformMatrix3x2fv, "glProgramUniformMatrix3x2fv");
    bind(glProgramUniformMatrix2x4fv, "glProgramUniformMatrix2x4fv");
    bind(glProgramUniformMatrix4x2fv, "glProgramUniformMatrix4x2fv");
    bind(glProgramUniformMatrix3x4fv, "glProgramUniformMatrix3x4fv");
    bind(glProgramUniformMatrix4x3fv, "glProgramUniformMatrix4x3fv");
    bind(glProgramUniformMatrix2x3dv, "glProgramUniformMatrix2x3dv");
    bind(glProgramUniformMatrix3x2dv, "glProgramUniformMatrix3x2dv");
    bind(glProgramUniformMatrix2x4dv, "glProgramUniformMatrix2x4dv");
    bind(glProgramUniformMatrix4x2dv, "glProgramUniformMatrix4x2dv");
    bind(glProgramUniformMatrix3x4dv, "glProgramUniformMatrix3x4dv");
    bind(glProgramUniformMatrix4x3dv, "glProgramUniformMatrix4x3dv");
    bind(glValidateProgramPipeline, "glValidateProgramPipeline");
    bind(glGetProgramPipelineInfoLog, "glGetProgramPipelineInfoLog");

    // ARB_vertex_attrib_64bit
    bind(glVertexAttribL1d, "glVertexAttribL1d");
    bind(glVertexAttribL2d, "glVertexAttribL2d");
    bind(glVertexAttribL3d, "glVertexAttribL3d");
    bind(glVertexAttribL4d, "glVertexAttribL4d");
    bind(glVertexAttribL1dv, "glVertexAttribL1dv");
    bind(glVertexAttribL2dv, "glVertexAttribL2dv");
    bind(glVertexAttribL3dv, "glVertexAttribL3dv");
    bind(glVertexAttribL4dv, "glVertexAttribL4dv");
    bind(glVertexAttribLPointer, "glVertexAttribLPointer");
    bind(glGetVertexAttribLdv, "glGetVertexAttribLdv");

    // ARB_viewport_array
    bind(glViewportArrayv, "glViewportArrayv");
    bind(glViewportIndexedf, "glViewportIndexedf");
    bind(glViewportIndexedfv, "glViewportIndexedfv");
    bind(glScissorArrayv, "glScissorArrayv");
    bind(glScissorIndexed, "glScissorIndexed");
    bind(glScissorIndexedv, "glScissorIndexedv");
    bind(glDepthRangeArrayv, "glDepthRangeArrayv");
    bind(glDepthRangeIndexed, "glDepthRangeIndexed");
    bind(glGetFloati_v, "glGetFloati_v");
    bind(glGetDoublei_v, "glGetDoublei_v");

    // ------------------------------------------------------------------------------------------------
    // GL 4.2

    // ARB_base_instance
    bind(glDrawArraysInstancedBaseInstance, "glDrawArraysInstancedBaseInstance");
    bind(glDrawElementsInstancedBaseInstance, "glDrawElementsInstancedBaseInstance");
    bind(glDrawElementsInstancedBaseVertexBaseInstance, "glDrawElementsInstancedBaseVertexBaseInstance");

    // ARB_transform_feedback_instanced
    bind(glDrawTransformFeedbackInstanced, "glDrawTransformFeedbackInstanced");
    bind(glDrawTransformFeedbackStreamInstanced, "glDrawTransformFeedbackStreamInstanced");

    // ARB_internalformat_query
    bind(glGetInternalformativ, "glGetInternalformativ");

    // ARB_shader_atomic_counters
    bind(glGetActiveAtomicCounterBufferiv, "glGetActiveAtomicCounterBufferiv");

    // ARB_shader_image_load_store
    bind(glBindImageTexture, "glBindImageTexture");
    bind(glMemoryBarrier, "glMemoryBarrier");

    // ARB_texture_storage
    bind(glTexStorage1D, "glTexStorage1D");
    bind(glTexStorage2D, "glTexStorage2D");
    bind(glTexStorage3D, "glTexStorage3D");
    bind(glTextureStorage1DEXT, "glTextureStorage1DEXT");
    bind(glTextureStorage2DEXT, "glTextureStorage2DEXT");
    bind(glTextureStorage3DEXT, "glTextureStorage3DEXT");
}

#endif
