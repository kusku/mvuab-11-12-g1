//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "functions.fx"

//////////////////////////////////////

//////////////////////////////////////
//Textures						   //
/////////////////////////////////////

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
	float2 TexCoord	: TexCoord0;
};

struct VertexShaderOutput
{
	float4 Position : POSITION0;
	float2 TexCoord	: TexCoord0;
};
//////////////////////////////////////


//////////////////////////////////////
//VS & PS Functions				   //
/////////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	//Basic Info
	output.Position = float4(input.Position, 1);
	output.TexCoord = input.TexCoord - HalfPixel;
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	//Get Normal From Map and unpack
	float3 normal = tex2D(NormalTextureMap, input.TexCoord).xyz;
	normal = normalize(UnpackNormal(normal));

    //Get Depth from Map
    float depthVal = tex2D(DepthTextureMap, input.TexCoord).r;

    //compute screen-space position
	float4 position = GetPositionFromDepth(input.TexCoord, depthVal);
	
	float4 FinalPixelColor = (float4)0;
	
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
	
	FinalPixelColor = saturate(FinalPixelColor);
	
	return FinalPixelColor;
}

//////////////////////////////////////


//////////////////////////////////////
//Techniques					   //
/////////////////////////////////////

technique BasicDeferredLighting
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