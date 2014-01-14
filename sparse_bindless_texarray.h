#pragma once

#include "gfx_gl.h"
#include <map>
#include <queue>
#include <tuple>
#include <vector>

class Texture2D;
class Texture2DContainer;
class TextureManager;

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

    const Texture2DContainer* getTexture2DContainer() const { return mContainer; }
    GLsizei getSliceNum() const { return (GLsizei)mSliceNum; }

private:
    Texture2DContainer* mContainer;

    // Kept as a float because that's how we need to write the uniform, and that should be done
    // much more often than changing data or adjusting residency.
    GLfloat mSliceNum;
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Texture2DContainer
{
public:
    Texture2DContainer(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei slices);
    GLsizei hasRoom() const;
    GLsizei virtualAlloc();
    void virtualFree(GLsizei slice);

    void commit(Texture2D* _tex);
    void free(Texture2D* _tex);

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
    Texture2D* newTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
    void free(Texture2D* _tex);

    bool init();
private:

    std::map<std::tuple<GLsizei, GLenum, GLsizei, GLsizei>, std::vector<Texture2DContainer*>> mTexArrays2D;
    GLsizei mMaxTextureArrayLevels;
    bool mInited;

    Texture2D* allocTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
};
