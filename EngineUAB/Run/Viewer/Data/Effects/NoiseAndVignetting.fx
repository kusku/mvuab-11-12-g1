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

float g_Time : PARAMETER0;

VertexOut RenderNoiseAndVignettingVS(VertexIn IN)
{
	VertexOut l_OUT = (VertexOut)0;
	l_OUT.Pos = float4(IN.Pos.xyz, 1);
	l_OUT.UV = IN.UV;
	
	return l_OUT;
}

float4 RenderNoiseAndVignettingPS(VertexOut IN) : COLOR
{
	float2 l_Offset = float2(cos(g_Time),sin(g_Time));
	float2 l_UV = IN.UV + l_Offset;
	float4 l_VignettingColor = tex2D(S0LinearClampSampler, IN.UV);
	float4 l_NoiseColor = tex2D(S1LinearWrapSampler, l_UV);
	
	return l_NoiseColor * l_VignettingColor;
	//return float4(l_NoiseColor.xyz*l_VignettingColor.xyz, l_NoiseColor.a + l_VignettingColor.a);
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}

technique RenderNoiseAndVignettingTechnique
{
	pass p0
	{
		CullMode = CCW;
		AlphaBlendEnable = true;
		BlendOp = add;
		SRCBLEND		= SrcAlpha;
		DESTBLEND			= InvSrcAlpha;
		
		VertexShader = compile vs_3_0 RenderNoiseAndVignettingVS();
		PixelShader = compile ps_3_0 RenderNoiseAndVignettingPS();
	}
}