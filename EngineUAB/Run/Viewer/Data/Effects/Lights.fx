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
};

struct TVertexNormalMapIn
{
	float3 Pos : POSITION;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
	float2 UV : TEXCOORD0;
};

struct TVertexNormalMapOut
{
	float4 HPos : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Tangent : TEXCOORD2;
	float3 Binormal : TEXCOORD3;
	float4 Pos : TEXCOORD4;
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
//float3	 g_AmbientColorLight = float3(1.0, 1.0, 1.0);
float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_WorldMatrix : World;
float3	 g_CameraPosition : Camera_Position;
float	g_Bump = 2.4;

float3 GetDiffuseContrib(int _Light, float3 _Pos, float3 _Nn, float3 _PosLight, float3 _Direction, float _Angle, float3 _Color)
{
	if( _Light == 0 ) //Omni
	{
		float3 Ln = normalize(_Pos - _PosLight);
		return (dot(-Ln, _Nn) > 0 ? 1.0 : 0.0) * _Color;
	}
	else if( _Light == 1 ) //Directional
	{
		return saturate(dot(-_Direction, _Nn)) * _Color;
	}
	else if( _Light == 2 ) //Spot
	{
		float3 Ln = normalize(_Pos - _PosLight);
		if( dot(-_Direction, Ln) <= cos(_Angle/2) )
		{
			return (dot(Ln, _Nn) < 0 ? 1.0 : 0.0) * _Color;	
		}
		else
		{
			return float3(0.0, 0.0, 0.0);
		}

		return float3(0.0, 0.0, 0.0);
	}
	else
	{
		return float3(1.0, 1.0, 1.0);
	}
	
	return float3(1.0, 1.0, 1.0);
}

float3 GetSpecularContrib(float3 _Nn, float3 _Pos, float3 _LightPos, float3 _Color, float3 _SpecularFactor)
{	
	float3 NDotL = saturate(dot(_Nn, -(_LightPos - _Pos)));
	float3 Reflect = normalize(2.0f * NDotL * _Nn + (_LightPos - _Pos));
	float SpecularShine = pow(saturate(dot(Reflect, normalize(g_CameraPosition - _Pos))), _SpecularFactor); 
	
	return SpecularShine * _Color;
}

TVertexOut mainVS(TVertexIn IN)
{
	TVertexOut l_OUT = (TVertexOut)0;
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.Normal = normalize(mul(IN.Normal, g_WorldMatrix));
	l_OUT.Pos = mul(float4(IN.Pos.xyz,1.0), g_WorldMatrix);
	
	return l_OUT;
}

float4 mainPS(TVertexOut IN) : COLOR 
{
	float3 Nn = normalize(IN.Normal);
	float4 l_Albedo = tex2D(g_DiffuseSampler, IN.UV);
	
	float3 l_DiffuseContrib = (float3)0;
	float3 l_SpecularContrib = (float3)0;
	
	for(int i=0; i < g_NumLights; ++i)
	{
		if( g_EnabledLights[i] )
		{		
			l_DiffuseContrib += GetDiffuseContrib(g_TypeLights[i], IN.Pos, Nn, g_PositionLights[i], 
					g_DirectionLights[i], g_AngleLights[i], g_ColorLights[i] );
					
			l_SpecularContrib += GetSpecularContrib(Nn, IN.Pos, g_PositionLights[i], g_ColorLights[i], 50);	
		}
	}
	
	//return float4(l_SpecularContrib, l_Albedo.a);
	return float4((g_AmbientColorLight + l_DiffuseContrib) * ( l_SpecularContrib + l_Albedo.rgb ), l_Albedo.a);
	return float4((g_AmbientColorLight + l_DiffuseContrib) * l_Albedo.rgb + l_SpecularContrib, l_Albedo.a);
}

TVertexNormalMapOut mainVSNormalMap(TVertexNormalMapIn IN)
{
	TVertexNormalMapOut l_OUT = (TVertexNormalMapOut)0;
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.Normal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	l_OUT.Tangent =mul(IN.Tangent.xyz, (float3x3)g_WorldMatrix);
	l_OUT.Binormal = mul(IN.Binormal.xyz, (float3x3)g_WorldMatrix);
	l_OUT.Pos = mul(float4(IN.Pos.xyz,1.0), g_WorldMatrix);
	
	return l_OUT;
}

float4 mainPSNormalMap(TVertexNormalMapOut IN) : COLOR
{
	float3 Nn = normalize(IN.Normal);
	float3 Tn=normalize(IN.Tangent);
	float3 Bn=normalize(IN.Binormal);
	float4 l_Albedo = tex2D(g_DiffuseSampler, IN.UV);
	
	/*float3 bump=g_Bump*(tex2D(g_NormalMapTextureSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);*/

	float3 l_DiffuseContrib = (float3)0;
	float3 l_SpecularContrib = (float3)0;
	
	for(int i=0; i < g_NumLights; ++i)
	{
		if( g_EnabledLights[i] )
		{		
			l_DiffuseContrib += GetDiffuseContrib(g_TypeLights[i], IN.Pos, Nn, g_PositionLights[i], 
					g_DirectionLights[i], g_AngleLights[i], g_ColorLights[i] );
					
			l_SpecularContrib += GetSpecularContrib(Nn, IN.Pos, g_PositionLights[i], g_ColorLights[i], 50);	
		}
	}
	
	return float4((g_AmbientColorLight + l_DiffuseContrib) * ( l_SpecularContrib + l_Albedo.rgb ), l_Albedo.a);
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
