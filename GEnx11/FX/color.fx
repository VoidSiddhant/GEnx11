
cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
}

struct VertexIn
{
	float3 posL : POSITION;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color :COLOR;
};

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.posH = mul(float4(vIn.posL,1.0f),gWorldViewProj);
	vOut.color = vIn.color;
	return vOut;
}

float4 PS(VertexOut vOut) :SV_TARGET
{
	return vOut.color;
}
 
RasterizerState WireFrameState
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
	MultisampleEnable = true;
};

technique11 ColorTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0,VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0,PS()));
	}
}

technique11 ColorTechWireFrame
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
		SetRasterizerState(WireFrameState);
	}
};
