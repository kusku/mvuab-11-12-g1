
////////////////

uniform extern float4x4		WorldViewProjection		: WORLDVIEWPROJECTION;
uniform extern TextureCube	SkyTexture				: SKY_TEXTURECUBE;
uniform extern Texture2D	SkyCloudTexture			: SKY_CLOUD_TEXTURE;
uniform extern float		CloudTimer				: CLOUD_TIMER;

///////////////

sampler2D SkyCloudSampler = sampler_state
{ 
	texture		= < SkyCloudTexture >; 
	magfilter	= LINEAR; 
	minfilter	= LINEAR; 
	mipfilter	= LINEAR; 
	AddressU	= clamp;
	AddressV	= clamp;
};

samplerCUBE SkyTextureSampler = sampler_state 
{
	texture		= < SkyTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= clamp;
	AddressV	= clamp;
	AddressW	= clamp;
};

///////////////

struct VertexShaderInput
{
    float3 Position : POSITION0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float3 TexCoord : TEXCOORD0;
};

struct PixelShaderDeferredOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
};

///////////////

float2 SwirlHoriz(float2 coord, bool clockwise)
{
	if(clockwise)
	{
		coord.x +=  CloudTimer;

		if(coord.x > 1)	
		{
			coord.x = coord.x-1;
		}
	}
	else
	{		
		coord.x -= CloudTimer;
		
		if(coord.x < 0)
		{	
			coord.x = coord.x+1;
		}
	}	
	
	return coord;
}

float2 SwirlVert(float2 coord, bool up)
{
	if(up)
	{
		coord.y -= CloudTimer;

		if(coord.y < 0)
		{
			coord.y = coord.y+1;
		}
	}
	else
	{
		coord.y += CloudTimer;

		if(coord.y > 1)	
		{
			coord.y = coord.y-1;
		}
	}
	
	return coord;
}

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput) 0;

	output.Position = mul(float4(input.Position, 1.0f), WorldViewProjection).xyzw;
	output.TexCoord = input.Position;

	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	return texCUBE(SkyTextureSampler, input.TexCoord);
}

float4 PixelShaderFunctionClouds(VertexShaderOutput input) : COLOR0
{	
	float4 cloudCol0 = 0;
	float4 cloudCol1 = 0;
	float4 cloudCol2 = 0;
	
	float2 cloudTex0 = (input.TexCoord.xy);
	float2 cloudTex1 = (input.TexCoord.yz);
	float2 cloudTex2 = (input.TexCoord.xz);	
	
	// Swirl the clouds	
	cloudTex0 = SwirlHoriz(cloudTex0, true);
	cloudTex0 = SwirlVert(cloudTex0, true);
	
	cloudTex1 = SwirlHoriz(cloudTex1, true);
	cloudTex1 = SwirlVert(cloudTex1, true);
	
	cloudTex2 = SwirlHoriz(cloudTex2, false);
	cloudTex2 = SwirlVert(cloudTex2, false);

	cloudCol0 = tex2D(SkyCloudSampler, cloudTex0);	
	cloudCol1 = tex2D(SkyCloudSampler, cloudTex1);
	cloudCol2 = tex2D(SkyCloudSampler, cloudTex2);

	float4 cloudCol = (cloudCol0 - (cloudCol1 + cloudCol2));

	cloudCol *= -input.TexCoord.y;

	return cloudCol;
}

PixelShaderDeferredOutput PixelShaderDeferredFunction(VertexShaderOutput input)
{
	PixelShaderDeferredOutput output = (PixelShaderDeferredOutput)0;

	output.DiffuseRT	= texCUBE(SkyTextureSampler, input.TexCoord);
	output.NormalRT		= float4(0.5, 0.05, 0.5, 0);
	output.DepthRT		= 1;
	
	return output;
}

///////////////

technique SkySphereEffectDeferred
{
	pass p0
	{
		CullMode = None;

		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderDeferredFunction();
	}
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

technique SkySphereEffectClouds
{
	pass p0
	{
		CullMode			= None;
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= SrcAlpha;
		DestBlend			= One;


		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunctionClouds();
	}
}