

float AmbientLightIntensity <
    string UIName =  "Ambient Light Intensity";
    string UIWidget = "slider";
    float UIMin = 0.0;
    float UIMax = 1.0;
    float UIStep = 0.1;
> = 0.10;

float4 AmbientLightColor <
    string UIName =  "Ambient Light Color";
    string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

/////////////////////////////////////////////////////

sampler DiffuseTextureSampler : register( s0 ) = sampler_state
{
    AddressU = CLAMP;
    AddressV = CLAMP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

sampler LightTextureSampler : register( s1 ) = sampler_state
{
    AddressU = CLAMP;
    AddressV = CLAMP;
    MagFilter = LINEAR;
    MinFilter = LINEAR;
    Mipfilter = LINEAR;
};

///////////////////////////////////////////////

struct VertexShaderInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	output.TexCoord = input.TexCoord;
	
	output.Position = float4(input.Position, 1);
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR
{
	float4 diffuseColor = tex2D(DiffuseTextureSampler, input.TexCoord);
    float4 light = tex2D(LightTextureSampler,input.TexCoord);
	
	float4 PixColor = diffuseColor * ( light + (AmbientLightIntensity * AmbientLightColor) );

	//return float4 (1.0f, 1.0f, 1.0f, 1.0f);
	PixColor.a = 1.0f;
	return PixColor;
	//return light;
}

technique BasicDeferredLightingCombine
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}