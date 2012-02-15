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

float g_ZBlurFocalStart : PARAMETER0;
float g_ZBlurFocalEnd : PARAMETER1;
float g_ZBlurEnd : PARAMETER2;
float g_ConstantBlur : PARAMETER3;

float4x4 g_InverseProjectionMatrix : PROJECTIONINVERSE;
float4x4 g_InverseViewMatrix : VIEWINVERSE;
float g_RenderTargetSize : RENDER_TARGET_SIZE;

VertexShaderOutput ZBlurVS(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	output.UV = input.UV;
	output.Position = float4(input.Position, 1);
	
	return output;
}

float4 ZBlurPS(VertexShaderOutput input) : COLOR
{
	float4 l_DepthMap=tex2D(S1LinearClampSampler,input.UV);
	float3 l_CameraPosition=g_InverseViewMatrix[3].xyz;
	float3 l_WorldPosition=GetPositionFromZDepthView(l_DepthMap, float2(0,0), g_InverseViewMatrix, g_InverseProjectionMatrix);
	float l_Distance=length(l_WorldPosition-l_CameraPosition);
	float4 l_Color=float4(0,0,0,0);
	float l_Blur=1.0;
	if(l_Distance<g_ZBlurFocalStart)
	{
		l_Blur=max(l_Distance/g_ZBlurFocalStart, g_ConstantBlur);
		}
	else if(l_Distance>g_ZBlurFocalEnd)
	{
		l_Blur=max(1.0-(l_Distance-g_ZBlurFocalEnd)/g_ZBlurEnd, g_ConstantBlur);
	}
	//return float4(l_Blur,l_Blur,l_Blur,1.0);
	const float2 delta[8] =
	{
		float2(-1,1),float2(1,-1),float2(-1,1),float2(1,1),
		float2(-1,0),float2(1,0),float2(0,-1),float2(0,1)
	};
	
	float2 l_PixelInc=4*1/g_RenderTargetSize; //4 pixeles a la redonda
	float4 l_AlbedoColor=tex2D(S0LinearClampSampler,input.UV);
	for( int i=0;i<8;i++ )
	{
		l_Color += tex2D(S0LinearClampSampler,input.UV + delta[i]*l_PixelInc)*(1-l_Blur)+l_Blur*l_AlbedoColor;
	}
	l_Color = l_Color*(1.0/8.0);
	
	return l_Color;
}

technique ZBlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 ZBlurVS();
		PixelShader = compile ps_3_0 ZBlurPS();
	}
}