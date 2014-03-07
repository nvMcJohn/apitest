// Constant Buffers ---------------------------------------------------------------------------------------------------

// Textures / Samplers ------------------------------------------------------------------------------------------------
Texture2D g_Texture;
SamplerState g_Sampler;

// Input --------------------------------------------------------------------------------------------------------------
struct PsInput
{ 
    float4 f4Position : SV_Position;
    float2 f2TexCoord : TexCoord;
};

// Output -------------------------------------------------------------------------------------------------------------
struct PsOutput
{
    float4 f4Color : SV_Target0;
};

// Functions ----------------------------------------------------------------------------------------------------------
PsOutput psMain(PsInput I)
{
    PsOutput O = (PsOutput)0;
    O.f4Color = g_Texture.Sample(g_Sampler, I.f2TexCoord);
    return O;
}
