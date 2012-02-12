//////////////////////////////////////
//Global Constants				   //
/////////////////////////////////////

#define OMNI 0
#define DIRECTIONAL 1
#define SPOT 2
#define SHADOW_EPSILON 0.00005f

uniform int			numLights				: Num_Lights;
uniform bool		lightEnable				: Lights_Enabled;
uniform int 		lightType				: Lights_Type;
uniform float3		lightPosition			: Lights_Position;
uniform float3		lightDirection			: Lights_Direction;
uniform float3		lightColor				: Lights_Color;
uniform float		lightStartAtt			: Lights_StartAtt;
uniform float		lightEndAtt				: Lights_EndAtt;
uniform float		lightAngle				: Lights_Angle;
uniform float		lightFalloff			: Lights_FallOff;

uniform float4x4	InvertViewProjection	: VIEWPROJECTIONINVERSE;
uniform float4x4	InvertView				: VIEWINVERSE;
uniform float4x4	View					: VIEW;
uniform float4x4	InvertProjection		: PROJECTIONINVERSE;

uniform int			SMap_Size				: SHADOW_MAP_SIZE				=	512;
uniform float3		LightPosition			: SHADOW_CAMERA_POSITION;
uniform float4x4	ShadowViewProjection	: SHADOW_VIEWPROJECTION;
uniform texture		DynamicShadowMap		: LIGHT_DYNAMIC_SHADOW_MAP;
uniform texture		StaticShadowMap			: LIGHT_STATIC_SHADOW_MAP;

//////////////////////////////////////


//////////////////////////////////////
//Textures						   //
/////////////////////////////////////

sampler2D DynamicShadowMapSampler = sampler_state
{
   Texture		= < DynamicShadowMap >;
   MinFilter	= Point;
   MagFilter	= Point;
   MipFilter	= Point;
   AddressU		= Clamp;
   AddressV		= Clamp;
};

sampler2D StaticShadowMapSampler = sampler_state
{
   Texture		= < StaticShadowMap >;
   MinFilter	= Point;
   MagFilter	= Point;
   MipFilter	= Point;
   AddressU		= Clamp;
   AddressV		= Clamp;
};

sampler2D NormalTextureMap : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

