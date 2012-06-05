//////////////////////////////////////
//Globals						   //
/////////////////////////////////////
#define OMNI 0
#define DIRECTIONAL 1
#define SPOT 2
//#define SHADOW_EPSILON 	0.00005f
#define SHADOW_EPSILON 		0.00000005f
//#define SHADOW_EPSILON 		0.001f
#define MAX_LIGHTS 4

//////////////////////////////////////
//Global Constants				   //
/////////////////////////////////////

float4x4 WorldViewProjection 	: WORLDVIEWPROJECTION;
float4x4 World					: WORLD;

float3		CameraPosition			: CAMERA_POSITION;

uniform float4x4	InvertViewProjection					: VIEWPROJECTIONINVERSE;
uniform float4x4	InvertView								: VIEWINVERSE;
uniform float4x4	InvertProjection						: PROJECTIONINVERSE;
uniform float4x4	ProjectionMatrix						: PROJECTION;

uniform float4x4 	ShadowWorldViewProjection				: SHADOW_WORLDVIEWPROJECTION;
uniform float4x4 	ShadowWorldView							: SHADOW_WORLDVIEW;

uniform int			numLights								: Num_Lights;
uniform int 		lightType[MAX_LIGHTS]					: Lights_Type;
uniform float3		lightPosition[MAX_LIGHTS]				: Lights_Position;
uniform float3		lightDirection[MAX_LIGHTS]				: Lights_Direction;
uniform float3		lightColor[MAX_LIGHTS]					: Lights_Color;
uniform float		lightStartAtt[MAX_LIGHTS]				: Lights_StartAtt;
uniform float		lightEndAtt[MAX_LIGHTS]					: Lights_EndAtt;
uniform float		lightAngle[MAX_LIGHTS]					: Lights_Angle;
uniform float		lightFalloff[MAX_LIGHTS]				: Lights_FallOff;
uniform float		lightIntensity[MAX_LIGHTS]				: Lights_Intensity;
uniform float2		lightLinNearFar[MAX_LIGHTS]				: Lights_LinNearFar;
uniform float2		shLightLinNearFar						: Lights_Shadow_LinNearFar;

uniform float2		HalfPixel								: HALFPIXEL;

uniform int			SMap_Size								: SHADOW_MAP_SIZE				=	2048;
uniform float4x4	ShadowViewProjection[MAX_LIGHTS]		: SHADOW_VIEWPROJECTION;
uniform bool		lightShadowDynamicEnable[MAX_LIGHTS]	: Lights_Shadow_Dynamic_Enable;
uniform bool		lightShadowStaticEnable[MAX_LIGHTS]		: Lights_Shadow_Static_Enable;
uniform texture2D	StaticShadowMap1						: STATIC_SHADOW_MAP_1;
uniform texture2D	StaticShadowMap2						: STATIC_SHADOW_MAP_2;
uniform texture2D	StaticShadowMap3						: STATIC_SHADOW_MAP_3;
uniform texture2D	StaticShadowMap4						: STATIC_SHADOW_MAP_4;
uniform texture2D	DynamicShadowMap1						: DYNAMIC_SHADOW_MAP_1;
uniform texture2D	DynamicShadowMap2						: DYNAMIC_SHADOW_MAP_2;
uniform texture2D	DynamicShadowMap3						: DYNAMIC_SHADOW_MAP_3;
uniform texture2D	DynamicShadowMap4						: DYNAMIC_SHADOW_MAP_4;

//Variance

uniform float		VSMMinVariance = 0.000001;	// Minimum variance for VSM
uniform bool		LBREnable = true;			// Enable/disable light bleeding reduction
uniform float		LBRAmount = 0.25;			// Aggressiveness of light bleeding reduction

/////

