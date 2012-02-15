#include "Samplers.h"
#include "functions.fx"

struct VertexShaderInput
{
    float3 Position : POSITION0;
    float2 UV : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float2 UV : TEXCOORD0;
};

float g_SSAAWeight=0.8;
float g_RenderTargetSize : RENDER_TARGET_SIZE;

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	output.UV = input.UV;
	output.Position = float4(input.Position, 1);
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR
{
	const float2 delta[8] =
	{
		float2(-1,1),float2(1,-1),float2(-1,1),float2(1,1),
		float2(-1,0),float2(1,0),float2(0,-1),float2(0,1)
	};
	float3 tex = Texture2Normal(tex2D(S1LinearClampSampler,input.UV).xyz);
	float factor = 0.0f;
	
	for( int i=0;i<4;++i )
	{
		float3 t = Texture2Normal(tex2D(S1LinearClampSampler, input.UV+ delta[i] * 1/g_RenderTargetSize).xyz);
		t -= tex;
		factor += dot(t,t);
	}
	factor = min(1.0,factor)*g_SSAAWeight;
	
	float4 color = float4(0.0,0.0,0.0,0.0);
	float4 l_AlbedoColor=tex2D(S0LinearClampSampler,input.UV);
	for( int i=0;i<8;++i )
	{
		color += tex2D(S0LinearClampSampler,input.UV + delta[i]*(1/g_RenderTargetSize))*factor+(1-factor)*l_AlbedoColor;
	}
	color += 2.0*l_AlbedoColor;
	color = color*(1.0/10);
	
	return color;
}

technique SSAATechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}