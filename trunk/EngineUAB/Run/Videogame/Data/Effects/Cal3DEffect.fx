
#include "functions.fx"

////////////////////////////////////////////////////////////////////

#define MAXBONES 29

float3x4 g_Bones[MAXBONES] : BONES;

////////////////////////////////////////////////////////////////////

sampler DiffuseTextureSampler : register( s0 ) = sampler_state
{
	MipFilter = linear;
	MinFilter = linear;
	MagFilter = linear;
	AddressU = clamp;
	AddressV = clamp;
};

sampler NormalTextureSampler : register( s1 ) = sampler_state
{
	MipFilter = linear;
	MinFilter = linear;
	MagFilter = linear;
	AddressU = clamp;
	AddressV = clamp;
};

////////////////////////////////////////////////////////////////////

struct VertexShaderInput
{
	float3 Position	: POSITION0;
	float4 Weight	: BLENDWEIGHT0;
	float4 Indices	: BLENDINDICES0;
	float4 Normal	: NORMAL0;
	float2 TexCoord	: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4	Position		: POSITION0;
	float2	TexCoord		: TEXCOORD0;
	float4	WVPPos			: TEXCOORD1;
	float3	Normal			: NORMAL0;
	float3	EyePosition		: NORMAL1;
	float4	WPos			: NORMAL2;
	float2	DepthInt		: NORMAL3;
	float2	VelocityMB		: NORMAL4;
};

struct VertexShaderNormalInput
{
	float3	Position	: POSITION0;
	float4	Weight		: BLENDWEIGHT0;
	float4	Indices		: BLENDINDICES0;
	float4	Normal		: NORMAL0;
	float4	Tangent		: TANGENT0;
	float4	BiNormal	: BINORMAL0;
	float2	TexCoord	: TEXCOORD0;
};

struct VertexShaderNormalOutput
{
    float4		Position		: POSITION0;
	float2		TexCoord		: TEXCOORD0;
	float4		WVPPos			: TEXCOORD1;
	float2		DepthInt		: NORMAL0;
	float2		VelocityMB		: NORMAL1;
	float3		EyePosition		: NORMAL2;
	float4		WPos			: NORMAL3;
	float3x3	TangentToWorld	: NORMAL4;
};

struct VertexShaderShadowInput
{
	float3 Position	: POSITION0;
	float4 Weight	: BLENDWEIGHT0;
	float4 Indices	: BLENDINDICES0;
};

struct VertexShaderShadowOutput
{
	float4 Position	: POSITION0;
	float2 Depth	: TEXCOORD0;
};

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 DepthRT		: COLOR1;
	float4 MotionBlurRT	: COLOR2;
};

struct PixelShaderDROutput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
};

////////////////////////////////////////////////////////////////////

//////////////////////////////////
// Helper Functions
//////////////////////////////////

float3 CalcAnimtedPos(float4 Position, float4 Indices, float4 Weight)
{
	float3 l_Position=0;
	
	l_Position = mul(g_Bones[Indices.x], Position) * Weight.x;
	l_Position += mul(g_Bones[Indices.y], Position) * Weight.y;
	l_Position += mul(g_Bones[Indices.z], Position) * Weight.z;
	l_Position += mul(g_Bones[Indices.w], Position) * Weight.w;
	
	return l_Position;
}

void CalcAnimatedNormalTangent(float3 Normal, float3 Tangent, float4 Indices, float4 Weight, out float3 OutNormal, out float3 OutTangent)
{
	OutNormal = 0;
	OutTangent =0;
	float3x3 m;
	
	m[0].xyz = g_Bones[Indices.x][0].xyz;
	m[1].xyz = g_Bones[Indices.x][1].xyz;
	m[2].xyz = g_Bones[Indices.x][2].xyz;
	
	OutNormal += mul(m, Normal.xyz)* Weight.x;
	OutTangent += mul(m, Tangent.xyz)* Weight.x;
	
	m[0].xyz = g_Bones[Indices.y][0].xyz;
	m[1].xyz = g_Bones[Indices.y][1].xyz;
	m[2].xyz = g_Bones[Indices.y][2].xyz;
	
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.y);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.y);
	
	m[0].xyz = g_Bones[Indices.z][0].xyz;
	m[1].xyz = g_Bones[Indices.z][1].xyz;
	m[2].xyz = g_Bones[Indices.z][2].xyz;
	
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.z);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.z);
	
	m[0].xyz = g_Bones[Indices.w][0].xyz;
	m[1].xyz = g_Bones[Indices.w][1].xyz;
	m[2].xyz = g_Bones[Indices.w][2].xyz;
	
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.w);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.w);
	
	OutNormal = normalize(OutNormal);
	OutTangent = normalize(OutTangent);
}

void CalcAnimatedNormal(float3 Normal, float4 Indices, float4 Weight, out float3 OutNormal)
{
	OutNormal = 0;
	float3x3 m;
	
	m[0].xyz = g_Bones[Indices.x][0].xyz;
	m[1].xyz = g_Bones[Indices.x][1].xyz;
	m[2].xyz = g_Bones[Indices.x][2].xyz;
	
	OutNormal += mul(m, Normal.xyz)* Weight.x;
	
	m[0].xyz = g_Bones[Indices.y][0].xyz;
	m[1].xyz = g_Bones[Indices.y][1].xyz;
	m[2].xyz = g_Bones[Indices.y][2].xyz;
	
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.y);
	
	m[0].xyz = g_Bones[Indices.z][0].xyz;
	m[1].xyz = g_Bones[Indices.z][1].xyz;
	m[2].xyz = g_Bones[Indices.z][2].xyz;
	
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.z);
	
	m[0].xyz = g_Bones[Indices.w][0].xyz;
	m[1].xyz = g_Bones[Indices.w][1].xyz;
	m[2].xyz = g_Bones[Indices.w][2].xyz;
	
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.w);
	
	OutNormal = normalize(OutNormal);
}

