#include "Samplers.h"

struct VertexInput
{
	float3 Position	: POSITION;
	float2 TexCoord	: TEXCOORD0;
};

struct VertexOutput
{
	float4 Position	: POSITION0;
	float2 TexCoord	: TEXCOORD0;
};


uniform float DyingAmount	: DYING_AMOUNT = 1.0f;

VertexOutput VertexShaderFunction(VertexInput input)
{
	VertexOutput output = (VertexOutput)0;

	output.Position = float4(input.Position, 1);
	output.TexCoord = input.TexCoord;
	
	return output;
}

float4 PixelShaderFunction(VertexOutput input) : COLOR0
{
	float4 PixEndColor = (float4)0;
	
	float3 colorText = tex2D(S0LinearWrapSampler, input.TexCoord).rgb;
	
	PixEndColor.rgb = colorText.rgb;

	if(DyingAmount > 0.0f)
	{	
		float deingColorMap = tex2D(S1LinearWrapSampler, input.TexCoord).r;

		if(deingColorMap == 1.0f)
		{
			float red = (PixEndColor.r * 2.5f) * DyingAmount;
			float green = (PixEndColor.g * 0.5f) * DyingAmount;
			float blue = (PixEndColor.b * 0.5f) * DyingAmount;

			PixEndColor.r += red;
			PixEndColor.g += green;
			PixEndColor.b += blue;
		}
		else
		{
			float3 grayScale = dot(colorText.rgb, float3(0.3, 0.59, 0.11));

			PixEndColor.rgb = lerp(colorText.rgb, grayScale.rgb, DyingAmount);
		}
	}

	PixEndColor.a = 1.0f;

	return PixEndColor;	
}

technique DyingEffectTechnique
{
	pass pass0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}