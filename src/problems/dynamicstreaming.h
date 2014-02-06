#pragma once

#include "problems/problem.h"

const size_t kParticleCountX = 1000;
const size_t kParticleCountY = 160;
const size_t kParticleCount = (kParticleCountX * kParticleCountY);
const size_t kVertsPerParticle = 6;
const size_t kVertexCount = kParticleCount * kVertsPerParticle;
const size_t kParticleBufferSize = sizeof(Vec2)* kVertexCount;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingProblem : public Problem
{
public:
    DynamicStreamingProblem();

    virtual bool Init() override;
    virtual void Render() override;
    virtual void Shutdown() override;
    inline virtual std::string GetName() override { return "DynamicStreaming"; }

protected:
    void Update();

    std::vector<Vec2> mVertexData;
    unsigned int mIteration;
};