//////////////////////////////////
// Vertex Shaders
//////////////////////////////////

/*********************
         Basic
**********************/
VertexShaderOutput RenderCal3DHWVS(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	float3 l_Normal= 0;
	
	CalcAnimatedNormal(input.Normal.xyz, input.Indices, input.Weight, l_Normal);
	
	float3 l_Position = CalcAnimtedPos(float4(input.Position.xyz,1.0), input.Indices, input.Weight);

	float4 WorldSpacePosition = mul(float4(l_Position, 1.0f), World);

	output.Position	= mul(float4(l_Position, 1), WorldViewProjection );
	output.TexCoord = input.TexCoord;
	
	output.WVPPos		= output.Position;
	output.WPos			= WorldSpacePosition;
	output.EyePosition	= CameraPosition - WorldSpacePosition.xyz;

    output.Normal = mul(l_Normal, World);

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

/*********************
       Shadow
**********************/
VertexShaderShadowOutput RenderCal3DHWShadowVS(VertexShaderShadowInput input)
{
	VertexShaderShadowOutput output = (VertexShaderShadowOutput)0;
	
	float3 position = CalcAnimtedPos(float4(input.Position.xyz,1.0), input.Indices, input.Weight);
	
	output.Position = mul(float4(position, 1.0), ShadowWorldViewProjection);
	
	output.Depth.xy = output.Position.zw;
	
	return output;
}

/*********************
    Normal/Tangent
**********************/
VertexShaderNormalOutput RenderCal3DHWNormalVS(VertexShaderNormalInput input)
{
	VertexShaderNormalOutput output = (VertexShaderNormalOutput)0;
	
	float3 l_Normal= 0;
	float3 l_Tangent=0;
	
	CalcAnimatedNormalTangent(input.Normal.xyz, input.Tangent.xyz, input.Indices, input.Weight, l_Normal, l_Tangent);
	
	float3 l_Position = CalcAnimtedPos(float4(input.Position.xyz,1.0), input.Indices, input.Weight);

	float4 WorldSpacePosition = mul(float4(l_Position, 1.0f), World);

	output.Position	= mul(float4(l_Position, 1), WorldViewProjection );
	output.TexCoord = input.TexCoord;
	
	output.WVPPos		= output.Position;
	output.WPos			= WorldSpacePosition;
	output.EyePosition	= CameraPosition - WorldSpacePosition.xyz;

    output.TangentToWorld[0] = mul(l_Tangent, World);
    output.TangentToWorld[1] = mul(cross(l_Tangent,l_Normal),(float3x3)World);
    output.TangentToWorld[2] = mul(l_Normal, World);

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

PixelShaderOutput RenderCal3DHWPS(VertexShaderOutput input, uniform bool shadow)
{
	PixelShaderOutput output = (PixelShaderOutput)0;

	float4 TexColor = tex2D(DiffuseTextureSampler, input.TexCoord);
	
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

	PixEndColor.a = TexColor.a;
	
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

PixelShaderOutput RenderCal3DHWNormalPS(VertexShaderNormalOutput input, uniform bool shadow)  
{
	PixelShaderOutput output = (PixelShaderOutput)0;

	float4 TexColor = tex2D(DiffuseTextureSampler, input.TexCoord);
	
	input.EyePosition = normalize(input.EyePosition);

	// Get the Color of the normal. The color describes the direction of the normal vector
	// and make it range from 0 to 1.
	float3 Normal = (2.0f * tex2D(NormalTextureSampler, input.TexCoord) - 1.0f);
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

	PixEndColor.a = TexColor.a;
	
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

/*PixelShaderDROutput RenderCal3DHWDRPS(CAL3D_HW_VERTEX_PS IN)
{
	PixelShaderDROutput output = (PixelShaderDROutput)0;
	
	output.DepthDR		= IN.Depth.x / IN.Depth.y;
	output.NormalRT.xyz	= IN.WorldNormal;
	output.DiffuseRT	= tex2D(DiffuseTextureSampler, IN.UV);
	
	return output;
}*/

float4 RenderCal3DHWShadowPS(VertexShaderShadowOutput input) : COLOR
{	
	float Depth = input.Depth.x / input.Depth.y;
	
    float2 Moments = ComputeMoments(Depth) - GetFPBias();
    
	float4 ret = (float4)0;
	
	ret.r = Moments.x;
	ret.g = Moments.y;
	
	return ret;
}

//////////////////////////////////
// Techniques
//////////////////////////////////

technique Cal3DTechniqueShadow
{
	pass p0
	{
		VertexShader = compile vs_3_0 RenderCal3DHWShadowVS();
		PixelShader = compile ps_3_0 RenderCal3DHWShadowPS();
	}
}

technique Cal3DTechniqueBasic
{
	pass p0
	{
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 RenderCal3DHWPS(false);
	}
}

technique Cal3DTechniqueBasicShadow
{
	pass p0
	{
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 RenderCal3DHWPS(true);
	}
}

technique Cal3DTechniqueNormal
{
	pass p0
	{
		VertexShader = compile vs_3_0 RenderCal3DHWNormalVS();
		PixelShader = compile ps_3_0 RenderCal3DHWNormalPS(false);
	}
}

technique Cal3DTechniqueNormalShadow
{
	pass p0
	{
		VertexShader = compile vs_3_0 RenderCal3DHWNormalVS();
		PixelShader = compile ps_3_0 RenderCal3DHWNormalPS(true);
	}
}

/*technique Cal3DTechniqueDR
{
	pass p0
	{
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 RenderCal3DHWDRPS();
	}
}*/