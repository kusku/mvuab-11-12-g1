#define BUMP 2.4

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
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 Binormal : BINORMAL;
	float2 UV : TEXCOORD0;
};

struct TVertexOut
{
	float4 HPos : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Tangent : TEXCOORD2;
	float3 Binormal : TEXCOORD3;
};

float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_World : World;

TVertexOut mainVS(TVertexIn IN)
{
	TVertexOut l_OUT = (TVertexOut)0;
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.Normal = normalize(mul(IN.Normal, g_World));
	l_OUT.Tangent =mul(IN.Tangent.xyz, (float3x3)g_World);
	l_OUT.Binormal = mul(IN.Binormal.xyz, (float3x3)g_World);
	
	return l_OUT;
}

float4 mainPS(TVertexOut IN) : COLOR
{
	float3 Nn = normalize(IN.Normal);
	float3 Tn = normalize(IN.Tangent);
	float3 Bn = normalize(IN.Binormal);

	float3 bump = BUMP*(tex2D(g_NormalMapTextureSampler,IN.UV).rgb - float3(0.5,0.5,0.5));
	Nn = Nn + bump.x*Tn + bump.y*Bn;
	Nn = normalize(Nn);
	
	return float4(1.0, 1.0, 1.0, 1.0);
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
		
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}