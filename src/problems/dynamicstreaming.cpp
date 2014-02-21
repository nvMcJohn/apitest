#include "pch.h"

#include "problems/dynamicstreaming.h"
#include "solutions/dynamicstreamingsoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
DynamicStreamingProblem::DynamicStreamingProblem()
: mIteration()
{

}

DynamicStreamingProblem::~DynamicStreamingProblem()
{
}

// --------------------------------------------------------------------------------------------------------------------
bool DynamicStreamingProblem::Init()
{
    mVertexData.resize(kParticleCount * kVertsPerParticle);

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingProblem::Render()
{
    // TODO: Update should be moved into its own thread, but for now let's just do it here.
    Update();

    if (mActiveSolution) {
        reinterpret_cast<DynamicStreamingSolution*>(mActiveSolution)->Render(mVertexData);
    }
}

// --------------------------------------------------------------------------------------------------------------------
bool DynamicStreamingProblem::SetSolution(Solution* _solution)
{
    if (!Problem::SetSolution(_solution)) {
        return false;
    }

    if (mActiveSolution) {
        console::log("Solution %s - Initializing.", mActiveSolution->GetName().c_str());
        bool retVal = reinterpret_cast<DynamicStreamingSolution*>(mActiveSolution)->Init();
        console::log("Solution %s - Initialize complete (Success: %s).", mActiveSolution->GetName().c_str(), retVal ? "true" : "false");
        return retVal;
    }

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingProblem::Shutdown()
{
    mVertexData.clear();
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingProblem::Update()
{
    const float spacing = 1.0f;
    const float w = 1.0f;
    const float h = 1.0f;

    const int kMarchPixelsX = 24;
    const int kMarchPixelsY = 128;

    const float offsetX = (mIteration % kMarchPixelsX) * w;
    const float offsetY = ((mIteration / kMarchPixelsX) % kMarchPixelsY) * h;

    size_t address = 0;
    for (int yPos = 0; yPos < kParticleCountY; ++yPos) {
        float y = spacing + yPos * (spacing + h);

        for (int xPos = 0; xPos < kParticleCountX; ++xPos) {
            float x = spacing + xPos * (spacing + w);
            
            Vec2 verts[] = {
                { x + offsetX + 0, y + offsetY + 0 },
                { x + offsetX + w, y + offsetY + 0 },
                { x + offsetX + 0, y + offsetY + h },
                { x + offsetX + w, y + offsetY + 0 },
                { x + offsetX + 0, y + offsetY + h },
                { x + offsetX + w, y + offsetY + h },
            };

            mVertexData[address + 0] = verts[0];
            mVertexData[address + 1] = verts[1];
            mVertexData[address + 2] = verts[2];
            mVertexData[address + 3] = verts[3];
            mVertexData[address + 4] = verts[4];
            mVertexData[address + 5] = verts[5];

            address += kVertsPerParticle;
        }
    }

    ++mIteration;
}
