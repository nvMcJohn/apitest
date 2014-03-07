// Constant Buffers ---------------------------------------------------------------------------------------------------
cbuffer CB0
{
    float4x4 ViewProjection;
};

cbuffer CB1
{
    float4x4 World;
};

// Input --------------------------------------------------------------------------------------------------------------
struct VsInput
{
    float3 f3Position : ObjPos;
    float2 f2TexCoord : TexCoord;
};

// Output -------------------------------------------------------------------------------------------------------------
struct VsOutput 
{
    float4 f4Position : SV_Position;
    float2 f2TexCoord : TexCoord;
};

// Functions ----------------------------------------------------------------------------------------------------------
VsOutput vsMain(VsInput I)
{
    VsOutput O = (VsOutput) 0;
    O.f4Position = mul(mul( float4(I.f3Position, 1.0f), World), ViewProjection);
    O.f2TexCoord = I.f2TexCoord;

    return O;
}
