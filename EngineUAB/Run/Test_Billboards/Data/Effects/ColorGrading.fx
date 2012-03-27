#include "Samplers.h"

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

/*float g_MonochromeColorGrading = 0.0;
float g_BrownSepiaColorGrading = 0.5;
float g_GreenSepiaColorGrading = 0.2;
float g_BlueSepiaColorGrading = 0.3;
float g_ContrastColorGrading = 1.0;
float g_BrightnessColorGrading = 0.0;
float g_ColorColorGrading = 1.0;*/

float g_MonochromeColorGrading : PARAMETER0;
float g_BrownSepiaColorGrading : PARAMETER1;
float g_GreenSepiaColorGrading : PARAMETER2;
float g_BlueSepiaColorGrading : PARAMETER3;
float g_ContrastColorGrading : PARAMETER4;
float g_BrightnessColorGrading : PARAMETER5;
float g_ColorColorGrading : PARAMETER6;

float4 ColorGrading(float4 _Color)
{
	//-------Color Matrices for Color Correction--------------
	float4x4 gray = {0.299,0.587,0.184,0,
		0.299,0.587,0.184,0,
		0.299,0.587,0.184,0,
		0,0,0,1};
	
	float4x4 sepia = {0.299,0.587,0.184,0.1,
		0.299,0.587,0.184,0.018,
		0.299,0.587,0.184,-0.090,
		0,0,0,1};
	
	float4x4 sepia2 = {0.299,0.587,0.184,-0.090,
		0.299,0.587,0.184,0.018,
		0.299,0.587,0.184,0.1,
		0,0,0,1};
	
	float4x4 sepia3 = {0.299,0.587,0.184,-0.090,
		0.299,0.587,0.184,0.1,
		0.299,0.587,0.184,0.1,
		0,0,0,1};

	float4x4 sepia4 = {0.299,0.587,0.184,-0.090,
		0.299,0.587,0.184,0.018,
		0.1299,0.587,0.184,0.1,
		0,0,0,1};
		
	float3 monochrome = (_Color.r * 0.3f + _Color.g * 0.59f + _Color.b * 0.11f);
	float4 monochrome4 = float4(monochrome,1);
	float4 result2 = _Color;
	float4 brownsepia = mul(sepia,result2) ;
	float4 greensepia = mul(sepia3,result2) ;
	float4 bluesepia = mul(sepia2,result2) ;
	float4 combine = (brownsepia *g_BrownSepiaColorGrading )
		+(greensepia*g_BrownSepiaColorGrading )
		+(bluesepia * g_BlueSepiaColorGrading )
		+(greensepia * g_GreenSepiaColorGrading )
		+(monochrome4* g_MonochromeColorGrading)
		+(g_ColorColorGrading * result2);
	
	return (combine * g_ContrastColorGrading) + g_BrightnessColorGrading;
}

VertexShaderOutput ColorGradingVS(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	output.UV = input.UV;
	output.Position = float4(input.Position, 1);
	
	return output;
}

float4 ColorGradingPS(VertexShaderOutput input) : COLOR
{
	float4 l_Color= tex2D(S0LinearClampSampler, input.UV);
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return ColorGrading(l_Color);
}

technique ColorGradingTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 ColorGradingVS();
		PixelShader = compile ps_3_0 ColorGradingPS();
	}
}