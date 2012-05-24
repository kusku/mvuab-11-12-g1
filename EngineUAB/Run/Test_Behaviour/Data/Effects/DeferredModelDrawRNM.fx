//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "functions.fx"

//////////////////////////////////

//////////////////////////////////

sampler2D DiffuseTextureMap : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D RNMXTextureMap : register( s1 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D RNMYTextureMap : register( s2 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D RNMZTextureMap : register( s3 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D NormalTextureMap : register( s4 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

struct VertexShaderInput
{
    float3 Position		: POSITION0;
    float2 TexCoord		: TEXCOORD0;
    float2 TexCoord2	: TEXCOORD1;
	float4 Normal		: NORMAL0;
	float4 Tangent		: TANGENT0;
	float4 Binormal		: BINORMAL0;
};

struct VertexShaderOutput
{
    float4 Position 		: POSITION0;
    float2 TexCoord 		: TEXCOORD0;
    float2 TexCoord2		: TEXCOORD1;
	float2 Depth			: TEXCOORD2;
	float3x3 TangentToWorld : TEXCOORD3;
};

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
	float4 LightRT		: COLOR3;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	output.Position		= mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord		= input.TexCoord;
	output.TexCoord2	= input.TexCoord2;
	output.Depth.x		= output.Position.z; 
	output.Depth.y		= output.Position.w; 
	
    // calculate tangent space to world space matrix using the world space tangent,
    // binormal, and normal as basis vectors.  the pixel shader will normalize these
    // in case the world matrix has scaling.
    output.TangentToWorld[0] = mul(input.Tangent.xyz,    World);
    output.TangentToWorld[1] = mul(input.Binormal.xyz,    World);
    output.TangentToWorld[2] = mul(input.Normal.xyz,    World);
	
	return output;
}

PixelShaderOutput PixelShaderFunction(VertexShaderOutput input)
{
	PixelShaderOutput output = (PixelShaderOutput)0;
	
	// Get the Color of the normal. The color describes the direction of the normal vector
	// and make it range from 0 to 1.
	float4 NormalMapTex = tex2D(NormalTextureMap, input.TexCoord);
	float3 Normal = (2.0f * NormalMapTex.xyz - 1.0f);
	Normal = mul(Normal, input.TangentToWorld);
	Normal = normalize(Normal);
	//Normal = normalize(Normal.x*input.TangentToWorld[0] - Normal.y*input.TangentToWorld[1] + Normal.z*input.TangentToWorld[2]);

	//RNM
	float3 l_Lightmap = GetRadiosityNormalMap(Normal, input.TexCoord2, World, RNMXTextureMap, RNMYTextureMap, RNMZTextureMap);
	
	output.DiffuseRT	= tex2D(DiffuseTextureMap, input.TexCoord);
	output.DepthRT.r	= input.Depth.x / input.Depth.y;
	output.NormalRT.rgb	= PackNormal(normalize(Normal));
	output.LightRT = float4(l_Lightmap, 1);
	
	return output;
}

technique DeferredModelDrawRNM
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}