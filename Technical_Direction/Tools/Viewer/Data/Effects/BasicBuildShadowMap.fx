
float4x4 WorldViewProjection : SHADOW_WORLDVIEWPROJECTION;

struct VertexShaderInput
{
    float3 Position : POSITION0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float2 Depth : TEXCOORD0;
};

struct PixelShaderOutput
{
	float4 DepthRT	: COLOR0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	// Compute the projected coordinates
	output.Position = mul( float4(input.Position, 1.0f), WorldViewProjection );
	
	// Store z and w in our spare texcoord
	output.Depth.x = output.Position.z;
	output.Depth.y = output.Position.w;
	
	return output;
}

PixelShaderOutput PixelShaderFunction(VertexShaderOutput input)
{
	PixelShaderOutput output = (PixelShaderOutput)0;
	
	// Depth is z / w
	//output.DepthRT.r = input.Depth.x / input.Depth.y;
	
	//return output;
	
	float d = input.Depth.x / input.Depth.y;
	float moment1 = d;
	float moment2 = d * d;

	 // Adjusting moments (this is sort of bias per pixel) using partial derivative
	float dx = ddx(d);
	float dy = ddy(d);
	moment2 += 0.25 * (dx * dx + dy * dy) ;
    output.DepthRT = float4(moment1, moment2, 0, 1.0f);
	
	return output;
}

technique BasicBuildShadowMap 
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}