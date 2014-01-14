#include <assert.h>

#include "sparse_bindless_texarray.h"


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Texture2D::Texture2D(Texture2DContainer* container, GLsizei sliceNum)
: mContainer(container)
, mSliceNum(GLfloat(sliceNum))
{
    assert(mContainer);
}

// ------------------------------------------------------------------------------------------------
Texture2D::~Texture2D()
{
    free();
}

// ------------------------------------------------------------------------------------------------
void Texture2D::commit()
{
    mContainer->commit(this);
}

// ------------------------------------------------------------------------------------------------
void Texture2D::free()
{
    mContainer->free(this);
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Texture2DContainer::Texture2DContainer(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei slices)
: mWidth(width)
, mHeight(height)
, mLevels(levels)
, mXTileSize(0)
, mYTileSize(0)
{
    glGenTextures(1, &mTexId);
    glBindTexture(GL_TEXTURE_2D_ARRAY, mTexId);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_SPARSE_ARB, GL_TRUE);

    // TODO: This could be done once per internal format. For now, just do it every time.
    GLint indexCount = 0,
        xSize = 0,
        ySize = 0,
        zSize = 0;

    GLint bestIndex = -1,
        bestXSize = 0,
        bestYSize = 0;

    glGetInternalformativ(GL_TEXTURE_2D_ARRAY, internalformat, GL_NUM_VIRTUAL_PAGE_SIZES_ARB, 1, &indexCount);
    for (GLint i = 0; i < indexCount; ++i) {
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, i);
        glGetInternalformativ(GL_TEXTURE_2D_ARRAY, internalformat, GL_VIRTUAL_PAGE_SIZE_X_ARB, 1, &xSize);
        glGetInternalformativ(GL_TEXTURE_2D_ARRAY, internalformat, GL_VIRTUAL_PAGE_SIZE_Y_ARB, 1, &ySize);
        glGetInternalformativ(GL_TEXTURE_2D_ARRAY, internalformat, GL_VIRTUAL_PAGE_SIZE_Z_ARB, 1, &zSize);

        // For our purposes, the "best" format is the one that winds up with Z=1 and the largest x and y sizes.
        if (zSize == 1) {
            if (xSize >= bestXSize && ySize >= bestYSize) {
                bestIndex = i;
                bestXSize = xSize;
                bestYSize = ySize;
            }
        }
    }

    // This would mean the implementation has no valid sizes for us, or that this format doesn't actually support sparse
    // texture allocation.
    assert(bestIndex != -1);

    mXTileSize = bestXSize;

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_VIRTUAL_PAGE_SIZE_INDEX_ARB, bestIndex);

    // We've set all the necessary parameters, now it's time to create the sparse texture.
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, internalformat, width, height, slices);
    for (GLsizei i = 0; i < slices; ++i) {
        mFreeList.push(i);
    }
}

// ------------------------------------------------------------------------------------------------
GLsizei Texture2DContainer::hasRoom() const
{
    return mFreeList.size() > 0;
}

// ------------------------------------------------------------------------------------------------
GLsizei Texture2DContainer::virtualAlloc()
{
    GLsizei retVal = mFreeList.front();
    mFreeList.pop();
    return retVal;
}

// ------------------------------------------------------------------------------------------------
void Texture2DContainer::virtualFree(GLsizei slice)
{
    mFreeList.push(slice);
}

// ------------------------------------------------------------------------------------------------
void Texture2DContainer::commit(Texture2D* _tex)
{
    assert(_tex->getTexture2DContainer() == this);

    changeCommitment(_tex->getSliceNum(), GL_TRUE);
}

// ------------------------------------------------------------------------------------------------
void Texture2DContainer::free(Texture2D* _tex)
{
    assert(_tex->getTexture2DContainer() == this);
    changeCommitment(_tex->getSliceNum(), GL_FALSE);
}

// ------------------------------------------------------------------------------------------------
void Texture2DContainer::changeCommitment(GLsizei slice, GLboolean commit)
{
    GLenum err = glGetError();

    GLsizei levelWidth = mWidth,
        levelHeight = mHeight;

    for (int level = 0; level < mLevels; ++level) {
        glTexturePageCommitmentEXT(mTexId, level, 0, 0, slice, levelWidth, levelHeight, 1, commit);
        levelWidth = max(levelWidth / 2, 1);
        levelHeight = max(levelHeight / 2, 1);
    }

    err = glGetError();
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
TextureManager::TextureManager()
: mInited(false)
, mMaxTextureArrayLevels()
{

}

// ------------------------------------------------------------------------------------------------
Texture2D* TextureManager::newTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
    assert(mInited);
    Texture2D* retTex = allocTexture2D(levels, internalformat, width, height);
    retTex->commit();

    return retTex;
}

// ------------------------------------------------------------------------------------------------
void TextureManager::free(Texture2D* _tex)
{
    assert(mInited);

}

// ------------------------------------------------------------------------------------------------
bool TextureManager::init()
{
    glGetIntegerv(GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB, &mMaxTextureArrayLevels);

    mInited = true;
    return true;
}

// ------------------------------------------------------------------------------------------------
Texture2D* TextureManager::allocTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
    Texture2DContainer* memArray = nullptr;

    auto texType = std::make_tuple(levels, internalformat, width, height);
    auto arrayIt = mTexArrays2D.find(texType);
    if (arrayIt == mTexArrays2D.end()) {
        mTexArrays2D[texType] = std::vector<Texture2DContainer*>();
        arrayIt = mTexArrays2D.find(texType);
        assert(arrayIt != mTexArrays2D.end());
    }

    for (auto it = arrayIt->second.begin(); it != arrayIt->second.end(); ++it) {
        if ((*it)->hasRoom()) {
            memArray = (*it);
            break;
        }
    }

    if (memArray == nullptr) {
        memArray = new Texture2DContainer(levels, internalformat, width, height, mMaxTextureArrayLevels);
        arrayIt->second.push_back(memArray);
    }

    assert(memArray);
    return new Texture2D(memArray, memArray->virtualAlloc());
}