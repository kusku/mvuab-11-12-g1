/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/
float3 g_DirectionalLight : DIRECTION
<
	string Object = "DirectionalLight";
	string Space = "World";
>;

float3 g_ColorDirectionalLight : COLOR
<
	string Object = "DirectionalLight";
>;

float3 g_OmniLight : POSITION
<
	string Object = "OmniLight";
	string Space = "World";
>;

float3 g_ColorOmniLight : COLOR
<
	string Object = "OmniLight";
>;

float g_StartAttOmniLight : CONSTANTATTENUATION
<
	string Object = "OmniLight";
>;

float g_EndAttOmniLight : LINEARATTENUATION
<
	string Object = "OmniLight";
>;

float3 g_SpotLight : POSITION
<
	string Object = "SpotLight";
	string Space = "World";
>;

float3 g_DirectionSpotLight : DIRECTION
<
	string Object = "SpotLight";
>;

float3 g_ColorSpotLight : COLOR
<
	string Object = "SpotLight";
>;

float g_AngleSpotLight : FALLOFFANGLE
<
	string Object = "SpotLight";
>;

float g_StartAttSpotLight : CONSTANTATTENUATION
<
	string Object = "SpotLight";
>;

float g_EndAttSpotLight : LINEARATTENUATION
<
	string Object = "SpotLight";
>;

texture g_DiffuseTexture : DIFFUSEMAP
<
	string UIName = "DiffuseMap";
	string ResourceType = "2D";
>;

float3 g_AmbientLight : AMBIENT
<
	string UIName = "AmbientLight";
	string UIWidget = "Diffuse";
>;

sampler g_DiffuseSampler = sampler_state 
{
	Texture = <g_DiffuseTexture>;
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

float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_WorldMatrix : World;

float3 GetDiffuseContrib(int _Light, float3 _Pos, float3 _Nn, float3 _PosLight, float3 _Direction, float _Angle, float3 _Color)
{
	if(_Light == 0) //Directional
	{
		return saturate(dot(-_Direction, _Nn)) * _Color;
	}
	else if(_Light == 1) //Omni
	{
		float3 Ln = normalize(_PosLight - _Pos);
		return (dot(Ln, _Nn) > 0 ? 1.0 : 0.0) * _Color;
	}
	else if(_Light == 2) //Spot
	{
		float3 Ln = normalize(_Pos - _PosLight);
		if( dot(_Direction, Ln) > cos(_Angle/2))
		{
			return (dot(Ln, _Nn) < 0 ? 1.0 : 0.0) * _Color;	
		}
		else
		{
			return float3(0.0, 0.0, 0.0);
		}
	}
	return float3(1.0, 1.0, 1.0);
}

TVertexOut mainVS(TVertexIn IN)
{
	TVertexOut l_OUT = (TVertexOut)0;
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.Normal = normalize(mul(IN.Normal, (float3x3)g_WorldMatrix));
	l_OUT.Pos = mul(float4(IN.Pos.xyz,1.0), g_WorldMatrix);
	
	return l_OUT;
}

float4 mainPS(TVertexOut IN) : COLOR 
{
	float3 Nn = normalize(IN.Normal);
	float4 l_Albedo = tex2D(g_DiffuseSampler, IN.UV);
	//float3 l_DiffuseContrib = GetDiffuseContrib(0, IN.Pos, Nn, float3(0.0, 0.0, 0.0), g_DirectionalLight, 0.0, g_ColorDirectionalLight);
	//float3 l_DiffuseContrib = GetDiffuseContrib(1, IN.Pos, Nn,  g_OmniLight, float3(0.0, 0.0, 0.0), 0.0, g_ColorOmniLight); 
	float3 l_DiffuseContrib = GetDiffuseContrib(2, IN.Pos, Nn, g_SpotLight, g_DirectionSpotLight, g_AngleSpotLight, g_ColorSpotLight);
	return float4((g_AmbientLight + l_DiffuseContrib) * l_Albedo.rgb, l_Albedo.a);
}

technique technique0 {
	pass p0 {
		//CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
