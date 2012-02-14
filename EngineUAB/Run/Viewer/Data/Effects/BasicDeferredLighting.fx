//////////////////////////////////////
//Global Constants				   //
/////////////////////////////////////

#define OMNI 0
#define DIRECTIONAL 1
#define SPOT 2

uniform float2		HalfPixel				: HALFPIXEL;

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
uniform float4x4	InvertProjection		: PROJECTIONINVERSE;

//////////////////////////////////////


//////////////////////////////////////
//Textures						   //
/////////////////////////////////////

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
	MagFilter = POINT;
	MinFilter = POINT;
	Mipfilter = POINT; 
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

//////////////////////////////////////


//////////////////////////////////////
//Structs						   //
/////////////////////////////////////

struct VertexShaderInput
{
	float3 Position : POSITION0;
	float2 TexCoord	: TexCoord0;
};

struct VertexShaderOutput
{
	float4 Position : POSITION0;
	float2 TexCoord	: TexCoord0;
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

float3 GetPositionFromZDepthViewInViewCoordinates(float ZDepthView, float2 UV)
{
	// Get the depth value for this pixel
	// Get x/w and y/w from the viewport position
	float x = UV.x * 2 - 1;
	float y = (1 - UV.y) * 2 - 1;
	float4 l_ProjectedPos = float4(x, y, ZDepthView, 1.0);
	// Transform by the inverse projection matrix
	float4 l_PositionVS = mul(l_ProjectedPos, InvertProjection);
	// Divide by w to get the view-space position
	return l_PositionVS.xyz / l_PositionVS.w;
}

float3 GetPositionFromZDepthView(float ZDepthView, float2 UV)
{
	float3 l_PositionView=GetPositionFromZDepthViewInViewCoordinates(ZDepthView, UV);
	return mul(float4(l_PositionView,1.0), InvertView).xyz;
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
//VS & PS Functions				   //
/////////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	//Basic Info
	output.Position = float4(input.Position, 1);
	output.TexCoord = input.TexCoord - HalfPixel;
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	//Get Normal From Map and unpack
	float3 normal = tex2D(NormalTextureMap, input.TexCoord).xyz;
	normal = normalize(UnpackNormal(normal));

    //Get Depth from Map
    float depthVal = tex2D(DepthTextureMap, input.TexCoord).r;

    //compute screen-space position
    //float4 position = float4(GetPositionFromZDepthView(depthVal, input.TexCoord), 1);
	float4 position = GetPosition(input.TexCoord, depthVal);
	
	float4 FinalPixelColor = (float4)0;
	
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
	
	FinalPixelColor = saturate(FinalPixelColor);
	
	return FinalPixelColor;
}

//////////////////////////////////////


//////////////////////////////////////
//Techniques					   //
/////////////////////////////////////

technique BasicDeferredLighting
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