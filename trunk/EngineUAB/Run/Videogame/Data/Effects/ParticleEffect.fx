
#include "functions.fx"

///////////////////////////////////

uniform extern float2		PSP_ViewportScale			: PSP_VIEWPORT_SCALE;

uniform extern float		PSP_CurrentTime				: PSP_CURRENT_TIME;

uniform extern float		PSP_Duration				: PSP_DURATION;
uniform extern float		PSP_DurationRandomness		: PSP_DURATION_RANDOMNESS;
uniform extern float3		PSP_Gravity					: PSP_GRAVITY;
uniform extern float		PSP_EndVelocity				: PSP_END_VELOCITY;
uniform extern float4		PSP_MinColor				: PSP_MIN_COLOR;
uniform extern float4		PSP_MaxColor				: PSP_MAX_COLOR;

uniform extern float2		PSP_RotateSpeed				: PSP_ROTATE_SPEED;
uniform extern float2		PSP_StartSize				: PSP_START_SIZE;
uniform extern float2		PSP_EndSize					: PSP_END_SIZE;

uniform extern Texture2D	PSP_ParticleTexture			: PSP_PARTICLE_TEXTURE;

///////////////////////////////////

sampler ParticleTextureSampler = sampler_state
{
    Texture		= < PSP_ParticleTexture >;
    MinFilter	= Linear;
    MagFilter	= Linear;
    MipFilter	= Point;
    AddressU	= Clamp;
    AddressV	= Clamp;
};

struct VertexShaderInput
{
    float2	Corner		: POSITION0;
    float3	Position	: POSITION1;
    float3	Velocity	: NORMAL0;
    float4	Random		: COLOR0;
    float	Time		: TEXCOORD0;
};

struct VertexShaderOutput
{
    float4	Position			: POSITION0;
    float4	Color				: COLOR0;
    float2	TextureCoordinate	: COLOR1;
	float2	DepthInt			: NORMAL0;
	float	FogLerp				: NORMAL1;
//	float2	VelocityMB			: NORMAL2;
};

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 DepthRT		: COLOR1;
	float4 MotionBlurRT	: COLOR2;
	float4 DyingColorRT	: COLOR3;
};

////////////////////////////////////////////////////////////////////

//////////////////////////////////
// Helper Functions
//////////////////////////////////

float4 ComputeParticlePosition(float3 position, float3 velocity,
                               float age, float normalizedAge)
{
    float startVelocity = length(velocity);

    float endVelocity = startVelocity * PSP_EndVelocity;

    float velocityIntegral = startVelocity * normalizedAge +
                             (endVelocity - startVelocity) * normalizedAge *
                                                             normalizedAge / 2;

    position += normalize(velocity) * velocityIntegral * PSP_Duration;

    position += PSP_Gravity * age * normalizedAge;

    return mul(mul(float4(position, 1), ViewMatrix), ProjectionMatrix);
}

float ComputeParticleSize(float randomValue, float normalizedAge)
{
    float startSize = lerp(PSP_StartSize.x, PSP_StartSize.y, randomValue);
    float endSize = lerp(PSP_EndSize.x, PSP_EndSize.y, randomValue);

    float size = lerp(startSize, endSize, normalizedAge);

    return size * ProjectionMatrix._m11;
}

float4 ComputeParticleColor(float4 projectedPosition,
                            float randomValue, float normalizedAge)
{
    float4 color = lerp(PSP_MinColor, PSP_MaxColor, randomValue);

    color.a *= normalizedAge * (1-normalizedAge) * (1-normalizedAge) * 6.7;

    return color;
}

float2x2 ComputeParticleRotation(float randomValue, float age)
{
    float rotateSpeed = lerp(PSP_RotateSpeed.x, PSP_RotateSpeed.y, randomValue);
    float rotation = rotateSpeed * age;

    float c = cos(rotation);
    float s = sin(rotation);

    return float2x2(c, -s, s, c);
}

//////////////////////////////////
// Vertex Shaders
//////////////////////////////////

VertexShaderOutput ParticleVertexShader(VertexShaderInput input)
{
    VertexShaderOutput output = (VertexShaderOutput)0;

    float age = PSP_CurrentTime - input.Time;

    age *= 1 + input.Random.x * PSP_DurationRandomness;

    float normalizedAge = saturate(age / PSP_Duration);

    output.Position = ComputeParticlePosition(input.Position, input.Velocity,
                                              age, normalizedAge);

    float size = ComputeParticleSize(input.Random.y, normalizedAge);

    float2x2 rotation = ComputeParticleRotation(input.Random.w, age);

    output.Position.xy += mul(input.Corner, rotation) * size * PSP_ViewportScale;

    output.Color = ComputeParticleColor(output.Position, input.Random.z, normalizedAge);

    output.TextureCoordinate = (input.Corner + 1) / 2;

	//Get World Space
	float4 WorldSpacePosition = mul(output.Position, InvertViewProjection);
	
	if(FogEnable == true)
	{
		output.FogLerp = saturate( (distance(WorldSpacePosition, (CameraPosition - WorldSpacePosition.xyz) ) - FogStart) / FogRange);
	}

	/////////////
	//Depth Map
	////////////
	output.DepthInt = output.Position.zw;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
//	output.VelocityMB = MotionBlurVelocity(output.Position, WorldSpacePosition);

	//End Motion Blur
	///////////////

    return output;
}

//////////////////////////////////
// Pixel Shaders
//////////////////////////////////

PixelShaderOutput ParticlePixelShader(VertexShaderOutput input)
{
	PixelShaderOutput output = (PixelShaderOutput)0;

    float4 PixEndColor = tex2D(ParticleTextureSampler, input.TextureCoordinate) * input.Color;
	
	if(FogEnable == true)
	{
		PixEndColor.xyz = lerp(PixEndColor.xyz, FogColor, input.FogLerp);
	}

	output.DiffuseRT = PixEndColor;
	
	/////////////
	//Depth Map
	////////////
	output.DepthRT.r = input.DepthInt.x / input.DepthInt.y;
	
	//End Depth Map
	////////////

	///////////////
	//Motion Blur
	///////////////
//	output.MotionBlurRT = float4(input.VelocityMB, 1.0f, 1.0f);

	//End Motion Blur
	///////////////

	return output;
}

//////////////////////////////////
// Techniques
//////////////////////////////////

technique ParticleTechnique
{
    pass Pass0
    {
        VertexShader = compile vs_3_0 ParticleVertexShader();
        PixelShader = compile ps_3_0 ParticlePixelShader();
    }
}
