//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "functions.fx"

////////////////////////////////////////////////////////////////////

sampler2D DiffuseTextureMap : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

////////////////////////////////////////////////////////////////////

struct VertexShaderInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
	float3 Normal	: NORMAL0;
};

struct VertexShaderInstanceInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
	float3 Normal	: NORMAL0;
	float4 Mat1		: TEXCOORD1;
	float4 Mat2		: TEXCOORD2;
	float4 Mat3		: TEXCOORD3;
	float4 Mat4		: TEXCOORD4;
};

struct VertexShaderOutput
{
    float4	Position        : POSITION0;
	float2	TexCoord        : TEXCOORD0;
	float3	Normal			: NORMAL0;
	float3	EyePosition     : NORMAL1;
	float4	WPos			: NORMAL2;
	float	FogLerp			: NORMAL3;
	float2	DepthInt		: NORMAL4;
};

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 DepthRT		: COLOR1;
};

////////////////////////////////////////////////////////////////////

//////////////////////////////////
// Vertex Shaders
//////////////////////////////////

VertexShaderOutput VertexShaderInstanceFunction(VertexShaderInstanceInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;

	float4x4 WorldInstance = (float4x4)0;

	WorldInstance[0] = input.Mat1;
	WorldInstance[1] = input.Mat2;
	WorldInstance[2] = input.Mat3;
	WorldInstance[3] = input.Mat4;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), WorldInstance);	

	output.Position = mul(WorldSpacePosition, ViewProjection);
	output.TexCoord = input.TexCoord;

	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
	output.Normal = mul(input.Normal, WorldInstance);
	
	[flatten]
	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, output.EyePosition) - FogStart) / FogRange);
	}

	/////////////
	//Depth Map
	////////////
	output.DepthInt = output.Position.zw;

    return output;
}

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), World);	

	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord = input.TexCoord;

	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
	output.Normal = mul(input.Normal, World);

	[flatten]
	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, output.EyePosition) - FogStart) / FogRange);
	}

	/////////////
	//Depth Map
	////////////
	output.DepthInt = output.Position.zw;

    return output;
}

//////////////////////////////////
// Pixel Shaders
//////////////////////////////////

PixelShaderOutput PixelShaderFunction(VertexShaderOutput input, uniform bool shadow, uniform bool vegetation, uniform float AlphaTestThreshold, uniform float AlphaTestDirection)
{	
	PixelShaderOutput output = (PixelShaderOutput)0;

	float4 TexColor = tex2D(DiffuseTextureMap, input.TexCoord);

	input.EyePosition = normalize(input.EyePosition);

	float3 Normal = normalize(input.Normal);
	
	float4 AmbientColor = AmbientLightIntensity * AmbientLightColor;
	float4 DiffuseColor = (float4)0;

	for(int i = 0; i < numLights && i < MAX_LIGHTS; ++i)
	{	
		float4 passColor = (float4)0;
		float shadowCoeffStatic = 1.0f;
		float shadowCoeffDynamic = 1.0f;
		
		if(shadow == true)
		{
			if(lightShadowStaticEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler1, i);
				}
				else if(i == 1)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler2, i);
				}
				else if(i == 2)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler3, i);
				}
				else
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler4, i);
				}
			}
			
			if(lightShadowDynamicEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler1, i);
				}
				else if(i == 1)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler2, i);
				}
				else if(i == 2)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler3, i);
				}
				else
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler4, i);
				}
			}
		}
		
		if(lightType[i] == OMNI)
		{
			passColor = CalculateOmniLight(Normal, input.WPos, i);
		}
		else if(lightType[i] == DIRECTIONAL)
		{
			passColor = CalculateDirectionLight(Normal, input.WPos, i);
		}
		else if(lightType[i] == SPOT)
		{
			passColor = CalculateSpotLight(Normal, input.WPos, i);
		}
		
		if(shadow == true)
		{
			passColor *= min(shadowCoeffStatic, shadowCoeffDynamic);
		}
		
		DiffuseColor += passColor;
	}
	
	float4 PixEndColor = (DiffuseColor + AmbientColor) * TexColor;
	
	[flatten]
	if(FogEnable == true)
	{
		PixEndColor.xyz = lerp(PixEndColor.xyz, FogColor, input.FogLerp);
	}

	PixEndColor.a = TexColor.a;
	
	[flatten]
	if(vegetation == true)
	{
		clip((PixEndColor.a - AlphaTestThreshold) * AlphaTestDirection);
	}
	
	output.DiffuseRT = PixEndColor;
	
	/////////////
	//Depth Map
	////////////
	output.DepthRT.r = input.DepthInt.x / input.DepthInt.y;

	return output;
}

//////////////////////////////////
// Techniques
//////////////////////////////////

technique ForwardLightingBasic
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, false, 0.0f, 0.0f);
	}
}

technique ForwardLightingBasicShadow
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, false, 0.0f, 0.0f);
	}
}

technique ForwardLightingBasicVegetation1
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true, 0.95f, 1.0f);
	}
}

technique ForwardLightingBasicVegetation2
{
	pass p0
	{
		
		CullMode			= None;
		ZWriteEnable		= false;
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true, 0.95f, -1.0f);
	}
}

technique ForwardLightingBasicShadowVegetation1
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true, 0.95f, 1.0f);
	}
}

technique ForwardLightingBasicShadowVegetation2
{
	pass p0
	{
		CullMode			= None;
		ZWriteEnable		= false;
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;
		
		//CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true, 0.95f, -1.0f);
	}
}


/// Instance
technique ForwardLightingBasicInstance
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, false, 0.0f, 0.0f);
	}
}

technique ForwardLightingBasicShadowInstance
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, false, 0.0f, 0.0f);
	}
}

technique ForwardLightingBasicVegetation1Instance
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true, 0.95f, 1.0f);
	}
}

technique ForwardLightingBasicVegetation2Instance
{
	pass p0
	{
		
		CullMode			= None;
		ZWriteEnable		= false;
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;
		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true, 0.95f, -1.0f);
	}
}

technique ForwardLightingBasicShadowVegetation1Instance
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true, 0.95f, 1.0f);
	}
}

technique ForwardLightingBasicShadowVegetation2Instance
{
	pass p0
	{	
		CullMode			= None;
		ZWriteEnable		= false;
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= SrcAlpha;
		DestBlend			= InvSrcAlpha;
		
		//CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true, 0.95f, -1.0f);
	}
}