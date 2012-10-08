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
//243.0 115.0 13.0

uniform bool		FogEnable = true;
uniform float3		FogColor = float3(0.9f, 0.65f, 0.05f);
uniform float		FogStart = 200.0f;
uniform float		FogRange = 350.0f;

uniform float4x4	World									: WORLD;
uniform float4x4	WorldViewProjection 					: WORLDVIEWPROJECTION;

uniform float4x4	ViewMatrix								: VIEW;
uniform float4x4	InvertView								: VIEWINVERSE;
uniform float4x4	PrevView								: VIEW_PREV;

uniform float4x4	ProjectionMatrix						: PROJECTION;
uniform float4x4	InvertProjection						: PROJECTIONINVERSE;
uniform float4x4	PrevProjectionMatrix					: PROJECTION_PREV;

uniform float4x4	ViewProjection 							: VIEWPROJECTION;
uniform float4x4	InvertViewProjection					: VIEWPROJECTIONINVERSE;
uniform float4x4	PrevViewProjection						: VIEWPROJECTION_PREV;

uniform float2		RenderTargetSize						: RENDER_TARGET_SIZE;

uniform float3		CameraPosition							: CAMERA_POSITION;

uniform float		ElapsedTime								: ELAPSED_TIME;
uniform float		TotalElapsedTime						: TOTAL_ELAPSED_TIME;

uniform float2		TextureDim								: TEXTURE_DIM;

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

//Cascade
#define NUM_CASCADES 3

uniform float4x4	CascadeShadowViewProjection[MAX_LIGHTS][NUM_CASCADES]	: CASCADE_SHADOW_VIEWPROJECTION;
uniform float3		CascadeDistances[MAX_LIGHTS]							: CASCADE_DISTANCES;
uniform float2		CascadeShadowMapPixelSize[MAX_LIGHTS]					: CASCADE_SHADOW_MAP_PIXEL_SIZE;

static int CascadeGroup = 0;

//uniform float2		CascadeClipPlanes[MAX_LIGHTS][NUM_CASCADES]				: CASCADE_NUM_SPLITS;
//uniform float2		CascadeShadowMapSize[MAX_LIGHTS]						: CASCADE_SHADOW_MAP_SIZE;

uniform float4x4 	ShadowWorldViewProjection				: SHADOW_WORLDVIEWPROJECTION;
uniform float4x4 	ShadowWorldView							: SHADOW_WORLDVIEW;
uniform float4x4 	ShadowView								: SHADOW_VIEW;
uniform int			SMap_Size								: SHADOW_MAP_SIZE				=	2048;
uniform float4x4	ShadowViewProjection[MAX_LIGHTS]		: SHADOW_VIEWPROJECTION;
uniform bool		lightShadowDynamicEnable[MAX_LIGHTS]	: Lights_Shadow_Dynamic_Enable;
uniform bool		lightShadowStaticEnable[MAX_LIGHTS]		: Lights_Shadow_Static_Enable;
uniform Texture2D	StaticShadowMap1						: STATIC_SHADOW_MAP_1;
uniform Texture2D	StaticShadowMap2						: STATIC_SHADOW_MAP_2;
uniform Texture2D	StaticShadowMap3						: STATIC_SHADOW_MAP_3;
uniform Texture2D	StaticShadowMap4						: STATIC_SHADOW_MAP_4;
uniform Texture2D	DynamicShadowMap1						: DYNAMIC_SHADOW_MAP_1;
uniform Texture2D	DynamicShadowMap2						: DYNAMIC_SHADOW_MAP_2;
uniform Texture2D	DynamicShadowMap3						: DYNAMIC_SHADOW_MAP_3;
uniform Texture2D	DynamicShadowMap4						: DYNAMIC_SHADOW_MAP_4;

//Variance
//0.000001
uniform float		VSMMinVariance = 0.000001;	// Minimum variance for VSM
uniform bool		LBREnable = true;			// Enable/disable light bleeding reduction
uniform float		LBRAmount = 0.18;			// Aggressiveness of light bleeding reduction
uniform float2		FPBias = float2(0.0, 0.0);

/////


sampler StaticShadowMapSampler1 = sampler_state
{
   Texture		= <StaticShadowMap1>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
};

sampler StaticShadowMapSampler2 = sampler_state
{
   Texture		= <StaticShadowMap2>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
};

sampler StaticShadowMapSampler3 = sampler_state
{
   Texture		= <StaticShadowMap3>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
};

sampler StaticShadowMapSampler4 = sampler_state
{
   Texture		= <StaticShadowMap4>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
};

sampler DynamicShadowMapSampler1 = sampler_state
{
   Texture		= <DynamicShadowMap1>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
};

sampler DynamicShadowMapSampler2 = sampler_state
{
   Texture		= <DynamicShadowMap2>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
};

sampler DynamicShadowMapSampler3 = sampler_state
{
   Texture		= <DynamicShadowMap3>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
};

