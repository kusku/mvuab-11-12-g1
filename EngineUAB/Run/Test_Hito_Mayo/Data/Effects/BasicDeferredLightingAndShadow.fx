//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "functions.fx"


//////////////////////////////////////
//Textures						   //
/////////////////////////////////////

sampler2D DynamicShadowMapSampler = sampler_state
{
   Texture		= < DynamicShadowMap >;
   MinFilter	= POINT;
   MagFilter	= POINT;
   MipFilter	= NONE;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
};

sampler2D StaticShadowMapSampler = sampler_state
{
   Texture		= < StaticShadowMap >;
   MinFilter	= POINT;
   MagFilter	= POINT;
   MipFilter	= NONE;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
};

sampler2D NormalTextureMap : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D DepthTextureMap : register( s1 ) = sampler_state
{
	MagFilter = POINT;
	MinFilter = POINT;
	Mipfilter = POINT;
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

//////////////////////////////////////


//////////////////////////////////////
//Structs						   //
/////////////////////////////////////

struct VertexShaderInput
{
	float3 Position : POSITION0;
	float2 TexCoord	: TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 Position : POSITION0;
	float2 TexCoord	: TEXCOORD0;
};

//////////////////////////////////////

//////////////////////////////////////
//VS & PS Functions				   //
/////////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	//Basic Info
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - HalfPixel;
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	float4 FinalPixelColor = (float4)0;
	
	//Get Depth from Map
	float depthVal = tex2D(DepthTextureMap, input.TexCoord);
	
	if(depthVal != 1)
	{
		//compute screen-space position
		float4 position = GetPositionFromDepth(input.TexCoord, depthVal);
		
		//Get Normal From Map and unpack
		float3 normal = tex2D(NormalTextureMap, input.TexCoord).rgb;
		normal = normalize(UnpackNormal(normal));
		
		float shadowCoeffStatic = 1.0f;
		float shadowCoeffDynamic = 1.0f;
		
		if(HasStaticShadowMap == true)
		{
			//shadowCoeffStatic = CalculateShadowCoeff(position, StaticShadowMapSampler);
			shadowCoeffStatic = CalcShadowCoeffVSM(position, StaticShadowMapSampler);
		}
		
		if(HasDynamicShadowMap == true)
		{
			//shadowCoeffDynamic = CalculateShadowCoeff(position, DynamicShadowMapSampler);
			shadowCoeffDynamic = CalcShadowCoeffVSM(position, DynamicShadowMapSampler);
		}
		
		
		if(shadowCoeffDynamic != 0 || shadowCoeffStatic != 0)
		{
			if(lightType[0] == OMNI)
			{
				FinalPixelColor = CalculateOmniLight(normal, position, 0);
			}
			else if(lightType[0] == DIRECTIONAL)
			{
				FinalPixelColor = CalculateDirectionLight(normal, position, 0);
			}
			else if(lightType[0] == SPOT)
			{
				FinalPixelColor = CalculateSpotLight(normal, position, 0);
			}
		}
		
		FinalPixelColor = saturate(FinalPixelColor * min(shadowCoeffStatic, shadowCoeffDynamic));
		//FinalPixelColor = saturate(FinalPixelColor * shadowCoeffDynamic);
	}
	else
	{
		FinalPixelColor = float4(1, 1, 1, 1);
	}
	
	return FinalPixelColor;
}

//////////////////////////////////////


//////////////////////////////////////
//Techniques					   //
/////////////////////////////////////

technique BasicDeferredLightingAndShadow
{
	pass p0
	{
		AlphaBlendEnable	= True;
		BlendOp				= Add;
		SrcBlend			= One;
		DestBlend			= One;
		CullMode			= CCW;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}