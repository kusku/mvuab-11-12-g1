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

float g_SpecularFactor
<
string UIName = "SpecularPowerFactor";
string UIWidget = "slider";
float uimin = 0.0;
float uimax = 200.0;
float uistep = 1.0;
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

float4x4 WorldViewProj : WorldViewProjection;
float4x4 g_WorldMatrix : World;
float4x4 g_ViewInverseMatrix : ViewInverse;

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

TVertexOut mainVS(TVertexIn IN)
{
	TVertexOut l_OUT = (TVertexOut)0;
	
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), WorldViewProj);
	l_OUT.Normal = normalize(mul(IN.Normal, (float3x3)g_WorldMatrix));
	l_OUT.UV = IN.UV;
	l_OUT.Pos = mul(float4(IN.Pos.xyz,1.0), g_WorldMatrix);
	
	return l_OUT;
}

float4 mainPS(TVertexOut IN) : COLOR
{
	float3 Nn = normalize(IN.Normal); //Nn = Normal Normalized
	float3 l_DiffuseContrib = saturate(dot(-g_DirectionalLight, Nn))*g_ColorDirectionalLight;
	
	float4 l_Albedo = tex2D(g_DiffuseSampler, IN.UV);
	
	float3 l_CameraPosition = g_ViewInverseMatrix[3].xyz;
	float3 Hn = normalize(normalize(l_CameraPosition - IN.Pos) - g_DirectionalLight);
	float3 l_SpecularContrib = pow(saturate(dot(Nn, Hn)), g_SpecularFactor);
	
	//return float4(IN.Normal, 1.0);
	return float4((l_DiffuseContrib + g_AmbientLight)*l_Albedo.rgb + l_SpecularContrib*g_ColorDirectionalLight, l_Albedo.a);
	//return float4(1.0, 0.0, 0.0, 1.0);
}

technique UABTechnique
{
	pass p0
	{
		//CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
