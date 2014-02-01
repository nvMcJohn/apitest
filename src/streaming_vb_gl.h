#pragma once

#include "gfx_gl.h"

class StreamingVB_GL : public StreamingVB
{
public:
    StreamingVB_GL();
    virtual ~StreamingVB_GL();

    virtual bool Init() override;

    virtual bool Begin(GfxBaseApi* _activeAPI) override;

    virtual void Draw(VertexPos2* vertices, int count) override;

private:
    struct Constants
    {
        float width;
        float height;
        float pad[2];
    };

    GLuint m_ub;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_dyn_vb;
    int m_dyn_offset;
    void* m_dyn_vb_ptr;
};
