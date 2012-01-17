/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/


float4x4 WorldViewProjection : WORLDVIEWPROJECTION;
float4x4 World : WORLD;
float4x4 View : VIEW;
float4x4 Projection : PROJECTION;

//texture2D ColorTexture;

float AmbientLightIntensity <
    string UIName =  "Ambient Light Intensity";
    string UIWidget = "slider";
    float UIMin = 0.0;
    float UIMax = 1.0;
    float UIStep = 0.1;
> = 1;

float4 AmbientLightColor <
    string UIName =  "Ambient Light Color";
    string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};


/*******************************************/

sampler2D ColorTextureSampler : register( s0 ) = sampler_state
{
   //Texture = < ColorTexture >;
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

struct VertexShaderInput
{
    float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output=(VertexShaderOutput) 0;
	
	float4x4 wvp = mul(mul(World, View), Projection);
	
	output.Position = mul(float4(input.Position, 1.0f), WorldViewProjection);
	
	output.TexCoord = input.TexCoord;
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR 
{
	float4 AmbientColor = AmbientLightColor * AmbientLightIntensity;
	float4 PixColor = (tex2D(ColorTextureSampler, input.TexCoord) * AmbientColor);
	
	//return float4 (1.0f, 1.0f, 1.0f, 1.0f);
	return PixColor;
}

technique technique0 
{
	pass p0 
	{
		//Activamos el Zbuffer, el Zwrite y la función de Z’s que queremos utilizar
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		//Deshabilitamos el alphablend
		AlphaBlendEnable = false;
		//Tipo de culling que queremos utilizar
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}
