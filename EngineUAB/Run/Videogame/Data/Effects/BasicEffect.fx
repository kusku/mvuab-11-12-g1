
#include "functions.fx"


struct VertexShaderInput
{
    float3 Position : POSITION0;
    float4 VtColor	: NORMAL0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float4 VtColor	: NORMAL0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;

	float4x4 wvp = mul(World, mul(ViewMatrix, ProjectionMatrix));
	
	output.VtColor = input.VtColor;
	
	output.Position = mul(float4(input.Position, 1), wvp);
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	return input.VtColor;
}

technique BasicEffectTechnique
{
	pass p0
	{	
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}