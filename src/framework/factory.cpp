#include "pch.h"
#include "factory.h"

#include "problems/dynamicstreaming.h"
#include "problems/null.h"
#include "problems/texturedquads.h"
#include "problems/untexturedobjects.h"

#include "solutions/dynamicstreaming/gl/buffersubdata.h"
#include "solutions/dynamicstreaming/gl/mappersistent.h"
#include "solutions/dynamicstreaming/gl/mapunsynchronized.h"

#include "solutions/nullsoln.h"
#include "solutions/untexturedobjects/gl/bindless.h"
#include "solutions/untexturedobjects/gl/bindlessindirect.h"
#include "solutions/untexturedobjects/gl/bufferrange.h"
#include "solutions/untexturedobjects/gl/bufferstorage.h"
#include "solutions/untexturedobjects/gl/dynamicbuffer.h"
#include "solutions/untexturedobjects/gl/multidraw.h"
#include "solutions/untexturedobjects/gl/texcoord.h"
#include "solutions/untexturedobjects/gl/uniform.h"

#include "solutions/texturedquads/gl/bindless.h"
#include "solutions/texturedquads/gl/bindlessmultidraw.h"
#include "solutions/texturedquads/gl/naive.h"
#include "solutions/texturedquads/gl/notex.h"
#include "solutions/texturedquads/gl/sparsebindlesstexturearray.h"
#include "solutions/texturedquads/gl/sparsebindlesstexturearraymultidraw.h"
#include "solutions/texturedquads/gl/texturearray.h"
#include "solutions/texturedquads/gl/texturearraymultidraw.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
ProblemFactory::ProblemFactory(bool _skipInit)
{
    Problem* newProb = nullptr;
    // Null
    newProb = new NullProblem();
    if (_skipInit || newProb->Init()) {
        if (!_skipInit) {
            newProb->Shutdown();
        }
        mProblems.push_back(newProb);
        mSolutions[mProblems.back()->GetName()].push_back(new NullSolution());
    } else {
        newProb->Shutdown();
        SafeDelete(newProb);
        console::error("Unable to create the Null Problem--exiting.");
    }

    // DynamicStreaming
    newProb = new DynamicStreamingProblem();
    if (_skipInit || newProb->Init()) {
        if (!_skipInit) {
            newProb->Shutdown();
        }
        mProblems.push_back(newProb);
        mSolutions[mProblems.back()->GetName()].push_back(new DynamicStreamingGLBufferSubData());
        mSolutions[mProblems.back()->GetName()].push_back(new DynamicStreamingGLMapUnsynchronized());
        mSolutions[mProblems.back()->GetName()].push_back(new DynamicStreamingGLMapPersistent());
    } else {
        newProb->Shutdown();
        SafeDelete(newProb);
    }

    // UntexturedObjects
    newProb = new UntexturedObjectsProblem();
    if (_skipInit || newProb->Init()) {
        if (!_skipInit) {
            newProb->Shutdown();
        }
        mProblems.push_back(newProb);
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLUniform());
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLMultiDraw(true));
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLMultiDraw(false));
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLBindless());
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLBindlessIndirect());
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLBufferRange());
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLBufferStorage(true));
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLBufferStorage(false));
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLDynamicBuffer());
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLTexCoord());
    } else {
        newProb->Shutdown();
        SafeDelete(newProb);
    }

    // Textured Quads
    newProb = new TexturedQuadsProblem();
    if (_skipInit || newProb->Init()) {
        if (!_skipInit) {
            newProb->Shutdown();
        }
        mProblems.push_back(newProb);
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLBindless());
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLBindlessMultiDraw());
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLNaive());
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLNoTex());
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLSparseBindlessTextureArray());
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLSparseBindlessTextureArrayMultiDraw());
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLTextureArray());
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLTextureArrayMultiDraw(true));
        mSolutions[mProblems.back()->GetName()].push_back(new TexturedQuadsGLTextureArrayMultiDraw(false));
    } else {
        newProb->Shutdown();
        SafeDelete(newProb);
    }
}

// --------------------------------------------------------------------------------------------------------------------
ProblemFactory::~ProblemFactory()
{
    for (auto probIt = mSolutions.begin(); probIt != mSolutions.end(); ++probIt) {
        for (auto solIt = probIt->second.begin(); solIt != probIt->second.end(); ++solIt) {
            SafeDelete(*solIt);
        }
    }

    for (auto probIt = mProblems.begin(); probIt != mProblems.end(); ++probIt) {
        SafeDelete(*probIt);
    }
}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Problem*> ProblemFactory::GetProblems()
{
    return mProblems;
}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Solution*> ProblemFactory::GetSolutions(Problem* _problem)
{
    assert(_problem);
    return mSolutions[_problem->GetName()];
}