sampler DynamicShadowMapSampler4 = sampler_state
{
   Texture		= <DynamicShadowMap4>;
   MinFilter	= ANISOTROPIC;
   MagFilter	= ANISOTROPIC;
   MipFilter	= ANISOTROPIC;
   AddressU		= CLAMP;
   AddressV		= CLAMP;
   MaxAnisotropy = 8;
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
	float lightDistance = distance(lightPosition[lightNum], position.xyz);
	float3 lightVector = normalize(lightPosition[lightNum] - position.xyz);
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

float4 CalculateSpecular(float3 pos, float specularFactor, float3 eyePosition, float3 normal, int lightNum, bool directional, out float specularShine)
{
	float4 SpecularColor = float4(lightColor[lightNum] * lightIntensity[lightNum], 1);
	float3 lightVector = (float3)0;
	specularShine = 0;

	if(directional)
	{
		lightVector = -normalize(lightDirection[lightNum]);
	}
	else
	{
		lightVector = normalize(lightPosition[lightNum] - pos);
	}
	
	float NdL = saturate(dot(lightVector, normal));

	float3 Reflect = normalize(2.0f * NdL * normal - lightVector);
	
	specularShine = pow(saturate(dot(Reflect, eyePosition)), specularFactor);

	return (SpecularColor * specularShine);
	//return float4(1, 0, 0, 1);
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

float CalcShadowCoeffVSM(float4 Pos, sampler2D shadowMapSampler, int light)
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
	
	//float2 moments = tex2Dlod( shadowMapSampler, float4(ShadowTexC, 0, 1)).xy;
	float2 moments = tex2D(shadowMapSampler, ShadowTexC).rg;

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
	float2 ParallaxTexCoord = (depthAmount * ViewDirection).xy + texCoord;
	
	return ParallaxTexCoord;
}

float3 GetRadiosityNormalMap(float3 Nn, float2 UV, float3x3 WorldMatrix, sampler2D rnmX, sampler2D rnmY, sampler2D rnmZ)
{
	float3 l_LightmapX = (tex2D(rnmX, UV)*2).xyz;
	float3 l_LightmapY = (tex2D(rnmY, UV)*2).xyz;
	float3 l_LightmapZ = (tex2D(rnmZ, UV)*2).xyz;

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
    return FPBias;
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
    ////Perhaps clamp maximum Delta here
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

float CalcShadowVarianceCascadeNum(int cascadeNum, sampler shadowMapSampler, int light, float4 Pos)
{
	float4x4 lightViewProj = CascadeShadowViewProjection[light][cascadeNum];
		
	float offset = 0.33333f * cascadeNum;
	
	float4 lightingPosition = mul(Pos, lightViewProj);
    
	float2 shadowTexCoord = lightingPosition.xy / lightingPosition.w;
	shadowTexCoord.x =  0.5f*shadowTexCoord.x + 0.5f; 
	shadowTexCoord.y = -0.5f*shadowTexCoord.y + 0.5f;

	shadowTexCoord.x = shadowTexCoord.x * 0.3333333f + offset;
	
	float RescaledDist = (lightingPosition.z / lightingPosition.w);

	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	
	float2 Moments = tex2Dlod(shadowMapSampler, float4(shadowTexCoord, 0, 1)).rg;
	Moments = Moments + GetFPBias();
	
	float ShadowContrib = ChebyshevUpperBound(Moments, RescaledDist, VSMMinVariance);
    
	if (LBREnable)
	{
		ShadowContrib = LBR(ShadowContrib);
	}
	
	return ShadowContrib;
}

float CalcShadowVarianceCascade(float4 Pos, sampler shadowMapSampler, int light, float4 vPos)
{	
	float percentAmount = 0.0f;
	
	if(vPos.z > CascadeDistances[light].z)
	{
		CascadeGroup = 2;
	}
	else if(vPos.z > CascadeDistances[light].y)
	{
		CascadeGroup = 1;

		percentAmount = (vPos.z / CascadeDistances[light].z);
	}
	else if(vPos.z > CascadeDistances[light].x)
	{
		CascadeGroup = 0;

		percentAmount = (vPos.z / CascadeDistances[light].y);
	}

	float ShadowContrib = CalcShadowVarianceCascadeNum(CascadeGroup, shadowMapSampler, light, Pos);

	return ShadowContrib;
}

float CalcShadowVariance(float4 Pos, sampler shadowMapSampler, int light, float4 vPos)
{	
	if(lightType[light] == DIRECTIONAL)
	{
		return CalcShadowVarianceCascade(Pos, shadowMapSampler, light, vPos);
	}
	else
	{
		float4 ShadowPos = mul(Pos, ShadowViewProjection[light]);
	
		// Project the texture coords and scale/offset to [0, 1].
		float2 ShadowTexC = ShadowPos.xy / ShadowPos.w;
		ShadowTexC.x =  0.5f*ShadowPos.x + 0.5f; 
		ShadowTexC.y = -0.5f*ShadowPos.y + 0.5f;


		float RescaledDist = ShadowPos.z / ShadowPos.w;
	
		float2 Moments = tex2Dlod(shadowMapSampler, float4(ShadowTexC, 0, 1)).rg;
		Moments = Moments + GetFPBias();
	
		float ShadowContrib = ChebyshevUpperBound(Moments, RescaledDist, VSMMinVariance);
    
		if (LBREnable)
		{
			ShadowContrib = LBR(ShadowContrib);
		}
	
		return ShadowContrib;
	}
}

//////////////////////////////////////

//////////////////////////////////////
//Motion Blur Functions				//
/////////////////////////////////////
float2 MotionBlurVelocity(float4 wvpPosition, float4 wPosition, bool skybox = false)
{
	float4 prevProjSpace = wvpPosition;
	float4 currentProjSpace = mul(wPosition, PrevViewProjection);

	if(skybox == true)
	{
		currentProjSpace = currentProjSpace.xyww;
	}
	
    // Convert to non-homogeneous points [-1,1] by dividing by w 
    currentProjSpace /= currentProjSpace.w;
    prevProjSpace /= prevProjSpace.w;

	// Vertex's velocity (in non-homogeneous projection space) is the position this frame minus 
    // its position last frame.  This information is stored in a texture coord.  The pixel shader 
    // will read the texture coordinate with a sampler and use it to output each pixel's velocity.
    float2 velocity = currentProjSpace - prevProjSpace;   
	
    // The velocity is now between (-2,2) so divide by 2 to get it to (-1,1)
    velocity /= 2.0f;

	return velocity;
}


//////////////////////////////////////


