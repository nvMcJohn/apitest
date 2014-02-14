#include "pch.h"
#include "problems/untexturedobjects.h"

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
const int kObjectsX = 64;
const int kObjectsY = 64;
const int kObjectsZ = 64;
const int kObjectCount = kObjectsX * kObjectsY * kObjectsZ;

const int kTransformCount = kObjectCount;
const int kVertexCount = 8;
const int kIndexCount = 36;

// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsProblem::UntexturedObjectsProblem()
: mIteration()
{

}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsProblem::Init()
{
    genUnitCube();
    mTransforms.resize(kTransformCount);

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsProblem::Shutdown()
{
    Problem::Shutdown();

    mIndices.clear();
    mVertices.clear();
    mTransforms.clear();
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsProblem::Render()
{
    Update();

    if (mActiveSolution) {
        reinterpret_cast<UntexturedObjectsSolution*>(mActiveSolution)->Render(mTransforms);
    }
}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsProblem::SetSolution(Solution* _solution)
{
    if (!Problem::SetSolution(_solution)) {
        return false;
    }

    if (mActiveSolution) {
        console::log("Solution %s - Initializing.", mActiveSolution->GetName().c_str());
        bool retVal = reinterpret_cast<UntexturedObjectsSolution*>(mActiveSolution)->Init(mVertices, mIndices, kObjectCount);
        console::log("Solution %s - Initialize complete (Success: %s).", mActiveSolution->GetName().c_str(), retVal ? "true" : "false");

        return retVal;
    }

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsProblem::Update()
{
    const float angle = mIteration * 0.01f;

    Matrix *m = &*mTransforms.begin();
    for (int x = 0; x < kObjectsX; ++x) {
        for (int y = 0; y < kObjectsY; ++y) {
            for (int z = 0; z < kObjectsZ; ++z) {
                *m = matrix_rotation_z(angle);
                m->w.x = 2.0f * x - kObjectsX;
                m->w.y = 2.0f * y - kObjectsY;
                m->w.z = 2.0f * z - kObjectsZ;
                ++m;
            }
        }
    }

    ++mIteration;

    if (angle > 2 * M_PI) {
        mIteration = 0;
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsProblem::genUnitCube()
{
    const Vertex vertices[] = {
        { -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f },
        {  0.5f,  0.5f, -0.5f,  1.0f,  1.0f,  0.0f },
        {  0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f },
        { -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  1.0f },
        { -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f },
        {  0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  1.0f },
        {  0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f },
        { -0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  0.0f },
    };

    for (int i = 0; i < ArraySize(vertices); ++i) {
        mVertices.push_back(vertices[i]);
    }
    assert(mVertices.size() == kVertexCount);

    const uint16_t indices[] =
    {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        3, 2, 5, 3, 5, 4,
        2, 1, 6, 2, 6, 5,
        1, 7, 6, 1, 0, 7,
        0, 3, 4, 0, 4, 7
    };

    for (int i = 0; i < ArraySize(indices); ++i) {
        mIndices.push_back(indices[i]);
    }

    assert(mIndices.size() == kIndexCount);
}
