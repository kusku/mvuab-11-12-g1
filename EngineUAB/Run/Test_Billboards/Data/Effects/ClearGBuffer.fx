

struct VertexShaderInput
{
    float3 Position : POSITION0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
};


VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;
	
    output.Position = float4(input.Position,1);
    
	return output;
}

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
	float4 LightRT		: COLOR3;
};

PixelShaderOutput PixelShaderFunction(VertexShaderOutput input)
{
    PixelShaderOutput output = (PixelShaderOutput)0;
	
    //black color
    output.DiffuseRT = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    //when transforming 0.5f into [-1,1], we will get 0.0f
    output.NormalRT.rgb = 0.5f;
    
	//no specular power
    output.NormalRT.a = 0.0f;
    
	//max depth
    output.DepthRT = 0.0f;
	
	//Transparent Light
    output.LightRT = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
    return output;
}

technique ClearGBuffer
{
    pass Pass1
    {
        VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 PixelShaderFunction();
    }
}