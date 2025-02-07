#include "Samplers.h"

/*=========================================
				DEFINES
=========================================*/
#define FXAA_SRGB_ROP 0
#define FXAA_PRESET 3

#define int2 float2
#define FxaaInt2 float2
#define FxaaFloat2 float2
#define FxaaFloat3 float3
#define FxaaFloat4 float4
#define FxaaBool2Float(a) (a)
#define FxaaPow3(x, y) pow(x, y)
#define FxaaSel3(f, t, b) ((f)*(!b) + (t)*(b))
#define FxaaTex sampler
#define FxaaToFloat3(a) FxaaFloat3((a), (a), (a))

/*-------------------------------------------------------------------------- */
#if (FXAA_PRESET == 1)
    #define FXAA_EDGE_THRESHOLD      (1.0/8.0)
    #define FXAA_EDGE_THRESHOLD_MIN  (1.0/16.0)
    #define FXAA_SEARCH_STEPS        4
    #define FXAA_SEARCH_ACCELERATION 3
    #define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
    #define FXAA_SUBPIX              1
    #define FXAA_SUBPIX_FASTER       0
    #define FXAA_SUBPIX_CAP          (3.0/4.0)
    #define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*-------------------------------------------------------------------------- */
#if (FXAA_PRESET == 2)
    #define FXAA_EDGE_THRESHOLD      (1.0/8.0)
    #define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
    #define FXAA_SEARCH_STEPS        8
    #define FXAA_SEARCH_ACCELERATION 2
    #define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
    #define FXAA_SUBPIX              1
    #define FXAA_SUBPIX_FASTER       0
    #define FXAA_SUBPIX_CAP          (3.0/4.0)
    #define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*-------------------------------------------------------------------------- */
#if (FXAA_PRESET == 3)
    #define FXAA_EDGE_THRESHOLD      (1.0/8.0)
    #define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
    #define FXAA_SEARCH_STEPS        16
    #define FXAA_SEARCH_ACCELERATION 1
    #define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
    #define FXAA_SUBPIX              1
    #define FXAA_SUBPIX_FASTER       0
    #define FXAA_SUBPIX_CAP          (3.0/4.0)
    #define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*-------------------------------------------------------------------------- */
#if (FXAA_PRESET == 4)
    #define FXAA_EDGE_THRESHOLD      (1.0/8.0)
    #define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
    #define FXAA_SEARCH_STEPS        24
    #define FXAA_SEARCH_ACCELERATION 1
    #define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
    #define FXAA_SUBPIX              1
    #define FXAA_SUBPIX_FASTER       0
    #define FXAA_SUBPIX_CAP          (3.0/4.0)
    #define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*-------------------------------------------------------------------------- */
#if (FXAA_PRESET == 5)
    #define FXAA_EDGE_THRESHOLD      (1.0/8.0)
    #define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
    #define FXAA_SEARCH_STEPS        32
    #define FXAA_SEARCH_ACCELERATION 1
    #define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
    #define FXAA_SUBPIX              1
    #define FXAA_SUBPIX_FASTER       0
    #define FXAA_SUBPIX_CAP          (3.0/4.0)
    #define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*-------------------------------------------------------------------------- */

#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))

float2 g_RenderTargetSize : RENDER_TARGET_SIZE;

/*=========================================
				STRUCTS
=========================================*/

struct VertexShaderInput
{
    float3 Position : POSITION0;
    float2 UV : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float2 UV : TEXCOORD0;
};

/*=========================================
				HELPERS
=========================================*/

float4 FxaaTexLod0(FxaaTex tex, float2 pos) {
	return tex2Dlod(tex, float4(pos.xy, 0.0, 0.0)); 
}

float4 FxaaTexGrad(FxaaTex tex, float2 pos, float2 grad) {
	return tex2Dgrad(tex, pos.xy, grad, grad);   
}

float4 FxaaTexOff(FxaaTex tex, float2 pos, int2 off, float2 rcpFrame) {
	return tex2Dlod(tex, float4(pos.xy + (off * rcpFrame), 0, 0)); 
}

