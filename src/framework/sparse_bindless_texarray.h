#pragma once

#include "gfx_gl.h"
#include <map>
#include <queue>
#include <tuple>
#include <vector>

class Texture2D;
class Texture2DContainer;
class TextureManager;

struct TexAddress
{
    GLuint64 m_container_handle;
    GLfloat m_tex_page;
    GLint reserved;
};

struct DenseTexAddress
{
    GLuint m_container_index;
    GLfloat m_layer;
    GLint reserved[2];
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Texture2DContainer
{
public:
    Texture2DContainer(bool sparse, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei slices);
    ~Texture2DContainer();
    GLsizei hasRoom() const;
    GLsizei virtualAlloc();
    void virtualFree(GLsizei slice);

    void commit(Texture2D* _tex);
    void free(Texture2D* _tex);

    void CompressedTexSubImage3D(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);

    // Returns the handle to this container.
    GLuint64 GetHandle() const { return mHandle; }
    GLuint GetTexId() const { return mTexId; }

private:
    GLuint64 mHandle;
    GLuint mTexId;
    std::queue<GLsizei> mFreeList;

    const GLsizei mWidth;
    const GLsizei mHeight;
    const GLsizei mLevels;
    const GLsizei mSlices;
    GLsizei mXTileSize;
    GLsizei mYTileSize;

    void changeCommitment(GLsizei slice, GLboolean commit);
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Texture2D
{
public:
    Texture2D(Texture2DContainer* container, GLsizei sliceNum);
    ~Texture2D();
    void commit();
    void free();

    void CompressedTexSubImage2D(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid* data);

    const Texture2DContainer* getTexture2DContainer() const { return mContainer; }
    GLsizei getSliceNum() const { return (GLsizei)mSliceNum; }

    TexAddress GetAddress() const
    {
        TexAddress ta = { mContainer->GetHandle(), mSliceNum };
        return ta;
    }

    GLuint GetTexId() const { return mContainer->GetTexId(); }

private:
    Texture2DContainer* mContainer;

    // Kept as a float because that's how we need to write the uniform, and that should be done
    // much more often than changing data or adjusting residency.
    GLfloat mSliceNum;
};


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    Texture2D* newTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    Texture2D* newTexture2DFromDetails(const TextureDetails* _texDetails);

    void free(Texture2D* _tex);

    // maxNumTextures <= 0 will cause allocation of maximum number of layers
    bool Init(bool sparse=true, GLsizei maxNumTextures=-1);
    void Shutdown();
private:

    std::map<std::tuple<GLsizei, GLenum, GLsizei, GLsizei>, std::vector<Texture2DContainer*>> mTexArrays2D;
    GLsizei mMaxTextureArrayLevels;
    bool mInited;
    bool mSparse;

    Texture2D* allocTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
};
