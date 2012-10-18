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

struct VertexShaderInstanceInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
	float4 Normal	: NORMAL0;
	float4 Tangent  : TANGENT0;
	float4 Binormal : BINORMAL0;
	float4 Mat1		: TEXCOORD1;
	float4 Mat2		: TEXCOORD2;
	float4 Mat3		: TEXCOORD3;
	float4 Mat4		: TEXCOORD4;
};

struct VertexShaderOutput
{
    float4		Position		: POSITION0;
	float2		TexCoord		: TEXCOORD0;
	float4		WVPPos			: TEXCOORD1;
	float2		DepthInt		: NORMAL0;
	float2		VelocityMB		: NORMAL1;
	float3		EyePosition		: NORMAL2;
	float4		WPos			: NORMAL3;
	float		FogLerp			: NORMAL4;
	float3x3	TangentToWorld	: NORMAL5;
};

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 DepthRT		: COLOR1;
	float4 MotionBlurRT	: COLOR2;
	float4 DyingColorRT	: COLOR3;
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
	
	output.WVPPos = output.Position;
	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
    output.TangentToWorld[0] = mul(input.Tangent.xyz, WorldInstance);
    output.TangentToWorld[1] = mul(input.Binormal.xyz, WorldInstance);
    output.TangentToWorld[2] = mul(input.Normal.xyz, WorldInstance);
	
	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, output.EyePosition) - FogStart) / FogRange);
	}

	/////////////
	//Depth Map
	////////////
	output.DepthInt = output.Position.zw;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
	output.VelocityMB = MotionBlurVelocity(output.Position, WorldSpacePosition);

	//End Motion Blur
	///////////////

    return output;
}

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), World);	

	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord = input.TexCoord;
	
	output.WVPPos = output.Position;
	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
    // calculate tangent space to world space matrix using the world space tangent,
    // binormal, and normal as basis vectors.  the pixel shader will normalize these
    // in case the world matrix has scaling.
    output.TangentToWorld[0] = mul(input.Tangent.xyz, World);
    output.TangentToWorld[1] = mul(input.Binormal.xyz, World);
    output.TangentToWorld[2] = mul(input.Normal.xyz, World);
	
	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, output.EyePosition) - FogStart) / FogRange);
	}

	/////////////
	//Depth Map
	////////////
	output.DepthInt = output.Position.zw;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
	output.VelocityMB = MotionBlurVelocity(output.Position, WorldSpacePosition);

	//End Motion Blur
	///////////////

    return output;
}

VertexShaderOutput VertexShaderWaterInstanceFunction(VertexShaderInstanceInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;

	float4x4 WorldInstance = (float4x4)0;

	WorldInstance[0] = input.Mat1;
	WorldInstance[1] = input.Mat2;
	WorldInstance[2] = input.Mat3;
	WorldInstance[3] = input.Mat4;
	
	float posY = input.Position.y;

	//Set the position for z to simulate waves in water
	//input.Position.z -= sin( ( (TotalElapsedTime / 4) * 8 ) + ( input.Position.y / 4 ) )/16;
	input.Position.y += sin( ( (TotalElapsedTime / 2) * 8 ) + ( input.Position.z ) ) / 4;
	//input.Position.x -= sin( ( (TotalElapsedTime / 4) * 4 ) + ( input.Position.z / 2 ) )/8;
	input.Position.z -= sin( ( (TotalElapsedTime / 4) * 4 ) + ( posY / 2 ) ) / 8;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), WorldInstance);	

	output.Position = mul(WorldSpacePosition, ViewProjection);
	output.TexCoord = input.TexCoord;
	
	output.WVPPos = output.Position;
	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
    // calculate tangent space to world space matrix using the world space tangent,
    // binormal, and normal as basis vectors.  the pixel shader will normalize these
    // in case the world matrix has scaling.
    output.TangentToWorld[0] = mul(input.Tangent.xyz, WorldInstance);
    output.TangentToWorld[1] = mul(input.Binormal.xyz, WorldInstance);
    output.TangentToWorld[2] = mul(input.Normal.xyz, WorldInstance);
	
	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, output.EyePosition) - FogStart) / FogRange);
	}

	/////////////
	//Depth Map
	////////////
	output.DepthInt = output.Position.zw;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
	output.VelocityMB = MotionBlurVelocity(output.Position, WorldSpacePosition);

	//End Motion Blur
	///////////////

    return output;
}

