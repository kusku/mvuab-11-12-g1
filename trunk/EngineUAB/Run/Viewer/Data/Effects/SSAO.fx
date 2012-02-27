#include "Samplers.h"
#include "functions.fx"

float g_SampleRadiusSSAO=0.023;
float g_DistanceScaleSSAO=0.405;
float2 g_RenderTargetSize : RENDER_TARGET_SIZE;
float4x4 g_InverseProjectionMatrix : PROJECTIONINVERSE;
float4x4 g_ProjectionMatrix : PROJECTION;

struct VertexIn
{
	float3 Pos : POSITION;
	float2 UV : TEXCOORD0;
};

struct VertexOut
{
	float4 Pos : POSITION0;
	float2 UV : TEXCOORD0;
};

VertexOut SSAOVS (VertexIn IN)
{
	VertexOut l_OUT = (VertexOut)0;
	l_OUT.Pos = float4(IN.Pos.xyz, 1);
	l_OUT.UV = IN.UV;
	
	return l_OUT;
}

float4 SSAOPS(VertexOut IN) : COLOR
{
	float4 samples[16] =
	{
		float4(0.355512, -0.709318, -0.102371, 0.0 ),
		float4(0.534186, 0.71511, -0.115167, 0.0 ),
		float4(-0.87866, 0.157139, -0.115167, 0.0 ),
		float4(0.140679, -0.475516, -0.0639818, 0.0 ),
		float4(-0.0796121, 0.158842, -0.677075, 0.0 ),
		float4(-0.0759516, -0.101676, -0.483625, 0.0 ),
		float4(0.12493, -0.0223423, -0.483625, 0.0 ),
		float4(-0.0720074, 0.243395, -0.967251, 0.0 ),
		float4(-0.207641, 0.414286, 0.187755, 0.0 ),
		float4(-0.277332, -0.371262, 0.187755, 0.0 ),
		float4(0.63864, -0.114214, 0.262857, 0.0 ),
		float4(-0.184051, 0.622119, 0.262857, 0.0 ),
		float4(0.110007, -0.219486, 0.435574, 0.0 ),
		float4(0.235085, 0.314707, 0.696918, 0.0 ),
		float4(-0.290012, 0.0518654, 0.522688, 0.0 ),
		float4(0.0975089, -0.329594, 0.609803, 0.0 )
	};
	
	float l_WidthScreenResolutionOffset=1/g_RenderTargetSize.x;
	float l_HeightScreenResolutionOffset=1/g_RenderTargetSize.y;
	float depth = tex2D(S1LinearWrapSampler, IN.UV).r;
	float3 se=GetPositionFromZDepthViewInViewCoordinates(depth, IN.UV, g_InverseProjectionMatrix);
	float4 vPositionVS = mul(float4(IN.UV.x,IN.UV.y,depth,1.0), g_InverseProjectionMatrix);
	depth = vPositionVS.z/vPositionVS.w;
	float3 randNormal = tex2D( S0LinearWrapSampler, IN.UV * 200.0 ).rgb;
	float finalColor = 0.0f;
	
	for (int i = 0; i < 16; i++)
	{
		float3 ray = reflect(samples[i].xyz,randNormal) * g_SampleRadiusSSAO;
		float4 sample = float4(se + ray, 1.0f);
		float4 ss = mul(sample, g_ProjectionMatrix);
		float2 sampleTexCoord = 0.5f * ss.xy/ss.w + float2(0.5f, 0.5f);
		sampleTexCoord.x += l_WidthScreenResolutionOffset;
		sampleTexCoord.y += l_HeightScreenResolutionOffset;
		sampleTexCoord.y=1.0-sampleTexCoord.y;
		float sampleDepth = tex2D(S1LinearWrapSampler, sampleTexCoord).r;
		vPositionVS = mul(float4(sampleTexCoord.x,sampleTexCoord.y, sampleDepth,1.0), g_InverseProjectionMatrix);
		sampleDepth=vPositionVS.z/vPositionVS.w;
		
		if (sampleDepth == 1.0)
		{
			finalColor ++;
		}
		else
		{
			//float occlusion = g_DistanceScaleSSAO* max(sampleDepth - depth, 0.0f);
			float occlusion = g_DistanceScaleSSAO* abs(sampleDepth - depth);
			finalColor += 1.0f / (1.0f + occlusion * occlusion * 0.1);
		}
	}
	return float4(finalColor/16, finalColor/16, finalColor/16, 1.0f);
}

float4 SSAOBlurPS(VertexOut IN) : COLOR
{
	float4 OUT=0;
	float l_RTWidth=g_RenderTargetSize.x;
	float l_RTHeight=g_RenderTargetSize.y;
	float2 blurDirection=float2(0.0, 1.0/l_RTHeight); //Vector Up screen
	IN.UV.x += 1.0/l_RTWidth;
	IN.UV.y += 1.0/l_RTHeight;
	float3 normal = tex2D(S1LinearWrapSampler, IN.UV).rgb;
	normal=normalize(UnpackNormal(normal));
	float color = tex2D( S0LinearWrapSampler, IN.UV).r;
	float num = 1;
	int blurSamples = 8;
	for( int i = -blurSamples/2; i <= blurSamples/2; i+=1)
	{
		float4 newTexCoord = float4(IN.UV + i * blurDirection.xy, 0, 0);
		float sample = tex2D(S0LinearWrapSampler, newTexCoord).r;
		float3 samplenormal = tex2D(S1LinearWrapSampler, newTexCoord).rgb;
		samplenormal=normalize(UnpackNormal(samplenormal));
		if (dot(samplenormal, normal) > 0.99 )
		{
			num += (blurSamples/2 - abs(i));
			color += sample * (blurSamples/2 - abs(i));
		}
	}
	
	return color / num;
}

//Technique que genera la textura de SSAO
technique SSAOTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 SSAOVS();
		PixelShader = compile ps_3_0 SSAOPS();
	}
}



//Technique que aplica Blur
technique SSAOBlurTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 SSAOVS();
		PixelShader = compile ps_3_0 SSAOBlurPS();
	}
}
	

