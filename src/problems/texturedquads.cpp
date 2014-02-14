
#include "pch.h"
#include "problems/texturedquads.h"
#include "solutions/texturedquadssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
const char* kTextureNames[] = {
    "Media/tex/Mandelbrot.dds",
    "Media/tex/image.dds",
};

const int kObjectsX = 100;
const int kObjectsY = 100;
const int kObjectCount = kObjectsX * kObjectsY;

// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsProblem::TexturedQuadsProblem()
{

}

// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsProblem::~TexturedQuadsProblem()
{
    for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
        SafeDelete(*it);
    }

    mTextures.clear();
}

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsProblem::Init()
{
    genUnitQuad();
    mTransforms.resize(kObjectCount);

    return loadTextures();
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsProblem::Render()
{
    Update();

    if (mActiveSolution) {
        reinterpret_cast<TexturedQuadsSolution*>(mActiveSolution)->Render(mTransforms);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsProblem::Shutdown()
{
    Problem::Shutdown();

    for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
        SafeDelete(*it);
    }
    mTextures.clear();

    mIndices.clear();
    mVertices.clear();
    mTransforms.clear();
}

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsProblem::SetSolution(Solution* _solution)
{
    if (!Problem::SetSolution(_solution)) {
        return false;
    }

    if (mActiveSolution) {
        console::log("Solution %s - Initializing.", mActiveSolution->GetName().c_str());
        bool retVal = reinterpret_cast<TexturedQuadsSolution*>(mActiveSolution)->Init(mVertices, mIndices, mTextures, kObjectCount);
        console::log("Solution %s - Initialize complete (Success: %s).", mActiveSolution->GetName().c_str(), retVal ? "true" : "false");

        return retVal;
    }

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsProblem::Update()
{
    const float angle = mIteration * 0.01f;

    Matrix *m = &*mTransforms.begin(); 
    for (int x = 0; x < kObjectsX; ++x) {
        for (int y = 0; y < kObjectsY; ++y) {
            *m = matrix_rotation_z(angle);
            m->w.x = 2.0f * x - kObjectsX;
            m->w.y = 2.0f * y - kObjectsY;
            m->w.z = 0.0f;
            ++m;
        }
    }

    ++mIteration;

    if (angle > 2 * M_PI) {
        mIteration = 0;
    }
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsProblem::genUnitQuad()
{
    // Buffers
    const Vertex vertices[] =
    {
        { -0.5, -0.5f,  0.0f, 0.0f, 0.0f, },
        {  0.5, -0.5f,  0.0f, 0.0f, 1.0f, },
        {  0.5,  0.5f,  0.0f, 1.0f, 0.0f, },
        { -0.5,  0.5f,  0.0f, 1.0f, 1.0f  },
    };

    for (int i = 0; i < ArraySize(vertices); ++i) {
        Vertex vert = vertices[i];
        mVertices.push_back(vert);
    }

    const uint16_t indices[] =
    {
        0, 1, 2, 0, 2, 3,
    };

    for (int i = 0; i < ArraySize(indices); ++i) {
        mIndices.push_back(indices[i]);
    }
}

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsProblem::loadTextures()
{
    for (int i = 0; i < ArraySize(kTextureNames); ++i) {
        TextureDetails* texDetails = new TextureDetails;
        if (!readDDSFromFile(kTextureNames[i], texDetails)) {
            delete texDetails;
            console::warn("Failed to initialize %s, couldn't load texture '%s' from disk.", GetName().c_str(), kTextureNames[i]);
            return false;
        }

        mTextures.push_back(texDetails);
    }
    
    return true;
}
