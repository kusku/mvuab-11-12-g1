#define MAX_LIGHTS 3

sampler g_DiffuseSampler : register(s0) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler g_NormalMapTextureSampler : register(s1) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

struct TVertexIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
};

struct TVertexOut
{
	float4 HPos : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 Pos : TEXCOORD2;
	float3 EyePosition : TEXCOORD3;
};

struct TVertexInNM
{
	float3 Pos : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
	float2 UV : TEXCOORD0;
};

struct TVertexOutNM
{
	float4 HPos : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float4 Pos : TEXCOORD2;
	float3 EyePosition : TEXCOORD3;
	float3 Tangent : TEXCOORD4;
	float3 Binormal : TEXCOORD5;
};

int		g_NumLights : Num_Lights;
bool	g_EnabledLights[MAX_LIGHTS] : Lights_Enabled;
int		g_TypeLights[MAX_LIGHTS] : Lights_Type;
float3 	g_PositionLights[MAX_LIGHTS] : Lights_Position;
float3 	g_DirectionLights[MAX_LIGHTS] : Lights_Direction;
float3 	g_ColorLights[MAX_LIGHTS] : Lights_Color;
float 	g_StartAttLights[MAX_LIGHTS] : Lights_StartAtt;
float 	g_EndAttLights[MAX_LIGHTS] : Lights_EndAtt;
float 	g_AngleLights[MAX_LIGHTS] : Lights_Angle;
float  	g_FallOffLights[MAX_LIGHTS] : Lights_FallOff;

float3	g_AmbientColorLight = float3(0.389, 0.763, 0.465);
float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_WorldMatrix : World;
float3	 g_CameraPosition : Camera_Position;
float	g_Bump = 2.4;

TVertexOut mainVS (TVertexIn IN)
{
	TVertexOut l_OUT = (TVertexOut)0;
	
	float3 WorldSpacePosition = mul(IN.Pos.xyz, g_WorldMatrix);
	
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.Normal = normalize(mul(IN.Normal, g_WorldMatrix));
	l_OUT.Pos = mul(float4(IN.Pos.xyz,1.0), g_WorldMatrix);
	l_OUT.EyePosition = g_CameraPosition - WorldSpacePosition;
	return l_OUT;
}

float4 mainPS (TVertexOut IN) : COLOR
{
	float3 Nn = normalize(IN.Normal);
	IN.EyePosition = normalize(IN.EyePosition);

	float3 l_DiffuseContrib = (float3)0;
	float l_SpecularContrib = (float)0;
	float4 l_Amount = tex2D(g_DiffuseSampler, IN.UV);
	
	for(int i=0; i < g_NumLights; ++i)
	{
		if( g_EnabledLights[i] )
		{
			if( g_TypeLights[i] == 0 ) //OMNI
			{
				float DistanceToLight = distance(g_PositionLights[i], IN.Pos);
				float Attenuation = 1 - saturate( (DistanceToLight - g_StartAttLights[i]) / (g_EndAttLights[i] - g_StartAttLights[i]) );
				float NDotL = saturate(dot( g_PositionLights[i] - (float3)IN.Pos.xyz, Nn));
				l_DiffuseContrib += (g_ColorLights[i] * NDotL) * Attenuation;
				
				float3 Reflect = normalize(2.0f * NDotL * Nn - (g_PositionLights[i] - IN.Pos.xyz));
				l_SpecularContrib += pow(saturate(dot(Reflect, IN.EyePosition)), 20);
			}
			else if( g_TypeLights[i] == 1) //DIRECTIONAL
			{
				float PixelDirectionalLight = saturate(dot(-g_DirectionLights[i], Nn));
				l_DiffuseContrib += g_ColorLights[i] * PixelDirectionalLight;
				
				float3 Reflect = normalize(2.0f * PixelDirectionalLight * Nn - -g_DirectionLights[i]);
				l_SpecularContrib += pow(saturate(dot(Reflect, IN.EyePosition)), 20);
			}
		}
	}
	
	return float4((g_AmbientColorLight + l_DiffuseContrib ) * ( l_SpecularContrib + l_Amount.rgb ), l_Amount.a);
}

TVertexOutNM mainVSNormalMap (TVertexInNM IN)
{
	TVertexOutNM l_OUT = (TVertexOutNM)0;
	
	float3 WorldSpacePosition = mul(IN.Pos.xyz, g_WorldMatrix);
	
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.Normal = normalize(mul(IN.Normal, g_WorldMatrix));
	l_OUT.Tangent =mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix);
	l_OUT.Binormal = mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix);
	l_OUT.Pos = mul(float4(IN.Pos.xyz,1.0), g_WorldMatrix);
	l_OUT.EyePosition = g_CameraPosition - WorldSpacePosition;
	return l_OUT;
}

float4 mainPSNormalMap (TVertexOutNM IN) : COLOR
{
	float3 Nn = normalize(IN.Normal);
	float3 Tn = normalize(IN.Tangent);
	float3 Bn = normalize(IN.Binormal);
	IN.EyePosition = normalize(IN.EyePosition);

	float3 l_DiffuseContrib = (float3)0;
	float l_SpecularContrib = (float)0;
	float4 l_Amount = tex2D(g_DiffuseSampler, IN.UV);
	
	float3 bump=g_Bump*(tex2D(g_NormalMapTextureSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);
	
	for(int i=0; i < g_NumLights; ++i)
	{
		if( g_EnabledLights[i] )
		{
			if( g_TypeLights[i] == 0 ) //OMNI
			{
				float DistanceToLight = distance(g_PositionLights[i], IN.Pos);
				float Attenuation = 1 - saturate( (DistanceToLight - g_StartAttLights[i]) / (g_EndAttLights[i] - g_StartAttLights[i]) );
				float NDotL = saturate(dot( g_PositionLights[i] - (float3)IN.Pos.xyz, Nn));
				l_DiffuseContrib += (g_ColorLights[i] * NDotL) * Attenuation;
				
				float3 Reflect = normalize(2.0f * NDotL * Nn - (g_PositionLights[i] - IN.Pos.xyz));
				l_SpecularContrib += pow(saturate(dot(Reflect, IN.EyePosition)), 20);
			}
			else if( g_TypeLights[i] == 1) //DIRECTIONAL
			{
				float PixelDirectionalLight = saturate(dot(-g_DirectionLights[i], Nn));
				l_DiffuseContrib += g_ColorLights[i] * PixelDirectionalLight;
				
				float3 Reflect = normalize(2.0f * PixelDirectionalLight * Nn - g_DirectionLights[i]);
				l_SpecularContrib += pow(saturate(dot(Reflect, IN.EyePosition)), 20);
			}
		}
	}
	
	//return float4(Nn, 1.0);
	return float4((g_AmbientColorLight + l_DiffuseContrib ) * ( l_SpecularContrib + l_Amount.rgb ), l_Amount.a);
}

technique NormalTexture
{
	pass p0 
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		
		//Deshabilitamos el alphablend
		AlphaBlendEnable = false;
		
		//Tipo de culling que queremos utilizar
		CullMode = CCW;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}

technique NormalMapping
{
	pass p0 
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		
		//Deshabilitamos el alphablend
		AlphaBlendEnable = false;
		
		//Tipo de culling que queremos utilizar
		CullMode = CCW;
		
		VertexShader = compile vs_3_0 mainVSNormalMap();
		PixelShader = compile ps_3_0 mainPSNormalMap();
	}
}