sampler2D DepthTextureMap : register( s1 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

//////////////////////////////////////


//////////////////////////////////////
//Structs						   //
/////////////////////////////////////

struct VertexShaderInput
{
	float3 Position : POSITION0;
	float2 TexCoord	: TEXCOORD0;
};

struct VertexShaderOutput
{
	float4 Position : POSITION0;
	float2 TexCoord	: TEXCOORD0;
};
//////////////////////////////////////

//////////////////////////////////////
//Helper Functions				   //
/////////////////////////////////////

float3 UnpackNormal(float3 normal)
{
    float3 unpackNormal = 2.0f * normal - 1.0f;
	
	return unpackNormal;
}

float4 GetPosition(float2 TexCoord, float depthVal)
{
	float4 position;
	
    position.x = TexCoord.x * 2.0f - 1.0f;
	position.y = -(TexCoord.y * 2.0f - 1.0f);
    //position.y = (1 - TexCoord.y) * 2.0f - 1.0f;
    position.z = depthVal;
    position.w = 1.0f;
	
    //transform to world space
    position = mul(position, InvertViewProjection);
    position /= position.w;
	
	return position;
}

float CalculateAttenuation(float distance)
{
	return 1.0f;
}

float CalculateAttenuationFromAngle(float angle)
{
	return 1.0f;
}

//////////////////////////////////////

//////////////////////////////////////
//Light Functions				   //
/////////////////////////////////////

float4 CalculateOmniLight(float3 normal, float4 position)
{
	//Get Light Vector
	float3 lightVector = normalize(lightPosition - position);
	
	float NDotL = saturate(dot(lightVector, normal));
	
	float OmniAttenuation = CalculateAttenuation(1);
	
	float4 OmniColorFinal = (float4(lightColor, 1) * NDotL) * OmniAttenuation;
	
	return (OmniColorFinal);
}

float4 CalculateDirectionLight(float3 normal, float4 position)
{
    float3 lightVector = -normalize(lightDirection);
	
	float LdN = saturate(dot(lightVector, normal));
	
	float4 DirColor = float4(lightColor, 1) * LdN;
	
	return (DirColor);
}

float4 CalculateSpotLight(float3 normal, float4 position)
{	
	float4 SpotColorFinal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	//Get Light Vector
	float3 lightVector = normalize(lightPosition - position);
	
	float SdL = dot(lightVector, -lightDirection);
	float ldM = length(lightVector);
	float sdM = length(lightDirection);
	float Rs = ( SdL / (ldM * sdM) );
	float an = acos(Rs);
	an = degrees(an);
	
	if(an <= (lightFalloff / 2))
	{
		float NDotL = saturate(dot(normal, -lightDirection));
		
		float4 SpotColor = float4(lightColor, 1.0f);
		
		float SpotLightAttenuationDistance = CalculateAttenuation(1);
		
		float SpotLightAttenuationAngle = CalculateAttenuationFromAngle(an);
		
		SpotColor *= (SpotLightAttenuationDistance * SpotLightAttenuationAngle);
		SpotColorFinal = (SpotColor * NDotL);
	}
	
	return ( SpotColorFinal );
}

//////////////////////////////////////


//////////////////////////////////////
//Shadow Functions				   //
/////////////////////////////////////

float CalculateShadowCoeff(float4 position)
{
	float4 LightPos = mul(position, ShadowViewProjection);
	
	float2 ShadowTexC = 0.5 * LightPos.xy / LightPos.w + float2( 0.5, 0.5 );
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	
	// Compute pixel depth for shadowing.
	float depth = LightPos.z / LightPos.w;
	
	// Transform to texel space
    float2 texelpos = SMap_Size * ShadowTexC.xy;
        
    // Determine the lerp amounts.
    float2 lerps = frac( texelpos );
    
    // 2x2 percentage closest filter.
    float dx = 1.0f / SMap_Size;
	float s0 = (tex2D(DynamicShadowMapSampler, ShadowTexC.xy).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s1 = (tex2D(DynamicShadowMapSampler, ShadowTexC.xy + float2(dx, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s2 = (tex2D(DynamicShadowMapSampler, ShadowTexC.xy + float2(0.0f, dx)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s3 = (tex2D(DynamicShadowMapSampler, ShadowTexC.xy + float2(dx, dx)).r   + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	
	float shadowCoeff = lerp( lerp( s0, s1, lerps.x ),
                              lerp( s2, s3, lerps.x ),
                              lerps.y );
							  
	return shadowCoeff;
}

//////////////////////////////////////


//////////////////////////////////////
//VS & PS Functions				   //
/////////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	//Basic Info
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord;
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	float4 FinalPixelColor = (float4)0;
	
	//Get Depth from Map
	float depthVal = tex2D(DepthTextureMap, input.TexCoord).r;
		
	//compute screen-space position
	float4 position = GetPosition(input.TexCoord, depthVal);
	
	//Get Normal From Map and unpack
	float3 normal = tex2D(NormalTextureMap, input.TexCoord).xyz;
	normal = normalize(UnpackNormal(normal));
	
	if(lightType == OMNI)
	{
		FinalPixelColor = CalculateOmniLight(normal, position);
	}
	else if(lightType == DIRECTIONAL)
	{
		FinalPixelColor = CalculateDirectionLight(normal, position);
	}
	else if(lightType == SPOT)
	{
		FinalPixelColor = CalculateSpotLight(normal, position);
	}
	
	float shadowCoeff = CalculateShadowCoeff(position);
	
	FinalPixelColor = saturate(FinalPixelColor * shadowCoeff);

	return FinalPixelColor;
}

//////////////////////////////////////


//////////////////////////////////////
//Techniques					   //
/////////////////////////////////////



technique BasicDeferredLightingAndShadow
{
	pass p0
	{
		AlphaBlendEnable	= True;
		BlendOp				= Add;
		SrcBlend			= One;
		DestBlend			= One;
		CullMode			= CCW;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}