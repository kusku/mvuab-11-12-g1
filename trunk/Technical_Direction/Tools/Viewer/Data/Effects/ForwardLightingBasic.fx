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
> = 0.0;

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

float4 PixelShaderFunction(VertexShaderOutput input, uniform bool shadow, uniform bool vegetation, uniform float AlphaTestThreshold, uniform float AlphaTestDirection) : COLOR
{	
	float4 TexColor = tex2D(DiffuseTextureMap, input.TexCoord);

	input.EyePosition = normalize(input.EyePosition);

	float3 Normal = normalize(input.Normal);
	
	float4 AmbientColor = AmbientLightIntensity * AmbientLightColor;
	float4 DiffuseColor = (float4)0;

	for(int i = 0; i < numLights && i < MAX_LIGHTS; ++i)
	{	
		float4 passColor = (float4)0;
		float shadowCoeffStatic = 1.0f;
		float shadowCoeffDynamic = 1.0f;
		
		if(shadow == true)
		{
			if(lightShadowStaticEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffStatic = CalcShadowCoeffVSM(input.WPos, StaticShadowSamplers[0], i);
				}
				else if(i == 1)
				{
					shadowCoeffStatic = CalcShadowCoeffVSM(input.WPos, StaticShadowSamplers[1], i);
				}
				else if(i == 2)
				{
					shadowCoeffStatic = CalcShadowCoeffVSM(input.WPos, StaticShadowSamplers[2], i);
				}
				else
				{
					shadowCoeffStatic = CalcShadowCoeffVSM(input.WPos, StaticShadowSamplers[3], i);
				}
			}
			
			if(lightShadowDynamicEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffDynamic = CalcShadowCoeffVSM(input.WPos, DynamicShadowSamplers[0], i);
				}
				else if(i == 1)
				{
					shadowCoeffDynamic = CalcShadowCoeffVSM(input.WPos, DynamicShadowSamplers[1], i);
				}
				else if(i == 2)
				{
					shadowCoeffDynamic = CalcShadowCoeffVSM(input.WPos, DynamicShadowSamplers[2], i);
				}
				else
				{
					shadowCoeffDynamic = CalcShadowCoeffVSM(input.WPos, DynamicShadowSamplers[3], i);
				}
			}
		}
		
		if(lightType[i] == OMNI)
		{
			passColor = CalculateOmniLight(Normal, input.WPos, i);
		}
		else if(lightType[i] == DIRECTIONAL)
		{
			passColor = CalculateDirectionLight(Normal, input.WPos, i);
		}
		else if(lightType[i] == SPOT)
		{
			passColor = CalculateSpotLight(Normal, input.WPos, i);
		}
		
		if(shadow == true)
		{
			passColor *= min(shadowCoeffStatic, shadowCoeffDynamic);
		}
		
		DiffuseColor += passColor;
	}
	
	float4 PixEndColor = (DiffuseColor + AmbientColor) * TexColor;

	PixEndColor.a = TexColor.a;
	
	if(vegetation == true)
	{
		clip((PixEndColor.a - AlphaTestThreshold) * AlphaTestDirection);
	}
	
	return PixEndColor;
}

technique ForwardLightingBasic
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, false, 0.0f, 0.0f);
	}
}

technique ForwardLightingBasicShadow
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, false, 0.0f, 0.0f);
	}
}

technique ForwardLightingBasicVegetation1
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true, 0.95f, 1.0f);
	}
}

technique ForwardLightingBasicVegetation2
{
	pass p0
	{
		
		CullMode			= None;
		ZWriteEnable		= false;
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true, 0.95f, -1.0f);
	}
}

technique ForwardLightingBasicShadowVegetation1
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true, 0.95f, 1.0f);
	}
}

technique ForwardLightingBasicShadowVegetation2
{
	pass p0
	{
		
		CullMode			= None;
		ZWriteEnable		= false;
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;
		
		//CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true, 0.95f, -1.0f);
	}
}