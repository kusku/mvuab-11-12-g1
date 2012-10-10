#include "functions.fx"

sampler2D DiffuseTextureMap : register( s0 ) = sampler_state
{
   MinFilter = Linear;
   MagFilter = Linear;
   MipFilter = Linear;   
   AddressU  = Wrap;
   AddressV  = Wrap;
};

struct VertexShaderInput
{
    float3 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

struct VertexShaderInstanceInput
{
    float3 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
	float4 Mat1		: TEXCOORD1;
	float4 Mat2		: TEXCOORD2;
	float4 Mat3		: TEXCOORD3;
	float4 Mat4		: TEXCOORD4;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	//float3 PosView	: NORMAL0;
	float2 TexCoord : TEXCOORD0;
	float2 PosView2	: TEXCOORD1;
};

VertexShaderOutput VertexShaderInstanceFunction(VertexShaderInstanceInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;

	float4x4 WorldInstance = (float4x4)0;

	WorldInstance[0] = input.Mat1;
	WorldInstance[1] = input.Mat2;
	WorldInstance[2] = input.Mat3;
	WorldInstance[3] = input.Mat4;
	
    output.Position = mul(mul(float4(input.Position, 1), WorldInstance), ShadowViewProjection[0]);
    //output.PosView  = mul(float4(input.Position, 1), ShadowWorldView);
	output.PosView2.xy	= output.Position.zw;
	
	output.TexCoord = input.TexCoord;

    return output;
}

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
    output.Position = mul(float4(input.Position, 1), ShadowWorldViewProjection);
    //output.PosView  = mul(float4(input.Position, 1), ShadowWorldView);
	output.PosView2.xy	= output.Position.zw;
	
	output.TexCoord = input.TexCoord;

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input, uniform bool clipAlpha) : COLOR0
{
	if(clipAlpha == true)
	{
		float alfa = tex2D(DiffuseTextureMap, input.TexCoord).a;
	
		clip(alfa - 0.1f);
	}

	////////////////////////////////////////
	////////////////////////////////////////
	////////////////////////////////////////

	//float d = input.PosView2.x / input.PosView2.y;
	//float moment1 = d;
	//float moment2 = d * d;

	//// Adjusting moments (this is sort of bias per pixel) using partial derivative
	//float dx = ddx(d);
	//float dy = ddy(d);
	//moment2 += 0.25 * (dx * dx + dy * dy) ;
	
	//return float4(moment1, moment2, 0, 1.0f);
	
	////////////////////////////////////////
	////////////////////////////////////////
	////////////////////////////////////////

	//float Depth = RescaleDistToLight(length(input.PosView));
	float Depth = input.PosView2.x / input.PosView2.y;
	
	float2 Moments = ComputeMoments(Depth) - GetFPBias();
    
	float4 ret = float4(Moments.x, Moments.y, 0, 1);
	
	return ret;
}

technique BasicBuildShadowMap 
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true);
	}
}

technique BasicBuildShadowMapNoClip
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false);
	}
}

//Instance
technique BasicBuildShadowMapInstance
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(true);
	}
}

technique BasicBuildShadowMapInstanceNoClip
{
	pass p0 
	{
		VertexShader = compile vs_3_0 VertexShaderInstanceFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction(false);
	}
}