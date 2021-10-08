cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
}

struct VertexIn
{
	float3 posL : POSITION
	float4 color : COLOR
};

struct PixelIn
{
	float4 posH : SV_POSITION
	float4 color : COLOR
};

PixelIn VS(VertexIn vIn)
{
	PixelIn vOut;
	vOut.posH = mul(float4(vIn.posL,1.0f),gWorldViewProj);
	vOut.color = vIn.color;
	return vOut;
}