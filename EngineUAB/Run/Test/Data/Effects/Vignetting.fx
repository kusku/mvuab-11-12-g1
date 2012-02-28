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

VertexOut RenderNoiseAndVignettingVS(VertexIn IN)
{
	VertexOut l_OUT = (VertexOut)0;
	l_OUT.Pos = float4(IN.Pos.xyz, 1);
	l_OUT.UV = IN.UV;
	
	return l_OUT;
}

float4 RenderNoiseAndVignettingPS(VertexOut IN) : COLOR
{
	float4 l_VignettingColor = tex2D(S0LinearWrapSampler, IN.UV);
	return l_VignettingColor;	
}

technique VignettingTechnique
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