float FxaaLuma(float3 rgb) {
    return rgb.y * (0.587/0.299) + rgb.x; 
} 

float3 FxaaLerp3(float3 a, float3 b, float amountOfA) {
    return (FxaaToFloat3(-amountOfA) * b) + 
        ((a * FxaaToFloat3(amountOfA)) + b); 
} 

float3 FxaaFilterReturn(float3 rgb) {
    #if FXAA_SRGB_ROP
        // Do sRGB encoded value to linear conversion.
        return FxaaSel3(
            rgb * FxaaToFloat3(1.0/12.92), 
            FxaaPow3(
                rgb * FxaaToFloat3(1.0/1.055) + FxaaToFloat3(0.055/1.055), 
                FxaaToFloat3(2.4)),
            rgb > FxaaToFloat3(0.04045)); 
    #else
        return rgb;
    #endif
}

/*=========================================
			  PIXEL SHADER
=========================================*/

float3 FxaaPixelShader( float2 pos, FxaaTex tex, float2 rcpFrame )
{
	float3 rgbN = FxaaTexOff(tex, pos.xy, FxaaInt2( 0,-1), rcpFrame).xyz;
    float3 rgbW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1, 0), rcpFrame).xyz;
    float3 rgbM = FxaaTexOff(tex, pos.xy, FxaaInt2( 0, 0), rcpFrame).xyz;
    float3 rgbE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1, 0), rcpFrame).xyz;
    float3 rgbS = FxaaTexOff(tex, pos.xy, FxaaInt2( 0, 1), rcpFrame).xyz;
    float lumaN = FxaaLuma(rgbN);
    float lumaW = FxaaLuma(rgbW);
    float lumaM = FxaaLuma(rgbM);
    float lumaE = FxaaLuma(rgbE);
    float lumaS = FxaaLuma(rgbS);
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
    if(range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD)) {
		//TODO: Not compile with the return
		//return FxaaFilterReturn(rgbM); 
	}
    #if FXAA_SUBPIX > 0
        #if FXAA_SUBPIX_FASTER
            float3 rgbL = (rgbN + rgbW + rgbE + rgbS + rgbM) * FxaaToFloat3(1.0/5.0);
        #else
            float3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
        #endif
    #endif        

	 #if FXAA_SUBPIX != 0
        float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
        float rangeL = abs(lumaL - lumaM);
    #endif        
    #if FXAA_SUBPIX == 1
        float blendL = max(0.0, 
            (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE; 
        blendL = min(FXAA_SUBPIX_CAP, blendL);
    #endif
    #if FXAA_SUBPIX == 2
        float blendL = rangeL / range; 
    #endif
	
	float3 rgbNW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1,-1), rcpFrame).xyz;
    float3 rgbNE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1,-1), rcpFrame).xyz;
    float3 rgbSW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1, 1), rcpFrame).xyz;
    float3 rgbSE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1, 1), rcpFrame).xyz;
    #if (FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0)
        rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
        rgbL *= FxaaToFloat3(1.0/9.0);
    #endif
    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);
    float edgeVert = 
        abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +
        abs((0.50 * lumaW ) + (-1.0 * lumaM) + (0.50 * lumaE )) +
        abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));
    float edgeHorz = 
        abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +
        abs((0.50 * lumaN ) + (-1.0 * lumaM) + (0.50 * lumaS )) +
        abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    bool horzSpan = edgeHorz >= edgeVert;
   
    float lengthSign = horzSpan ? -rcpFrame.y : -rcpFrame.x;
    if(!horzSpan) lumaN = lumaW;
    if(!horzSpan) lumaS = lumaE;
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;
	
	bool pairN = gradientN >= gradientS;
   
    if(!pairN) lumaN = lumaS;
    if(!pairN) gradientN = gradientS;
    if(!pairN) lengthSign *= -1.0;
    float2 posN;
    posN.x = pos.x + (horzSpan ? 0.0 : lengthSign * 0.5);
    posN.y = pos.y + (horzSpan ? lengthSign * 0.5 : 0.0);
	
	gradientN *= FXAA_SEARCH_THRESHOLD;
	
	float2 posP = posN;
    float2 offNP = horzSpan ? 
        FxaaFloat2(rcpFrame.x, 0.0) :
        FxaaFloat2(0.0f, rcpFrame.y); 
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;
    #if FXAA_SEARCH_ACCELERATION == 1
        posN += offNP * FxaaFloat2(-1.0, -1.0);
        posP += offNP * FxaaFloat2( 1.0,  1.0);
    #endif
    #if FXAA_SEARCH_ACCELERATION == 2
        posN += offNP * FxaaFloat2(-1.5, -1.5);
        posP += offNP * FxaaFloat2( 1.5,  1.5);
        offNP *= FxaaFloat2(2.0, 2.0);
    #endif
    #if FXAA_SEARCH_ACCELERATION == 3
        posN += offNP * FxaaFloat2(-2.0, -2.0);
        posP += offNP * FxaaFloat2( 2.0,  2.0);
        offNP *= FxaaFloat2(3.0, 3.0);
    #endif
    #if FXAA_SEARCH_ACCELERATION == 4
        posN += offNP * FxaaFloat2(-2.5, -2.5);
        posP += offNP * FxaaFloat2( 2.5,  2.5);
        offNP *= FxaaFloat2(4.0, 4.0);
    #endif
    for(int i = 0; i < FXAA_SEARCH_STEPS; i++) {
        #if FXAA_SEARCH_ACCELERATION == 1
            if(!doneN) lumaEndN = 
                FxaaLuma(FxaaTexLod0(tex, posN.xy).xyz);
            if(!doneP) lumaEndP = 
                FxaaLuma(FxaaTexLod0(tex, posP.xy).xyz);
        #else
            if(!doneN) lumaEndN = 
                FxaaLuma(FxaaTexGrad(tex, posN.xy, offNP).xyz);
            if(!doneP) lumaEndP = 
                FxaaLuma(FxaaTexGrad(tex, posP.xy, offNP).xyz);
        #endif
        doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
        if(doneN && doneP) i = FXAA_SEARCH_STEPS;
        if(!doneN) posN -= offNP;
        if(!doneP) posP += offNP; 
	}
	
	float dstN = horzSpan ? pos.x - posN.x : pos.y - posN.y;
    float dstP = horzSpan ? posP.x - pos.x : posP.y - pos.y;
    bool directionN = dstN < dstP;
   
    lumaEndN = directionN ? lumaEndN : lumaEndP;
	
	if(((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
        lengthSign = 0.0;
	
	float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5 + (dstN * (-1.0/spanLength))) * lengthSign;
   
    float3 rgbF = FxaaTexLod0(tex, FxaaFloat2(
        pos.x + (horzSpan ? 0.0 : subPixelOffset),
        pos.y + (horzSpan ? subPixelOffset : 0.0))).xyz;
	
	#if FXAA_SUBPIX == 0
        return FxaaFilterReturn(rgbF); 
    #else        		
       return FxaaFilterReturn(FxaaLerp3(rgbL, rgbF, blendL)); 
    #endif
}

/*=========================================
			  VERTEX SHADER
=========================================*/

VertexShaderOutput mainVS(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput)0;
	output.UV = input.UV;
	
	//TODO: I don't know if it's the first or second line.
	//output.Position = float4(output.UV * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	output.Position = float4(input.Position.xyz, 1.0f);
	return output;
}

float4 mainPS(VertexShaderOutput input) : COLOR
{
	//TODO: I don't know if it's the first or second line.
	//float2 rcpFrame = float2(1/g_RenderTargetSize.x, 1/g_RenderTargetSize.y);
	float2 rcpFrame = g_RenderTargetSize;
	
	return float4(FxaaPixelShader(input.UV, (FxaaTex)S0AnisotropicWrapSampler, rcpFrame.xy), 1.0f);
}

/*=========================================
				TECHNIQUE
=========================================*/

technique fxaa_technique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}