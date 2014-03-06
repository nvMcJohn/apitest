
// Constant Buffers ---------------------------------------------------------------------------------------------------
cbuffer CB0
{
    float2 Viewport;
    float2 unused;
};

// Input --------------------------------------------------------------------------------------------------------------
struct VsInput
{
    float2 f2Position : Attr;
};

// Output -------------------------------------------------------------------------------------------------------------
struct VsOutput 
{
    float4 f4Position : SV_Position;
};

// Functions ----------------------------------------------------------------------------------------------------------
VsOutput vsMain(VsInput I)
{
    VsOutput O = (VsOutput)0;
    O.f4Position = float4(I.f2Position * Viewport + float2(-1, 1), 0, 1);
    return O;
}
