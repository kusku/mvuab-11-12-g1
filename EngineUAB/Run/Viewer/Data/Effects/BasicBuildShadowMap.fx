#include "functions.fx"

struct VertexShaderInput
{
    float3 Position : POSITION0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	//float3 PosView	: NORMAL0;
	float2 PosView2	: NORMAL1;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
    
    output.Position = mul(float4(input.Position, 1), ShadowWorldViewProjection);
    //output.PosView  = mul(float4(input.Position, 1), ShadowWorldView);
	output.PosView2	= output.Position.zw;
	
    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{	
    //float Depth = RescaleDistToLight(length(input.PosView));
	float Depth = input.PosView2.x / input.PosView2.y;
	
    float2 Moments = ComputeMoments(Depth) - GetFPBias();
    
	float4 ret = (float4)0;
	
	ret.r = Moments.x;
	ret.g = Moments.y;
	
	return ret;
}

technique BasicBuildShadowMap 
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}