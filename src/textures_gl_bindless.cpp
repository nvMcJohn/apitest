#include "pch.h"

#include "textures_gl_bindless.h"
#include "mathlib.h"
#include "console.h"

// This doesn't seem to cost very much. Good news!
#define GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER 0

Textures_GL_Bindless::Textures_GL_Bindless()
    : m_ib()
    , m_vb_pos()
    , m_vb_tex()
    , m_tex1()
    , m_tex2()
    , m_vs()
    , m_fs()
    , m_prog()
    , m_transform_buffer()
    , m_tex1_handle()
    , m_tex2_handle()
{}

Textures_GL_Bindless::~Textures_GL_Bindless()
{
#if GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER
    glMakeTextureHandleNonResidentARB(m_tex1_handle);
    glMakeTextureHandleNonResidentARB(m_tex2_handle);
#endif

    m_tex1_handle = m_tex2_handle = 0;

    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb_pos);
    glDeleteBuffers(1, &m_vb_tex);
    glDeleteBuffers(1, &m_transform_buffer);
    glDeleteTextures(1, &m_tex1);
    glDeleteTextures(1, &m_tex2);
    glDeleteShader(m_vs);
    glDeleteShader(m_fs);
    glDeleteProgram(m_prog);
}

bool Textures_GL_Bindless::init()
{
    if (glGetTextureHandleARB == nullptr) {
        console::debug("Textures_GL_Bindless requires support for bindless textures (duh?). Cannot start this test.");
        return false;
    }

    // Shaders
    if (!create_shader(GL_VERTEX_SHADER, "textures_gl_bindless_vs.glsl", &m_vs))
        return false;

    if (!create_shader(GL_FRAGMENT_SHADER, "textures_gl_bindless_fs.glsl", &m_fs))
        return false;

    if (!compile_program(&m_prog, m_vs, m_fs, 0))
        return false;

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

    GLuint texs[2] = { 0 };

    glGenTextures(2, texs);
    m_tex1 = texs[0];
    m_tex2 = texs[1];

    // Textures (TODO: This should go in a utility function elsewhere; also we should cache the texture loads. For now; whatevs).
    TextureDetails tex_details;
    if (!readDDSFromFile("Media/tex/Mandelbrot.dds", &tex_details)) {
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_tex1);
    glTexStorage2D(GL_TEXTURE_2D, tex_details.szMipMapCount, tex_details.glFormat,
                   tex_details.dwWidth, tex_details.dwHeight);

    size_t offset = 0;
    for (int mip = 0; mip < tex_details.szMipMapCount; ++mip) {
        glCompressedTexSubImage2D(GL_TEXTURE_2D, mip, 0, 0, tex_details.MipMapWidth(mip), tex_details.MipMapHeight(mip), tex_details.glFormat, tex_details.pSizes[mip], (char*)tex_details.pPixels + offset);
        offset += tex_details.pSizes[mip];
    }

    if (!readDDSFromFile("Media/tex/image.dds", &tex_details)) {
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_tex2);
    glTexStorage2D(GL_TEXTURE_2D, tex_details.szMipMapCount, tex_details.glFormat,
                   tex_details.dwWidth, tex_details.dwHeight);

    // Bindless
    m_tex1_handle = glGetTextureHandleARB(m_tex1);
    m_tex2_handle = glGetTextureHandleARB(m_tex2);

    if (m_tex1_handle == 0 || m_tex2_handle == 0) {
        return false;
    }

#if GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER
    glMakeTextureHandleResidentARB(m_tex1_handle);
    glMakeTextureHandleResidentARB(m_tex2_handle);
#endif

    offset = 0;
    for (int mip = 0; mip < tex_details.szMipMapCount; ++mip) {
        glCompressedTexSubImage2D(GL_TEXTURE_2D, mip, 0, 0, tex_details.MipMapWidth(mip), tex_details.MipMapHeight(mip), tex_details.glFormat, tex_details.pSizes[mip], (char*)tex_details.pPixels + offset);
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

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_1D, tex);
    glBindTexture(GL_TEXTURE_1D, 0);
    glDeleteTextures(1, &tex);

    tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_1D, tex);
    glBindTexture(GL_TEXTURE_1D, 0);
    glDeleteTextures(1, &tex);

    tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_1D, tex);
    glBindTexture(GL_TEXTURE_1D, 0);
    glDeleteTextures(1, &tex);

    return glGetError() == GL_NO_ERROR;
}

bool Textures_GL_Bindless::begin(GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(swap_chain->wnd, &width, &height);

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

#if !GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER
    glMakeTextureHandleResidentARB(m_tex1_handle);
    glMakeTextureHandleResidentARB(m_tex2_handle);
#endif

    return true;
}

void Textures_GL_Bindless::end(GfxSwapChain* swap_chain)
{
#if !GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER
    glMakeTextureHandleNonResidentARB(m_tex1_handle);
    glMakeTextureHandleNonResidentARB(m_tex2_handle);
#endif

    SDL_GL_SwapWindow(swap_chain->wnd);
#if defined(_DEBUG)
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
#endif
}

void Textures_GL_Bindless::draw(Matrix* transforms, int count)
{
    assert(count <= TEXTURES_COUNT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_transform_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrix) * count, transforms, GL_DYNAMIC_DRAW);

    for (int i = 0; i < count; ++i) {
        // Update the draw ID (since we cannot use multi_draw here
        glUniform1i(1, i); 

        // And update our texture via bindless
        // Note: Intentionally doing this different than Textures_GL_Forward so it's obvious
        // that the test is different.
        GLuint64 activeTex = ((i & 1) == 1) ? m_tex1_handle : m_tex2_handle;

        glUniformHandleui64ARB(128, activeTex);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    }
}