sampler StaticShadowMapSampler1 = sampler_state
{
   Texture		= <StaticShadowMap1>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

sampler StaticShadowMapSampler2 = sampler_state
{
   Texture		= <StaticShadowMap2>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

sampler StaticShadowMapSampler3 = sampler_state
{
   Texture		= <StaticShadowMap3>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

sampler StaticShadowMapSampler4 = sampler_state
{
   Texture		= <StaticShadowMap4>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

sampler DynamicShadowMapSampler1 = sampler_state
{
   Texture		= <DynamicShadowMap1>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

sampler DynamicShadowMapSampler2 = sampler_state
{
   Texture		= <DynamicShadowMap2>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

sampler DynamicShadowMapSampler3 = sampler_state
{
   Texture		= <DynamicShadowMap3>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

sampler DynamicShadowMapSampler4 = sampler_state
{
   Texture		= <DynamicShadowMap4>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 16;
};

//////////////////////////////////////
//Helper Functions				   //
/////////////////////////////////////
float3 PackNormal(float3 normal)
{
	float3 packNormal = 0.5f * (normal + 1.0f);
	
	return packNormal;
}

float3 UnpackNormal(float3 normal)
{
    float3 unpackNormal = (2.0f * normal) - 1.0f;
	
	//enc method 3
	//return normal;

	return unpackNormal;
}

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV, float4x4 l_InvertProjection)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, l_InvertProjection);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV, float4x4 l_InvertView, float4x4 l_InvertProjection)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV, l_InvertProjection);
	return mul(float4(l_PositionView,1.0), l_InvertView).xyz;
}

float4 GetPositionFromDepth(float2 TexCoord, float depthVal)
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

//////////////////////////////////////

//////////////////////////////////////
//Light Functions				   //
/////////////////////////////////////

float CalculateAttenuation(float distance, float startAtt, float endAtt)
{
	return ( 1 - saturate( (distance - startAtt) / (endAtt - startAtt) ) );
}

float CalculateAttenuationFromAngle(float angle, float ltAngle, float ltFalloff)
{
	float startAngle = ltAngle / 2;
	float endAngle = ltFalloff / 2;
	
	return (1 - saturate( (angle - startAngle) / (endAngle - startAngle) ) );
}

float4 CalculateOmniLight(float3 normal, float4 position, int lightNum)
{
	//Get Light Vector
	float lightDistance = distance(lightPosition[lightNum], position.xyz);
	float3 lightVector = normalize(lightPosition[lightNum] - position.xyz);
	
	float NDotL = max(0, dot(normal, lightVector));
	
	float OmniAttenuation = CalculateAttenuation(lightDistance, lightStartAtt[lightNum], lightEndAtt[lightNum]);
	
	float4 OmniColorFinal = (float4(lightColor[lightNum] * lightIntensity[lightNum], 1) * NDotL) * OmniAttenuation;
	
	return OmniColorFinal;
}

float4 CalculateDirectionLight(float3 normal, float4 position, int lightNum)
{
    float3 lightVector = -normalize(lightDirection[lightNum]);
	
	float LdN = max(0, dot(lightVector, normal));
	
	float4 DirColor = float4(lightColor[lightNum] * lightIntensity[lightNum], 1) * LdN;
	
	return (DirColor);
}

float4 CalculateSpotLight(float3 normal, float4 position, int lightNum)
{	
	float4 SpotColorFinal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	//Get Light Vector
	float lightDistance = distance(lightPosition[lightNum], position);
	float3 lightVector = normalize(lightPosition[lightNum] - position);
	lightVector = normalize(lightVector);
	
	float SdL = dot(lightVector, -lightDirection[lightNum]);
	float ldM = length(lightVector);
	float sdM = length(lightDirection[lightNum]);
	float Rs = ( SdL / (ldM * sdM) );
	float an = acos(Rs);
	an = degrees(an);
	
	if(an <= (lightFalloff[lightNum] / 2))
	{
		float NDotL = saturate(dot(lightVector, normal));
		
		//float4 SpotColor = float4(1, 1, 1, 1);
		float4 SpotColor = float4(lightColor[lightNum] * lightIntensity[lightNum], 1.0f);
		
		float SpotLightAttenuationDistance = CalculateAttenuation(lightDistance, lightStartAtt[lightNum], lightEndAtt[lightNum]);
		
		float SpotLightAttenuationAngle = CalculateAttenuationFromAngle(an, lightAngle[lightNum], lightFalloff[lightNum]);
		
		SpotColor *= (SpotLightAttenuationDistance * SpotLightAttenuationAngle);
		SpotColorFinal = (SpotColor * NDotL);
	}
	
	return ( SpotColorFinal );
}

//////////////////////////////////////

//////////////////////////////////////
//Shadow Functions				   //
/////////////////////////////////////

float CalculateShadowCoeff(float4 position, sampler2D shadowMapSampler, int light)
{
	float4 ShadowPos = mul(position, ShadowViewProjection[light]);
	
	// Project the texture coords and scale/offset to [0, 1].
	ShadowPos.xy /= ShadowPos.w;
	ShadowPos.x =  0.5f*ShadowPos.x + 0.5f; 
	ShadowPos.y = -0.5f*ShadowPos.y + 0.5f;
	
	//float2 ShadowTexC = 0.5 * LightPos.xy / LightPos.w + float2( 0.5, 0.5 );
	//ShadowTexC.y = 1.0f - ShadowTexC.y;
	
	// Compute pixel depth for shadowing.
	float depth = (ShadowPos.z / ShadowPos.w);
	
	// Transform to texel space
    float2 texelpos = SMap_Size * ShadowPos.xy;
        
    // Determine the lerp amounts.
    float2 lerps = frac( texelpos );
    
    // 2x2 percentage closest filter.
    float dx = 1.0f / SMap_Size;
	float s0 = (tex2D(shadowMapSampler, ShadowPos.xy).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s1 = (tex2D(shadowMapSampler, ShadowPos.xy + float2(dx, 0.0f)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s2 = (tex2D(shadowMapSampler, ShadowPos.xy + float2(0.0f, dx)).r + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	float s3 = (tex2D(shadowMapSampler, ShadowPos.xy + float2(dx, dx)).r   + SHADOW_EPSILON < depth) ? 0.0f : 1.0f;
	
	float shadowCoeff = lerp( lerp( s0, s1, lerps.x ),
                              lerp( s2, s3, lerps.x ),
                              lerps.y );

	//return s0;
	return shadowCoeff;
}

float CalcShadowCoeffVSM(float4 Pos, sampler shadowMapSampler, int light)
{	
	float lightAmount = 1.0;

	//float2 depth = (float2) 0;
	float depth = 0;
	float2 ShadowTexC = (float2)0;
	
	float4 ShadowPos = mul(Pos, ShadowViewProjection[light]);
	
	// Project the texture coords and scale/offset to [0, 1].
	ShadowPos.xy /= ShadowPos.w;
	ShadowPos.x =  0.5f*ShadowPos.x + 0.5f; 
	ShadowPos.y = -0.5f*ShadowPos.y + 0.5f;
	
	ShadowTexC.x = ShadowPos.x;
	ShadowTexC.y = ShadowPos.y;
	//depth.x = ShadowPos.z;
	//depth.y = ShadowPos.w;
	depth = ShadowPos.z / ShadowPos.w;
	
	float2 moments = tex2Dlod( shadowMapSampler, float4(ShadowTexC, 0, 1)).xy;
	//float2 moments = tex2D(shadowMapSampler, ShadowTexC).xy;

	float mean = moments.x;
	float meanSqr = moments.y;	
	float Ex_2 = mean * mean;
	float E_x2 = meanSqr;
	float variance = min(max(E_x2 - Ex_2, 0.0f) + SHADOW_EPSILON, 1.0f);
	float m_d = (depth - mean);
	float p = variance / (variance + m_d * m_d);

	// Reduce light bleeding
	float min = 2.5f;

	p = saturate(pow(p, min) + 0.2f);

	lightAmount = max(p, depth <= mean);	

	return lightAmount; 
}

float2 CalculateParallax(float2 scaleAmount, float2 texCoord, float height, float3 ViewDirection)
{
	float depthAmount = scaleAmount.x * height  + scaleAmount.y;
	float2 ParallaxTexCoord = depthAmount * ViewDirection + texCoord;
	
	return ParallaxTexCoord;
}

float3 GetRadiosityNormalMap(float3 Nn, float2 UV, float3x3 WorldMatrix, sampler2D rnmX, sampler2D rnmY, sampler2D rnmZ)
{
	float3 l_LightmapX = tex2D(rnmX, UV)*2;
	float3 l_LightmapY = tex2D(rnmY, UV)*2;
	float3 l_LightmapZ = tex2D(rnmZ, UV)*2;

	float3 l_BumpBasisX = normalize(float3(0.816496580927726, 0.5773502691896258, 0 ));
	float3 l_BumpBasisY = normalize(float3(-0.408248290463863, 0.5773502691896258, 0.7071067811865475 ));
	float3 l_BumpBasisZ = normalize(float3(-0.408248290463863, 0.5773502691896258, -0.7071067811865475));
	
	float3 diffuseLighting = saturate( dot( Nn, l_BumpBasisX ) ) * l_LightmapX + saturate( dot( Nn, l_BumpBasisY ) ) * l_LightmapY + saturate( dot( Nn, l_BumpBasisZ ) ) * l_LightmapZ;
	
	return diffuseLighting;
}

//////////////////////////////////////

//Variance Shadow Map

//////////////////////////////////////

float linstep(float min, float max, float v)
{
	return clamp((v - min) / (max - min), 0, 1);
}

// Rescale into [0, 1]
float RescaleDistToLight(float Distance, int light)
{
    return linstep(lightLinNearFar[light].x, lightLinNearFar[light].y, Distance);
}

float RescaleDistToLight(float Distance)
{
    return linstep(shLightLinNearFar.x, shLightLinNearFar.y, Distance);
}

float2 GetFPBias()
{
    return float2(0.5, 0);
}

// Utility function
float2 ComputeMoments(float Depth)
{
    // Compute first few moments of depth
    float2 Moments;
    Moments.x = Depth;
    Moments.y = Depth * Depth;
    
    // Ajust the variance distribution to include the whole pixel if requested
    // NOTE: Disabled right now as a min variance clamp takes care of all problems
    // and doesn't risk screwy hardware derivatives.
    //float dx = ddx(Depth);
    //float dy = ddy(Depth);
    //float Delta = 0.25 * (dx*dx + dy*dy);
    // Perhaps clamp maximum Delta here
    //Moments.y += Delta;

    return Moments;
}

float LBR(float p)
{
    // Lots of options here if we don't care about being an upper bound.
    // Use whatever falloff function works well for your scene.
    return linstep(LBRAmount, 1, p);
    //return smoothstep(g_LBRAmount, 1, p);
}

float ChebyshevUpperBound(float2 Moments, float Mean, float MinVariance)
{
    // Standard shadow map comparison
    float p = (Mean <= Moments.x);
    
    // Compute variance
    float Variance = Moments.y - (Moments.x * Moments.x);
    Variance = max(Variance, MinVariance);
    
    // Compute probabilistic upper bound
    float d     = Mean - Moments.x;
    float p_max = Variance / (Variance + d*d);
    
    return max(p, p_max);
}

float CalcShadowVariance(float4 Pos, sampler shadowMapSampler, int light)
{		
	float4 ShadowPos = mul(Pos, ShadowViewProjection[light]);
	
	// Project the texture coords and scale/offset to [0, 1].
    float2 ShadowTexC = (ShadowPos.xy / ShadowPos.w) * float2(0.5, -0.5) + 0.5;
		
    //float3 DirToLight = lightPosition[light] - Pos.xyz;
    //float DistToLight = length(DirToLight);
	
	//float RescaledDist = RescaleDistToLight(DistToLight, light);
	float RescaledDist = ShadowPos.z / ShadowPos.w;
	
	float2 Moments = tex2Dlod( shadowMapSampler, float4(ShadowTexC, 0, 1)).xy;
	//float2 Moments = tex2D(shadowMapSampler, ShadowTexC).rg;
    Moments = Moments + GetFPBias();
	
    float ShadowContrib = ChebyshevUpperBound(Moments, RescaledDist, VSMMinVariance);
    
    [flatten] 
	if (LBREnable)
	{
        ShadowContrib = LBR(ShadowContrib);
    }
	
	return ShadowContrib;
}

