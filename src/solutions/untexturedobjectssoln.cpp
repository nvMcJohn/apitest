#include "pch.h"

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsSolution::UntexturedObjectsSolution()
: mObjectCount()
, mIndexCount()
{ }

// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsSolution::~UntexturedObjectsSolution()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsSolution::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                     const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                     size_t _objectCount)
{
    mObjectCount = _objectCount;
    mIndexCount = _indices.size();
    
    return true;
}

