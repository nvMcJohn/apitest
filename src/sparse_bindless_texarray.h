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

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Texture2DContainer
{
public:
    Texture2DContainer(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei slices);
    ~Texture2DContainer();
    GLsizei hasRoom() const;
    GLsizei virtualAlloc();
    void virtualFree(GLsizei slice);

    void commit(Texture2D* _tex);
    void free(Texture2D* _tex);

    void CompressedTexSubImage3D(GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid * data);

    // Returns the handle to this container.
    GLuint64 GetHandle() const { return mHandle; }

private:
    GLuint64 mHandle;
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
    Texture2D* newTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    void free(Texture2D* _tex);

    bool init();
private:

    std::map<std::tuple<GLsizei, GLenum, GLsizei, GLsizei>, std::vector<Texture2DContainer*>> mTexArrays2D;
    GLsizei mMaxTextureArrayLevels;
    bool mInited;

    Texture2D* allocTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
};
