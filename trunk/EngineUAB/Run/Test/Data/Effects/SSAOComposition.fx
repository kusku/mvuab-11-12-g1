#include "Samplers.h"

struct VertexIn
{
	float3 Pos : POSITION;
	float2 UV : TEXCOORD0;
};

struct VertexOut
{
	float4 Pos : POSITION0;
	float2 UV : TEXCOORD0;
};

VertexOut SSAOVS (VertexIn IN)
{
	VertexOut l_OUT = (VertexOut)0;
	l_OUT.Pos = float4(IN.Pos.xyz, 1);
	l_OUT.UV = IN.UV;
	
	return l_OUT;
}

float4 SSAOFinalCompositionPS(VertexOut IN) : COLOR
{
	//return float4(tex2D(S1LinearWrapSampler,IN.UV).xyz,1.0);
	//return float4(tex2D(S0LinearWrapSampler,IN.UV).xyz, 1.0);
	return float4(tex2D(S0LinearWrapSampler,IN.UV).xyz *tex2D(S1LinearWrapSampler,IN.UV).r,0.0);
}

//Technique que renderiza el SSAO con la imagen completa
technique SSAOFinalCompositionTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 SSAOVS();
		PixelShader = compile ps_3_0 SSAOFinalCompositionPS();
	}
}