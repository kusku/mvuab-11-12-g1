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

float g_Time : TIME;

VertexOut NoiseVS(VertexIn IN)
{
	VertexOut l_OUT = (VertexOut)0;
	l_OUT.Pos = float4(IN.Pos.xyz, 1);
	l_OUT.UV = IN.UV;
	
	return l_OUT;
}

float4 NoisePS(VertexOut IN) : COLOR
{
	float2 l_Offset = float2(cos(g_Time),sin(g_Time));
	float2 l_UV = IN.UV + l_Offset;
	float4 l_NoiseColor = tex2D(S0LinearWrapSampler, l_UV);
	
	return float4(l_NoiseColor.xyz, (1-l_NoiseColor.r));
}

technique NoiseTechnique
{
	pass p0
	{
		CullMode = CCW;
		AlphaBlendEnable = true;
		BlendOp = add;
		SRCBLEND		= SrcAlpha;
		DESTBLEND			= InvSrcAlpha;
		
		VertexShader = compile vs_3_0 NoiseVS();
		PixelShader = compile ps_3_0 NoisePS();
	}
}