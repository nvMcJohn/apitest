#include "textures_gl_sparse_bindless_texture_array_multidraw.h"
#include "mathlib.h"
#include <assert.h>
#include <stdint.h>
#include "console.h"

#include "dds_helper.h"

// This doesn't seem to cost very much. Good news!
#define GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER 0

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw::Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw()
    : m_ib()
    , m_vb_pos()
    , m_vb_tex()
    , m_vs()
    , m_fs()
    , m_prog()
    , m_texbuf()
    , m_transform_buffer()
{}

// ------------------------------------------------------------------------------------------------
Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw::~Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw()
{
    delete mTex2;
    delete mTex1;

    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb_pos);
    glDeleteBuffers(1, &m_vb_tex);
    glDeleteBuffers(1, &m_transform_buffer);
    glDeleteBuffers(1, &m_texbuf);
    glDeleteShader(m_vs);
    glDeleteShader(m_fs);
    glDeleteProgram(m_prog);
}

// ------------------------------------------------------------------------------------------------
bool Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw::init()
{
    if (!mTexManager.init()) {
        return false;
    }

#if 1
    if (glGetTextureHandleARB == nullptr) {
        console::debug("Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw requires support for bindless textures (duh?). Cannot start this test.");
        return false;
    }
#endif

    // Shaders
    if (!create_shader(GL_VERTEX_SHADER, "textures_gl_sparse_bindless_texture_array_multidraw_vs.glsl", &m_vs))
        return false;

    if (!create_shader(GL_FRAGMENT_SHADER, "textures_gl_sparse_bindless_texture_array_multidraw_fs.glsl", &m_fs))
        return false;

    if (!compile_program(&m_prog, m_vs, m_fs, 0))
        return false;

    const char* uninames[] = {
        "texAddress[0].Container",
        "texAddress[0].Page",
    };

    GLuint uniindices[ARRAYSIZE(uninames)] = { 0 };
    GLint offsets[ARRAYSIZE(uninames)] = { 0 };
    GLint sizes[ARRAYSIZE(uninames)] = { 0 };
    GLint arraystrides[ARRAYSIZE(uninames)] = { 0 };

    for (int i = 0; i < ARRAYSIZE(uninames); ++i) {
        uniindices[i] = glGetProgramResourceIndex(m_prog, GL_BUFFER_VARIABLE, uninames[i]);
    }

    for (int i = 0; i < ARRAYSIZE(uniindices); ++i) {
        GLenum props[] = { GL_OFFSET, GL_TOP_LEVEL_ARRAY_STRIDE };
        GLint results[ARRAYSIZE(props)] = { 0 };
        glGetProgramResourceiv(m_prog, GL_BUFFER_VARIABLE, uniindices[i], ARRAYSIZE(props), props, ARRAYSIZE(props), NULL, results);
        offsets[i] = results[0];
        arraystrides[i] = results[1];
    }

    glGetActiveUniformsiv(m_prog, ARRAYSIZE(uninames), uniindices, GL_UNIFORM_OFFSET, offsets);

    // Buffers
    Vec3 vertices_pos[] =
    {
        { -0.5, -0.5f,  0.0f },
        {  0.5, -0.5f,  0.0f },
        {  0.5,  0.5f,  0.0f },
        { -0.5,  0.5f,  0.0f },
    };

    Vec2 vertices_tex[] = 
    {
        { 0, 0 },
        { 0, 1 },
        { 1, 0 },
        { 1, 1 }
    };

    GLushort indices[] =
    {
        0, 1, 2, 0, 2, 3,
    };

    // Textures (TODO: This should go in a utility function elsewhere; also we should cache the texture loads. For now; whatevs).
    TextureDetails tex_details;
    if (!readDDSFromFile("Media/tex/Mandelbrot.dds", &tex_details)) {
        return false;
    }

    mTex1 = mTexManager.newTexture2D(tex_details.szMipMapCount, tex_details.glFormat, tex_details.dwWidth, tex_details.dwHeight);

    size_t offset = 0;
    for (int mip = 0; mip < tex_details.szMipMapCount; ++mip) {
        mTex1->CompressedTexSubImage2D(mip, 0, 0, tex_details.MipMapWidth(mip), tex_details.MipMapHeight(mip), tex_details.glFormat, tex_details.pSizes[mip], (char*)tex_details.pPixels + offset);
        offset += tex_details.pSizes[mip];
    }

    if (!readDDSFromFile("Media/tex/image.dds", &tex_details)) {
        return false;
    }

    mTex2 = mTexManager.newTexture2D(tex_details.szMipMapCount, tex_details.glFormat, tex_details.dwWidth, tex_details.dwHeight);

    offset = 0;
    for (int mip = 0; mip < tex_details.szMipMapCount; ++mip) {
        mTex2->CompressedTexSubImage2D(mip, 0, 0, tex_details.MipMapWidth(mip), tex_details.MipMapHeight(mip), tex_details.glFormat, tex_details.pSizes[mip], (char*)tex_details.pPixels + offset);
        offset += tex_details.pSizes[mip];
    }

    // Buffers
    GLuint vbs[2] = { 0 };

    glGenBuffers(2, vbs);
    m_vb_pos = vbs[0];
    m_vb_tex = vbs[1];

    glBindBuffer(GL_ARRAY_BUFFER, m_vb_pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pos), vertices_pos, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vb_tex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tex), vertices_tex, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_transform_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_transform_buffer);

    assert(TEXTURES_COUNT % 2 == 0);
    TexAddress addr[TEXTURES_COUNT];
    for (int i = 0; i < TEXTURES_COUNT; i += 2) {
        addr[i + 0] = mTex1->GetAddress();
        addr[i + 1] = mTex2->GetAddress();
    }    

    glGenBuffers(1, &m_texbuf);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_texbuf);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(addr), addr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_texbuf);

    return glGetError() == GL_NO_ERROR;
}

// ------------------------------------------------------------------------------------------------
bool Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw::begin(void* window, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
{
    HWND hWnd = reinterpret_cast<HWND>(window);

    RECT rc;
    GetClientRect(hWnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    // Bind and clear frame buffer
    int fbo = PTR_AS_INT(frame_buffer);
    float c[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float d = 1.0f;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glClearBufferfv(GL_COLOR, 0, c);
    glClearBufferfv(GL_DEPTH, 0, &d);

    // Program
    Vec3 dir = { 0, 0, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 1, 0 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix proj = matrix_perspective_rh_gl(radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);
    Matrix view_proj = proj * view;

    glUseProgram(m_prog);
    glUniformMatrix4fv(0, 1, GL_TRUE, &view_proj.x.x);

    // Input Layout. First the IB
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);

    // Then the VBs (SOA).
    glBindBuffer(GL_ARRAY_BUFFER, m_vb_pos);
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb_tex);
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, 0, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Rasterizer State
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, width, height);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    return true;
}

// ------------------------------------------------------------------------------------------------
void Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw::end(GfxSwapChain* swap_chain)
{
    SwapBuffers(swap_chain->hdc);
#if defined(_DEBUG)
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
#endif
}

// ------------------------------------------------------------------------------------------------
void Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw::draw(Matrix* transforms, int count)
{
    assert(count <= TEXTURES_COUNT);

    for (int i = 0; i < count; ++i)
    {
        DrawElementsIndirectCommand *cmd = &m_commands[i];
        cmd->count = 6;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = 0;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_transform_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrix)* count, transforms, GL_DYNAMIC_DRAW);

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, m_commands, count, 0);
}
