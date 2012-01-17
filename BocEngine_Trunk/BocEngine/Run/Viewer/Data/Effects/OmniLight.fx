/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
float4x4 World : WORLD;

float AmbientLightIntensity <
    string UIName =  "Ambient Light Intensity";
    string UIWidget = "slider";
    float UIMin = 0.0;
    float UIMax = 1.0;
    float UIStep = 0.1;
> = 0.15;

float4 AmbientLightColor <
    string UIName =  "Ambient Light Color";
    string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float OmniLightIntensity <
    string UIName =  "Omni Light Intensity";
    string UIWidget = "slider";
    float UIMin = 0.0;
    float UIMax = 1.0;
    float UIStep = 0.1;
> = 1;

float4 OmniLightColor <
    string UIName =  "Omni Light Color";
    string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float3 OmniLightPosition : POSITION <
	string Object = "OmniPointLight";
	string Space = "World";
> = {0.0f, 0.0f, 0.0f};

float StartAttenuation = 30;
float EndAttenuation = 40;

/*******************************************/

//texture2D ColorTexture;
sampler2D ColorTextureSampler : register( s0 ) = sampler_state
{
   //Texture = < ColorTexture >;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

struct VertexShaderInput
{
    float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 LightVector : NORMAL1;
	float DistanceToLight : TEXCOORD2;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output;
	
	float3 WorldSpacePosition = mul(input.Position, World);
	
	output.Position = mul(input.Position, WorldViewProjection);
	output.TexCoord = input.TexCoord;

	output.LightVector = normalize(OmniLightPosition - WorldSpacePosition);

	output.Normal = normalize(mul(input.Normal, World));
	
	output.DistanceToLight.x = distance(OmniLightPosition, WorldSpacePosition);

    return output;
}

float CalculateAttenuation(float LightDistance)
{
	return ( 1 - saturate( (LightDistance - StartAttenuation) / (EndAttenuation - StartAttenuation) ) );
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR
{	
	float4 AmbientColor = AmbientLightColor * AmbientLightIntensity;
	
	float4 OmniColor = OmniLightColor * OmniLightIntensity;
	
	float NDotL = saturate(dot(input.LightVector, input.Normal));
	
	float OmniAttenuation = CalculateAttenuation(input.DistanceToLight.x);
	
	float4 OmniColorFinal = (OmniColor * NDotL) * OmniAttenuation;
	
	float4 FinalLightColor = AmbientColor + OmniColorFinal;
	
	float4 TexColor = tex2D(ColorTextureSampler, input.TexCoord);

	float4 PixColor = FinalLightColor * (TexColor) ;

    return PixColor;
}

technique technique0 
{	
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}
