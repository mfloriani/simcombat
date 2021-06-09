cbuffer ModelViewProjCB : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    matrix inverseView;
};


float4 main(float3 inpos : POSITION) : SV_POSITION
{
    float4 pos = float4(inpos, 1.0);
    pos = mul(pos, model);
    return pos;
}