VertexShaderOutput VertexShaderWaterFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	//Set the position for z to simulate waves in water
	//input.Position.z -= sin( ( (TotalElapsedTime / 4) * 8 ) + ( input.Position.y / 4 ) )/16;
	input.Position.y -= sin( ( (TotalElapsedTime / 4) * 8 ) + ( input.Position.z / 4 ) )/16;
	//input.Position.x -= sin( ( (TotalElapsedTime / 4) * 4 ) + ( input.Position.z / 2 ) )/8;
	//input.Position.z -= sin( ( (TotalElapsedTime / 4) * 4 ) + ( input.Position.y / 2 ) )/8;
	
	float4 WorldSpacePosition = mul(float4(input.Position, 1.0f), World);	

	output.Position = mul(float4(input.Position, 1), WorldViewProjection);
	output.TexCoord = input.TexCoord;
	
	output.WVPPos = output.Position;
	output.WPos = WorldSpacePosition;
	output.EyePosition = CameraPosition - WorldSpacePosition.xyz;
	
    // calculate tangent space to world space matrix using the world space tangent,
    // binormal, and normal as basis vectors.  the pixel shader will normalize these
    // in case the world matrix has scaling.
    output.TangentToWorld[0] = mul(input.Tangent.xyz, World);
    output.TangentToWorld[1] = mul(input.Binormal.xyz, World);
    output.TangentToWorld[2] = mul(input.Normal.xyz, World);

	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, output.EyePosition) - FogStart) / FogRange);
	}

	/////////////
	//Depth Map
	////////////
	output.DepthInt = output.Position.zw;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
	output.VelocityMB = MotionBlurVelocity(output.Position, WorldSpacePosition);

	//End Motion Blur
	///////////////

    return output;
}

//////////////////////////////////
// Pixel Shaders
//////////////////////////////////

PixelShaderOutput PixelShaderFunction(VertexShaderOutput input, uniform bool shadow, uniform bool vegetation)
{
	PixelShaderOutput output = (PixelShaderOutput)0;

	float4 TexColor = tex2D(DiffuseTextureMap, input.TexCoord);

	input.EyePosition = normalize(input.EyePosition);

	// Get the Color of the normal. The color describes the direction of the normal vector
	// and make it range from 0 to 1.
	float3 Normal = (2.0f * tex2D(NormalTextureMap, input.TexCoord) - 1.0f);
	Normal = mul(Normal, input.TangentToWorld);
	Normal = normalize(Normal);
	
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
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler1, i, input.WVPPos);
				}
				else if(i == 1)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler2, i, input.WVPPos);
				}
				else if(i == 2)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler3, i, input.WVPPos);
				}
				else
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler4, i, input.WVPPos);
				}
			}
			
			if(lightShadowDynamicEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler1, i, input.WVPPos);
				}
				else if(i == 1)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler2, i, input.WVPPos);
				}
				else if(i == 2)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler3, i, input.WVPPos);
				}
				else
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler4, i, input.WVPPos);
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
	
	if(FogEnable == true)
	{
		PixEndColor.xyz = lerp(PixEndColor.xyz, FogColor, input.FogLerp);
	}

	PixEndColor.a = TexColor.a;
	
	if(vegetation == true)
	{
		clip(PixEndColor.a - 0.2f);
	}
	
	output.DiffuseRT = PixEndColor;
	
	/////////////
	//Depth Map
	////////////
	output.DepthRT.r = input.DepthInt.x / input.DepthInt.y;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
	output.MotionBlurRT = float4(input.VelocityMB, 1.0f, 1.0f);

	//End Motion Blur
	///////////////

	return output;
}

