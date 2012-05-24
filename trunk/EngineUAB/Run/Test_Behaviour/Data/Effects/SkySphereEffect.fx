
////////////////

uniform extern float4x4 WorldViewProjection : WORLDVIEWPROJECTION;

uniform extern TextureCube SkyTexture : SKY_TEXTURECUBE;

///////////////

sampler SkyTextureSampler = sampler_state 
{
	Texture		= < SkyTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

struct VertexShaderInput
{
    float3 Position : POSITION0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float3 TexCoord : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput) 0;

	output.Position = mul(float4(input.Position, 1.0f), WorldViewProjection).xyww;
	output.TexCoord = input.Position;

	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	return texCUBE(SkyTextureSampler, input.TexCoord);
}

technique SkySphereEffect
{
	pass p0
	{
		CullMode = None;

		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}

struct PixelShaderDeferredOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
};

PixelShaderDeferredOutput PixelShaderDeferredFunction(VertexShaderOutput input)
{
	PixelShaderDeferredOutput output = (PixelShaderDeferredOutput)0;

	output.DiffuseRT	= texCUBE(SkyTextureSampler, input.TexCoord);
	output.NormalRT		= float4(0.5, 0.05, 0.5, 0);
	output.DepthRT		= 1;
	
	return output;
}

technique SkySphereEffectDeferred
{
	pass p0
	{
		CullMode = None;

		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderDeferredFunction();
	}
}