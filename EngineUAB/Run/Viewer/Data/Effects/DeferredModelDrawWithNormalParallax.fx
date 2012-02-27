//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "Globals.fx"
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
    float4 Position 		: POSITION0;
    float2 TexCoord 		: TEXCOORD0;
	float2 Depth			: TEXCOORD1;
	float3x3 TangentToWorld : TEXCOORD2;
	float3 ViewDirection	: NORMAL0;
};

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	float3 WorldPosition = mul(float4(input.Position, 1), World).xyz;
	
	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord	= input.TexCoord;
	output.Depth.x	= output.Position.z; 
	output.Depth.y	= output.Position.w; 
	
    // calculate tangent space to world space matrix using the world space tangent,
    // binormal, and normal as basis vectors.  the pixel shader will normalize these
    // in case the world matrix has scaling.
    output.TangentToWorld[0] = mul(input.Tangent.xyz,    World);
    output.TangentToWorld[1] = mul(input.Binormal.xyz,    World);
    output.TangentToWorld[2] = mul(input.Normal.xyz,    World);
	
	
	output.ViewDirection = WorldPosition - CameraPosition;
	output.ViewDirection = mul(output.ViewDirection, output.TangentToWorld);
	
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
	
	float2 scaleAmount = float2(0.03f, -0.025f);
	
	input.ViewDirection = normalize(input.ViewDirection);
	input.TexCoord = CalculateParallax(scaleAmount, input.TexCoord, NormalMapTex.a, input.ViewDirection);

	output.DiffuseRT	= tex2D(DiffuseTextureMap, input.TexCoord);
	output.DepthRT		= input.Depth.x / input.Depth.y;
	output.NormalRT.rgb	= PackNormal(normalize(Normal));
	
	return output;
}

technique DeferredModelDrawWithNormalParallax
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}