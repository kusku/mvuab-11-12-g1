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

sampler2D NormalTextureMap : register( s1 ) = sampler_state
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
	float4 Normal	: NORMAL0;
	float4 Tangent  : TANGENT0;
	float4 Binormal : BINORMAL0;
};

struct VertexShaderOutput
{
    float4 Position         : POSITION0;
	float2 TexCoord         : TEXCOORD0;
	float3 EyePosition      : TEXCOORD1;
	float3x3 TangentToWorld : TEXCOORD2;
	float4 WPos				: NORMAL0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), World);	

	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord = input.TexCoord;

	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
    // calculate tangent space to world space matrix using the world space tangent,
    // binormal, and normal as basis vectors.  the pixel shader will normalize these
    // in case the world matrix has scaling.
    output.TangentToWorld[0] = mul(input.Tangent.xyz, World);
    output.TangentToWorld[1] = mul(input.Binormal.xyz, World);
    output.TangentToWorld[2] = mul(input.Normal.xyz, World);

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR
{
	float4 TexColor = tex2D(DiffuseTextureMap, input.TexCoord);

	input.EyePosition = normalize(input.EyePosition);

	// Get the Color of the normal. The color describes the direction of the normal vector
	// and make it range from 0 to 1.
	float3 Normal = (2.0f * tex2D(NormalTextureMap, input.TexCoord) - 1.0f);
	Normal = mul(Normal, input.TangentToWorld);
	Normal = normalize(Normal);
	
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

technique ForwardLightingWithNormal
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}