cbuffer ModelViewProjCB : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
    matrix inverseView;
};

#if 1

struct GSOutput
{
	float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

static float3 QuadPos[4] =
{
    float3(-1, 1, 0),
    float3(1, 1, 0),
    float3(-1, -1, 0),
    float3(1, -1, 0),
};

static float2 QuadUV[4] =
{
    float2(0, 0),
    float2(1, 0),
    float2(0, 1),
    float2(1, 1),
};

static float QuadSize = 0.05;

[maxvertexcount(4)]
void main(
	point float4 input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
    for (uint i = 0; i < 4; i++)
    {
        float3 pos = QuadPos[i] * QuadSize;
        pos = mul(pos, (float3x3) inverseView) + input[0].xyz;
		
        GSOutput v;
        v.pos = float4(pos, 1.0);
        v.pos = mul(v.pos, view);
        v.pos = mul(v.pos, projection);
		
        v.tex = QuadUV[i];

        output.Append(v);
    }
}

#else

struct GSOutput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
    uint PrimID : SV_PrimitiveID;
};

static const float2 TexCoord[4] =
{
    float2(0.0f, 1.0f),
    float2(0.0f, 0.0f),
    float2(1.0f, 1.0f),
    float2(1.0f, 0.0f)
};

static const float3 EyePosW = float3(0.0f, 0.0f, 3.5f);
static const float QuadSize = 1.;

[maxvertexcount(4)]
void main(
	point float4 input[1] : SV_POSITION,
    uint primID : SV_PrimitiveID,
	inout TriangleStream<GSOutput> output
)
{
    // Compute the local coordinate system of the sprite relative to the
    // world space such that the billboard is aligned with the y-axis
    // and faces the eye.
    //
    float3 up = float3(0.0f, 1.0f, 0.0f);
    float3 look = EyePosW - input[0].xyz;
    look.y = 0.0f; // y-axis aligned, so project to xz-plane
    look = -normalize(look);
    float3 right = cross(up, look);
    
    //
    // Compute triangle strip vertices (quad) in world space.
    //
    float halfWidth = 0.5f * QuadSize;
    float halfHeight = 0.5f * QuadSize;
    float4 v[4];
    v[0] = float4(input[0].xyz + halfWidth * right - halfHeight * up, 1.0f);
    v[1] = float4(input[0].xyz + halfWidth * right + halfHeight * up, 1.0f);
    v[2] = float4(input[0].xyz - halfWidth * right - halfHeight * up, 1.0f);
    v[3] = float4(input[0].xyz - halfWidth * right + halfHeight * up, 1.0f);
    
    //
    // Transform quad vertices to world space and output
    // them as a triangle strip.
    for (int i = 0; i < 4; ++i)
    {
        GSOutput gsOut;
        gsOut.PosH = mul(v[i], view);
        gsOut.PosH = mul(gsOut.PosH, projection);
        gsOut.PosW = v[i].xyz;
        gsOut.NormalW = look;
        gsOut.Tex = TexCoord[i];
        gsOut.PrimID = primID;
        output.Append(gsOut);
    }
}

#endif