#pragma once

#include "sparse_bindless_texarray.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Textures_GL_Sparse_Bindless_Texture_Array : public Textures
{
public:
    Textures_GL_Sparse_Bindless_Texture_Array();
    virtual ~Textures_GL_Sparse_Bindless_Texture_Array();

    virtual bool init() override;

    virtual bool begin(void* hwnd, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) override;
    virtual void end(GfxSwapChain* swap_chain) override;

    virtual void draw(Matrix* transforms, int count) override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec2 tex;
    };

    GLuint m_ib;
    GLuint m_vb_pos;
    GLuint m_vb_tex;
    GLuint m_tex1;
    GLuint m_tex2;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_transform_buffer;

    GLuint64 m_tex1_handle;
    GLuint64 m_tex2_handle;

    DrawElementsIndirectCommand m_commands[TEXTURES_COUNT];

    TextureManager mTexManager;
};
