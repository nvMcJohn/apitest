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

// --------------------------------------------------------------------------------------------------------------------
bool DynamicStreamingProblem::Init()
{
    mVertexData.resize(kParticleCount);

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

    const float offsetX = (mIteration % 2) * w;
    const float offsetY = ((mIteration / 2) % 2) * h;

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
