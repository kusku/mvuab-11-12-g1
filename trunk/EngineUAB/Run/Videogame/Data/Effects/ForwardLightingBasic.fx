//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "functions.fx"

//////////////////////////////////

float AmbientLightIntensity <
    string UIName =  "Ambient Light Intensity";
    string UIWidget = "slider";
    float UIMin = 0.0;
    float UIMax = 1.0;
    float UIStep = 0.1;
> = 0.15;

float4 AmbientLightColor <
    string UIName =  "Ambient Light Color";
    string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

//////////////////////////////////

sampler2D DiffuseTextureMap : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

struct VertexShaderInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
	float3 Normal	: NORMAL0;
};

struct VertexShaderOutput
{
    float4 Position         : POSITION0;
	float2 TexCoord         : TEXCOORD0;
	float3 EyePosition      : TEXCOORD1;
	float3 Normal			: NORMAL0;
	float4 WPos				: NORMAL1;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), World);	

	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord = input.TexCoord;

	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
	output.Normal = mul(input.Normal, World);

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR
{
	float4 TexColor = tex2D(DiffuseTextureMap, input.TexCoord);

	input.EyePosition = normalize(input.EyePosition);

	float3 Normal = normalize(input.Normal);
	
	float4 AmbientColor = AmbientLightIntensity * AmbientLightColor;
	float4 DiffuseColor = (float4)0;
	
	for(int i = 0; i < numLights && i < MAX_LIGHTS; ++i)
	{
		if(lightEnable[i] == true)
		{
			if(lightType[i] == OMNI)
			{
				DiffuseColor += CalculateOmniLight(Normal, input.WPos, i);
			}
			else if(lightType[i] == DIRECTIONAL)
			{
				DiffuseColor += CalculateDirectionLight(Normal, input.WPos, i);
			}
			else if(lightType[i] == SPOT)
			{
				DiffuseColor += CalculateSpotLight(Normal, input.WPos, i);
			}
		}
	}
	
	float4 PixEndColor = (DiffuseColor + AmbientColor) * TexColor;

	PixEndColor = saturate(PixEndColor);
	PixEndColor.a = 1.0f;

	//return DiffuseColor;
	return PixEndColor;
	//return float4(0, 1, 0, 1);
}

technique ForwardLightingBasic
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}