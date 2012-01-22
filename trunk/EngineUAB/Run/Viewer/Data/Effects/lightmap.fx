sampler g_DiffuseSampler : register(s0) = sampler_state 
{
	MagFilter = Linear;
	MinFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	AddressW = Wrap;
};

sampler g_LightMapSampler : register(s1) = sampler_state 
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
	float2 UV2 : TEXCOORD1;
};

struct TVertexOut
{
	float4 HPos : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
};

float4x4 g_WorldViewProj : WorldViewProjection;
float4x4 g_WorldMatrix : World;

TVertexOut mainVS(TVertexIn IN)
{
	TVertexOut l_OUT = (TVertexOut)0;
	l_OUT.HPos = mul(float4(IN.Pos.xyz, 1.0), g_WorldViewProj);
	l_OUT.UV = IN.UV;
	l_OUT.UV2 = IN.UV2;
	
	return l_OUT;
}

float4 mainPS(TVertexOut IN) : COLOR 
{
	float4 l_AlbedoDiffuse = tex2D(g_DiffuseSampler, IN.UV);
	float4 l_AlbedoLightMap = tex2D(g_LightMapSampler, IN.UV);
	
	float4 l_Color = l_AlbedoDiffuse * l_AlbedoLightMap;
	
	return l_Color;
}

technique LightMapping
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