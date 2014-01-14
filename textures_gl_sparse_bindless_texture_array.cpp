#include "Textures_GL_Sparse_Bindless_Texture_Array.h"
#include "mathlib.h"
#include <assert.h>
#include <stdint.h>
#include "console.h"

#include "dds_helper.h"

// This doesn't seem to cost very much. Good news!
#define GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER 1

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Texture::Texture(Texture2DContainer* container, GLsizei sliceNum)
    : mContainer(container)
    , mSliceNum(GLfloat(sliceNum))
{
    assert(mContainer);
}

// ------------------------------------------------------------------------------------------------
Texture::~Texture()
{
    free();
}

// ------------------------------------------------------------------------------------------------
void Texture::commit()
{
    mContainer->commit(this);
}

// ------------------------------------------------------------------------------------------------
void Texture::free()
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
void Texture2DContainer::commit(Texture* _tex)
{
    assert(_tex->getTexture2DContainer() == this);

    changeCommitment(_tex->getSliceNum(), GL_TRUE);
}

// ------------------------------------------------------------------------------------------------
void Texture2DContainer::free(Texture* _tex)
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
        glTexturePageCommitmentEXT(mTexId, level, 0, 0, slice,levelWidth, levelHeight, 1, commit);
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
Texture* TextureManager::newTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
{
    assert(mInited);
    Texture* retTex = allocTexture2D(levels, internalformat, width, height);
    retTex->commit();

    return retTex;
}

// ------------------------------------------------------------------------------------------------
void TextureManager::free(Texture* _tex)
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
Texture* TextureManager::allocTexture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)
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
    return new Texture(memArray, memArray->virtualAlloc());
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Textures_GL_Sparse_Bindless_Texture_Array::Textures_GL_Sparse_Bindless_Texture_Array()
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

// ------------------------------------------------------------------------------------------------
Textures_GL_Sparse_Bindless_Texture_Array::~Textures_GL_Sparse_Bindless_Texture_Array()
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

// ------------------------------------------------------------------------------------------------
bool Textures_GL_Sparse_Bindless_Texture_Array::init()
{
    if (!mTexManager.init()) {
        return false;
    }

    Texture* myTex = mTexManager.newTexture2D(10, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 512, 512);
    delete myTex; 
    myTex = nullptr;

    if (glGetTextureHandleARB == nullptr) {
        console::debug("Textures_GL_Sparse_Bindless_Texture_Array requires support for bindless textures (duh?). Cannot start this test.");
        return false;
    }

    // Shaders
    if (!create_shader(GL_VERTEX_SHADER, "textures_gl_sparse_bindless_texture_array_vs.glsl", &m_vs))
        return false;

    if (!create_shader(GL_FRAGMENT_SHADER, "textures_gl_sparse_bindless_texture_array_fs.glsl", &m_fs))
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

// ------------------------------------------------------------------------------------------------
bool Textures_GL_Sparse_Bindless_Texture_Array::begin(void* window, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
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

#if !GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER
    glMakeTextureHandleResidentARB(m_tex1_handle);
    glMakeTextureHandleResidentARB(m_tex2_handle);
#endif

    return true;
}

// ------------------------------------------------------------------------------------------------
void Textures_GL_Sparse_Bindless_Texture_Array::end(GfxSwapChain* swap_chain)
{
#if !GL_TEXTURE_BINDLESS_RESIDENCY_ONCE_EVER
    glMakeTextureHandleNonResidentARB(m_tex1_handle);
    glMakeTextureHandleNonResidentARB(m_tex2_handle);
#endif

    SwapBuffers(swap_chain->hdc);
#if defined(_DEBUG)
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
#endif
}

// ------------------------------------------------------------------------------------------------
void Textures_GL_Sparse_Bindless_Texture_Array::draw(Matrix* transforms, int count)
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
