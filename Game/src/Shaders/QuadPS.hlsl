Texture2D Texture : register(t0);
SamplerState SamplerTex : register(s0);

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 texel = Texture.Sample(SamplerTex, input.tex);
    
    //if (texel.a < 0.1)
    //    return float4(.0f, .0f, 1.0f, 1.0f);
    
    return texel;
}