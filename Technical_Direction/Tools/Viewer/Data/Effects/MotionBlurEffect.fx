//////////////////////////////////////
//Includes						   //
/////////////////////////////////////
#include "functions.fx"

////////////////////////////////////////////////////////////////////

sampler2D DiffuseTextureMap : register( s0 ) = sampler_state
{
	MinFilter = Point;
	MagFilter = Point;

	AddressU  = Clamp;
	AddressV  = Clamp;
};

sampler2D LastFramePixelVelSampler : register( s1 ) = sampler_state
{
	MinFilter = Point;
	MagFilter = Point;

    AddressU = Clamp;
    AddressV = Clamp;
};

sampler2D CurFramePixelVelSampler : register( s2 ) = sampler_state
{
	MinFilter = Point;
	MagFilter = Point;

    AddressU = Clamp;
    AddressV = Clamp;
};

////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////

static const float NumberOfPostProcessSamples = 12.0f;
uniform float PixelBlurConst : PIXEL_MOTION_BLUR = 0.15f;

////////////////////////////////////////////////////////////////////

//////////////////////////////////
// Vertex Shaders
//////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	
	output.TexCoord = input.TexCoord;
	
	output.Position = float4(input.Position, 1);
	
	return output;
}

//////////////////////////////////
// Pixel Shaders
//////////////////////////////////

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{	
	float2 pixelVelocity = (float2)0;
    
    // Get this pixel's current velocity and this pixel's last frame velocity
    // The velocity is stored in .r & .g channels
    float4 curFramePixelVelocity = tex2D(CurFramePixelVelSampler, input.TexCoord);
    float4 lastFramePixelVelocity = tex2D(LastFramePixelVelSampler, input.TexCoord);

    // If this pixel's current velocity is zero, then use its last frame velocity
    // otherwise use its current velocity.  We don't want to add them because then 
    // you would get double the current velocity in the center.  
    // If you just use the current velocity, then it won't blur where the object 
    // was last frame because the current velocity at that point would be 0.  Instead 
    // you could do a filter to find if any neighbors are non-zero, but that requires a lot 
    // of texture lookups which are limited and also may not work if the object moved too 
    // far, but could be done multi-pass.
    float curVelocitySqMag = curFramePixelVelocity.r * curFramePixelVelocity.r +
                             curFramePixelVelocity.g * curFramePixelVelocity.g;

    float lastVelocitySqMag = lastFramePixelVelocity.r * lastFramePixelVelocity.r +
                              lastFramePixelVelocity.g * lastFramePixelVelocity.g;
                                   
    if( lastVelocitySqMag > curVelocitySqMag )
    {
        pixelVelocity.x =  lastFramePixelVelocity.r * PixelBlurConst;   
        pixelVelocity.y = -lastFramePixelVelocity.g * PixelBlurConst;
    }
    else
    {
        pixelVelocity.x =  curFramePixelVelocity.r * PixelBlurConst;   
        pixelVelocity.y = -curFramePixelVelocity.g * PixelBlurConst;    
    }
    
    // For each sample, sum up each sample's color in "Blurred" and then divide
    // to average the color after all the samples are added.
    float3 Blurred = 0;    
    for(float i = 0; i < NumberOfPostProcessSamples; i++)
    {   
        // Sample texture in a new spot based on pixelVelocity vector 
        // and average it with the other samples        
        float2 lookup = pixelVelocity * i / NumberOfPostProcessSamples + input.TexCoord;
        
        // Lookup the color at this new spot
        float4 Current = tex2D(DiffuseTextureMap, lookup);
        
        // Add it with the other samples
        Blurred += Current.rgb;
    }
    
    // Return the average color of all the samples
    //return tex2D(DiffuseTextureMap, input.TexCoord);
	return float4(Blurred / NumberOfPostProcessSamples, 1.0f);
}

//////////////////////////////////
// Techniques
//////////////////////////////////

technique MotionBlurTechnique
{
	pass p0
	{		
		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}