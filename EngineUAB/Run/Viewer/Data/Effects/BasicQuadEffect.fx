
sampler2D QuadTextureSampler : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Clamp;
   AddressV  = Clamp;
};

struct VertexShaderInput
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	output.TexCoord = input.TexCoord;
	
	output.Position = float4(input.Position, 1);
	
	return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR
{
	float4 PixColor = tex2D(QuadTextureSampler, input.TexCoord);
	
	//return float4 (1.0f, 1.0f, 1.0f, 1.0f);
	//PixColor.a = 1.0f;
	return PixColor;
}

technique BasicQuadEffect
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}