PixelShaderOutput PixelShaderWaterFunction(VertexShaderOutput input, uniform bool shadow)
{
	PixelShaderOutput output = (PixelShaderOutput)0;

	//Mofidy Tex Coords for Color Texture
	input.TexCoord.y = input.TexCoord.y * 10.0f + sin( (TotalElapsedTime / 4) * 3 + 10 ) / TextureDim.y;
    input.TexCoord.x = input.TexCoord.x * 10.0f;

	float4 TexColor = tex2D(DiffuseTextureMap, input.TexCoord);

	input.EyePosition = normalize(input.EyePosition);

	input.TexCoord.y += ( sin( (TotalElapsedTime / 4) * 3 +10 ) / TextureDim.y ) + ( (TotalElapsedTime / 4) / 16 );

	// Get the Color of the normal. The color describes the direction of the normal vector
	// and make it range from 0 to 1.
	float3 Normal = (2.0f * tex2D(NormalTextureMap, input.TexCoord) - 1.0f);
	Normal = mul(Normal, input.TangentToWorld);
	Normal = normalize(Normal);

	input.TexCoord.y -= ( ( sin( (TotalElapsedTime / 4) * 3 + 10 ) / TextureDim.y ) + ( (TotalElapsedTime / 4) / 16 ) ) * 2;

	float3 Normal2 = (2.0f * tex2D(NormalTextureMap, input.TexCoord) - 1.0f);
	Normal2 = mul(Normal2, input.TangentToWorld);
	Normal2 = normalize(Normal2);

	Normal = ( Normal + Normal2 ) / 2;
	
	float4 AmbientColor = AmbientLightIntensity * AmbientLightColor;
	float4 DiffuseColor = (float4)0;
	float4 Specular = (float4)0;
	float SpecularShine = 0;
	
	for(int i = 0; i < numLights && i < MAX_LIGHTS; ++i)
	{	
		float4 passColor = (float4)0;
		float shadowCoeffStatic = 1.0f;
		float shadowCoeffDynamic = 1.0f;
		float specularTemp = 0;
		
		if(shadow == true)
		{
			if(lightShadowStaticEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler1, i, input.WVPPos);
				}
				else if(i == 1)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler2, i, input.WVPPos);
				}
				else if(i == 2)
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler3, i, input.WVPPos);
				}
				else
				{
					shadowCoeffStatic = CalcShadowVariance(input.WPos, StaticShadowMapSampler4, i, input.WVPPos);
				}
			}
			
			if(lightShadowDynamicEnable[i] == true)
			{
				if(i == 0)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler1, i, input.WVPPos);
				}
				else if(i == 1)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler2, i, input.WVPPos);
				}
				else if(i == 2)
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler3, i, input.WVPPos);
				}
				else
				{
					shadowCoeffDynamic = CalcShadowVariance(input.WPos, DynamicShadowMapSampler4, i, input.WVPPos);
				}
			}
		}
		
		if(lightType[i] == OMNI)
		{
			passColor = CalculateOmniLight(Normal, input.WPos, i);
			Specular += CalculateSpecular(input.WPos, 100, input.EyePosition, Normal, i, false, specularTemp);
			SpecularShine += specularTemp;
		}
		else if(lightType[i] == DIRECTIONAL)
		{
			passColor = CalculateDirectionLight(Normal, input.WPos, i);
			Specular += CalculateSpecular(input.WPos, 100, input.EyePosition, Normal, i, true, specularTemp);
			SpecularShine += specularTemp;
		}
		else if(lightType[i] == SPOT)
		{
			passColor = CalculateSpotLight(Normal, input.WPos, i);
			Specular += CalculateSpecular(input.WPos, 100, input.EyePosition, Normal, i, false, specularTemp);
			SpecularShine += specularTemp;
		}
		
		if(shadow == true)
		{
			passColor *= min(shadowCoeffStatic, shadowCoeffDynamic);
		}
		
		DiffuseColor += passColor;
	}
	
	float4 PixEndColor = (DiffuseColor + AmbientColor) * ((TexColor * .25f) + Specular);
	
	if(FogEnable == true)
	{
		PixEndColor.xyz = lerp(PixEndColor.xyz, FogColor, input.FogLerp);
	}

	PixEndColor.a = 0.8f + SpecularShine;
	
	output.DiffuseRT = PixEndColor;
	
	/////////////
	//Depth Map
	////////////
	output.DepthRT.r = input.DepthInt.x / input.DepthInt.y;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
	output.MotionBlurRT = float4(input.VelocityMB, 1.0f, 1.0f);

	//End Motion Blur
	///////////////

	return output;
}

//////////////////////////////////
// Techniques
//////////////////////////////////

technique ForwardLightingWithNormalWater
{
	pass p0
	{		
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= One;
		DestBlend			= InvSrcAlpha;

		VertexShader = compile vs_3_0 VertexShaderWaterFunction();
		PixelShader = compile ps_3_0 PixelShaderWaterFunction(false);
	}
}

technique ForwardLightingWithNormalShadowWater
{
	pass p0
	{		
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= One;
		DestBlend			= InvSrcAlpha;

		VertexShader = compile vs_3_0 VertexShaderWaterFunction();
		PixelShader = compile ps_3_0 PixelShaderWaterFunction(true);
	}
}

technique ForwardLightingWithNormal
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, false);
	}
}

technique ForwardLightingWithNormalShadow
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, false);
	}
}

technique ForwardLightingWithNormalVegetation
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true);
	}
}

technique ForwardLightingWithNormalShadowVegetation
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true);
	}
}


//Instance

technique ForwardLightingWithNormalWaterInstance
{
	pass p0
	{		
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= One;
		DestBlend			= InvSrcAlpha;

		VertexShader = compile vs_3_0 VertexShaderWaterInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderWaterFunction(false);
	}
}

technique ForwardLightingWithNormalShadowWaterInstance
{
	pass p0
	{		
		AlphaBlendEnable	= true;
		BlendOp				= add;
		SrcBlend			= One;
		DestBlend			= InvSrcAlpha;

		VertexShader = compile vs_3_0 VertexShaderWaterInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderWaterFunction(true);
	}
}

technique ForwardLightingWithNormalInstance
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, false);
	}
}

technique ForwardLightingWithNormalShadowInstance
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, false);
	}
}

technique ForwardLightingWithNormalVegetationInstance
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false, true);
	}
}

technique ForwardLightingWithNormalShadowVegetationInstance
{
	pass p0
	{
		AlphaBlendEnable	= false;
		CullMode			= None;
		
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true, true);
	}
}