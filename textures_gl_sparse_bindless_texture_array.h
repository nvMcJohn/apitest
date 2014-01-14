#pragma once

#include "gfx_gl.h"
#include <map>
#include <queue>
#include <tuple>
#include <vector>

class Texture2DContainer;

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Texture
{
public:
    Texture(Texture2DContainer* container, GLsizei sliceNum);
    ~Texture();
    void commit();
    void free();

    const Texture2DContainer* getTexture2DContainer() const { return mContainer; }
    GLsizei getSliceNum() const { return (GLsizei)mSliceNum; }

private:
    Texture2DContainer* mContainer;

    // Kept as a float because that's how we need to write the uniform, and that should be done
    // much more often than changing data or adjusting residency.
    GLfloat mSliceNum;
};

class Texture2DContainer
{
public:
    Texture2DContainer(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei slices);
    GLsizei hasRoom() const;
    GLsizei virtualAlloc();
    void virtualFree(GLsizei slice);

    void commit(Texture* _tex);
    void free(Texture* _tex);

private:
    GLuint mTexId;
    std::queue<GLsizei> mFreeList;

    const GLsizei mWidth;
    const GLsizei mHeight;
    const GLsizei mLevels;
    GLsizei mXTileSize;
    GLsizei mYTileSize;

    void changeCommitment(GLsizei slice, GLboolean commit);
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class TextureManager
{
public:
    TextureManager();
    Texture* newTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    void free(Texture* _tex);
    
    bool init();
private:

    std::map<std::tuple<GLsizei, GLenum, GLsizei, GLsizei>, std::vector<Texture2DContainer*>> mTexArrays2D;
    GLsizei mMaxTextureArrayLevels;
    bool mInited;

    Texture* allocTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
};

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
