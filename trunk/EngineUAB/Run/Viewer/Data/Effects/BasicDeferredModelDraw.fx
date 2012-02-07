
//////////////////////////////////

float4x4 WorldViewProjection 	: WORLDVIEWPROJECTION;
float4x4 World					: WORLD;

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
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
	float3 Normal	: NORMAL0;
	float2 Depth	: TEXCOORD1;
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
	
	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.Normal	= normalize(mul(input.Normal, World));
	output.TexCoord	= input.TexCoord;
	output.Depth.xy	= output.Position.zw; 
	
	return output;
}

float3 PackNormal(float3 normal)
{
	float3 packNormal = 0.5f * (normal + 1.0f);
	
	return packNormal;
}

PixelShaderOutput PixelShaderFunction(VertexShaderOutput input)
{
	PixelShaderOutput output = (PixelShaderOutput)0;

	output.DiffuseRT	= tex2D(DiffuseTextureMap, input.TexCoord);
	output.DepthRT		= input.Depth.x / input.Depth.y;
	output.NormalRT.xyz	= PackNormal(normalize(input.Normal));
	
	return output;
}

technique BasicDeferredModelDraw
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}