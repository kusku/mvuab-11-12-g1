
float4x4 WorldViewProjection : WORLDVIEWPROJECTION;

struct VertexShaderInput
{
    float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct VertexShaderOutput
{
    float4 Position : POSITION;
	float2 Depth : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	// Compute the projected coordinates
	output.Position = mul( float4(input.Position, 1.0f), WorldViewProjection );
	
	// Store z and w in our spare texcoord
	output.Depth.xy = output.Position.zw;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR
{
	// Depth is z / w
	float4 RetPix = Depth.x / Depth.y;
	
	return RetPix;
}

technique ShadowMap 
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}