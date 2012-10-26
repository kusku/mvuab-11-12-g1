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

/*float g_ZBlurFocalStart=50;
float g_ZBlurFocalEnd=65;
float g_ZBlurEnd=300;
float g_ConstantBlur=0.4;*/

extern uniform float ZBlurFocalStart	: PARAMETER0 = 0;
extern uniform float ZBlurFocalEnd		: PARAMETER1 = 200;
extern uniform float ZBlurEnd			: PARAMETER2 = 300;
extern uniform float ConstantBlur		: PARAMETER3 = 0.4f;

uniform const float2 BlurDelta[8] =
{
	float2(-1,1),float2(1,-1),float2(-1,1),float2(1,1),
	float2(-1,0),float2(1,0),float2(0,-1),float2(0,1)
};

VertexShaderOutput ZBlurVS(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	output.UV = input.UV;
	output.Position = float4(input.Position, 1);
	
	return output;
}

float4 ZBlurPS(VertexShaderOutput input) : COLOR
{
	float depthVal = tex2D(S1LinearClampSampler, input.UV).r;

	float3 pos = GetPositionFromDepth(input.UV, depthVal);
	
	float dist = length(pos - CameraPosition);
	
	float4 PixEndColor = (float4)0;
	
	float blur = 1.0;
	
	if(dist < ZBlurFocalStart)
	{
		blur = max(dist / ZBlurFocalStart, ConstantBlur);
	}
	else if(dist > ZBlurFocalEnd)
	{
		blur = max(1.0 - (dist - ZBlurFocalEnd) / ZBlurEnd, ConstantBlur);
	}

	float2 pixelInc = 4*1 / RenderTargetSize.x; //4 pixeles a la redonda

	float4 albedoColor = tex2D(S0LinearClampSampler, input.UV);
	
	for( int i=0;i<8;i++ )
	{
		PixEndColor += tex2D(S0LinearClampSampler, input.UV + BlurDelta[i] * pixelInc) * ( 1 - blur) + blur * albedoColor;
	}

	PixEndColor = PixEndColor * (1.0/8.0);
	
	return PixEndColor;
}

technique ZBlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		
		VertexShader = compile vs_3_0 ZBlurVS();
		PixelShader = compile ps_3_0 ZBlurPS();
	}
}