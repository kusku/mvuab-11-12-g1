//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "functions.fx"

//////////////////////////////////

////////////////////////////////////////////////////////////

//Blend Map

sampler2D BlendTextureMap : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Point;
   AddressU  = Wrap;
   AddressV  = Wrap;
};


////////////////////////////////////////////////////////////

//Tiled Textures

sampler2D Text1S  : register( s1 ) = sampler_state
{
	MinFilter		= Anisotropic;
	MagFilter		= LINEAR;
	MipFilter		= LINEAR;
	MaxAnisotropy	= 8;
	AddressU		= WRAP;
    AddressV		= WRAP;
};

sampler2D Text2S  : register( s2 ) = sampler_state
{
	MinFilter		= Anisotropic;
	MagFilter		= LINEAR;
	MipFilter		= LINEAR;
	MaxAnisotropy	= 8;
	AddressU		= WRAP;
    AddressV		= WRAP;
};

sampler2D Text3S  : register( s3 ) = sampler_state
{
	MinFilter		= Anisotropic;
	MagFilter		= LINEAR;
	MipFilter		= LINEAR;
	MaxAnisotropy	= 8;
	AddressU		= WRAP;
    AddressV		= WRAP;
};

sampler2D Text4S  : register( s4 ) = sampler_state
{
	MinFilter		= Anisotropic;
	MagFilter		= LINEAR;
	MipFilter		= LINEAR;
	MaxAnisotropy	= 8;
	AddressU		= WRAP;
    AddressV		= WRAP;
};

//////////////////////////////////////////////////////////////////////

struct VertexShaderInput
{
    float3 Position			: POSITION0;
    float2 TexCoord			: TEXCOORD0;
    float2 TiledTexCoord	: TEXCOORD1;
	float3 Normal			: NORMAL0;
};

struct VertexShaderOutput
{
    float4 Position         : POSITION0;
	float2 TexCoord         : TEXCOORD0;
    float2 TiledTexCoord	: TEXCOORD1;
	float3 Normal			: NORMAL0;
	float3 EyePosition      : NORMAL1;
	float4 WPos				: NORMAL2;
	float FogLerp			: NORMAL3;
};

//////////////////////////////////////////////////////////////////////

float3 TileColor(float2 TileTexCoord, float2 NonTileTexCoord)
{
    float3 c0 = tex2D(Text1S, TileTexCoord).rgb;
    float3 c1 = tex2D(Text2S, TileTexCoord).rgb;
    float3 c2 = tex2D(Text3S, TileTexCoord).rgb;
    float3 c3 = tex2D(Text4S, TileTexCoord).rgb;
    
    float4 BlendPer = tex2D(BlendTextureMap, NonTileTexCoord);

	float totalInverse = 1.0f / (BlendPer.r + BlendPer.g + BlendPer.b + BlendPer.a);
    
    c0 *= (BlendPer.r * totalInverse);
    c1 *= (BlendPer.g * totalInverse);
    c2 *= (BlendPer.b * totalInverse);
    c3 *= (BlendPer.a * totalInverse);
    
    float3 final = (c0 + c1 + c2 + c3);

	return final;
}

//////////////////////////////////////////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), World);	

	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord = input.TexCoord;

	output.TiledTexCoord = input.TiledTexCoord;

	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
	output.Normal = mul(input.Normal, World);

	[flatten]
	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, output.EyePosition) - FogStart) / FogRange);
	}

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input, uniform bool shadow) : COLOR
{	
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
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler1, i);
				}
				else if(i == 1)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler2, i);
				}
				else if(i == 2)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler3, i);
				}
				else
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler4, i);
				}
			}
			
			if(lightShadowDynamicEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler1, i);
				}
				else if(i == 1)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler2, i);
				}
				else if(i == 2)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler3, i);
				}
				else
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler4, i);
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

	float3 tileC = TileColor(input.TiledTexCoord, input.TexCoord);
	
	float4 PixEndColor = (float4)0;
	
	PixEndColor.rgb = (DiffuseColor + AmbientColor).rgb * tileC;
	
	[flatten]
	if(FogEnable == true)
	{
		PixEndColor.xyz = lerp(PixEndColor.xyz, FogColor, input.FogLerp);
	}

	PixEndColor.a = 1;
	
	return PixEndColor;
}

technique TerrainEffectTechnique
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false);
	}
}

technique TerrainEffectTechniqueShadow
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true);
	}
}