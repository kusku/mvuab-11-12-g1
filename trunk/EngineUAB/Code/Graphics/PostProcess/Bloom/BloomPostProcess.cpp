

#include "BloomPostProcess.h"
#include "Textures\Texture.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"
#include "Base.h"
#include "Core.h"
#include "Effects\EffectManager.h"
#include "RenderManager.h"

CBloomPostProcess::CBloomPostProcess(const SBloomPostProcessSettings& settings)
	: m_Settings(settings)
	, m_Scene(NULL)
	, m_RT1(NULL)
	, m_RT2(NULL)
	, m_BloomExtract(NULL)
	, m_GaussianBlur(NULL)
	, m_BloomCombine(NULL)
	, m_SceneTextureParameter(NULL)
	, m_BloomThresholdParameter(NULL)
	, m_SampleOffsetsParameter(NULL)
	, m_SampleWeightsParameter(NULL)
	, m_GaussianBlurTextureParameter(NULL)
	, m_BloomIntensityParameter(NULL)
	, m_BaseIntensityParameter(NULL)
	, m_BloomSaturationParameter(NULL)
	, m_BaseSaturationParameter(NULL)
	, m_PostBloomTextureParameter(NULL)
	, m_BloomEffect(NULL)
{
}

CBloomPostProcess::~CBloomPostProcess()
{
	CHECKED_DELETE(m_Scene);
	CHECKED_DELETE(m_RT1);
	CHECKED_DELETE(m_RT2);
}

void CBloomPostProcess::Init()
{
	m_BloomExtract = CORE->GetEffectManager()->GetEffectTechnique("BloomExtract");
	m_BloomCombine = CORE->GetEffectManager()->GetEffectTechnique("BloomCombine");
	m_GaussianBlur = CORE->GetEffectManager()->GetEffectTechnique("GaussianBlur");
	m_BloomEffect = CORE->GetEffectManager()->GetEffect("BloomPostProcess");

	m_BloomEffect->GetParameterBySemantic("SCENE_TEXTURE", m_SceneTextureParameter);
	m_BloomEffect->GetParameterBySemantic("BLOOM_THRESHOLD", m_BloomThresholdParameter);
	m_BloomEffect->GetParameterBySemantic("SAMPLE_OFFSETS", m_SampleOffsetsParameter);
	m_BloomEffect->GetParameterBySemantic("SAMPLE_WEIGHTS", m_SampleWeightsParameter);
	m_BloomEffect->GetParameterBySemantic("GAUSSIAN_TEXTURE", m_GaussianBlurTextureParameter);
	m_BloomEffect->GetParameterBySemantic("BLOOM_INTENSITY", m_BloomIntensityParameter);
	m_BloomEffect->GetParameterBySemantic("BASE_INTENSITY", m_BaseIntensityParameter);
	m_BloomEffect->GetParameterBySemantic("BLOOM_SATURATION", m_BloomSaturationParameter);
	m_BloomEffect->GetParameterBySemantic("BASE_SATURATION", m_BaseSaturationParameter);
	m_BloomEffect->GetParameterBySemantic("POST_BLOOM_TEXTURE", m_PostBloomTextureParameter);
	
	uint32 width = CORE->GetRenderManager()->GetScreenSize().x;
	uint32 height = CORE->GetRenderManager()->GetScreenSize().y;

	m_Scene = new CTexture();
	m_Scene->Create("SCENE_TEXTURE", width, height, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8);

	width /= 2;
	height /= 2;

	m_RT1 = new CTexture();
	m_RT1->Create("RT1", width, height, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8);

	m_RT2 = new CTexture();
	m_RT2->Create("RT2", width, height, 1, CTexture::RENDERTARGET, CTexture::DEFAULT, CTexture::A8R8G8B8);
}	 
	 
void CBloomPostProcess::Render()
{
	m_Scene->CaptureFrameBuffer(0);
	
	RenderBloomExtract();

	RenderBloomGaussianBlur();

	RenderBloomCombine();
}

void CBloomPostProcess::SetBlurParameters(float dx, float dy)
{
	float sampleWeights[BLOOM_BLUR_SAMPLE_COUNT];
	Vect2f sampleOffsets[BLOOM_BLUR_SAMPLE_COUNT];

	sampleWeights[0] = ComputeGaussianBlur(0);
	sampleOffsets[0] = v2fZERO;

	float totalWeights = sampleWeights[0];

	for (int i = 0; i < BLOOM_BLUR_SAMPLE_COUNT / 2; i++)
	{
		float weight = ComputeGaussianBlur(i + 1.0f);

		sampleWeights[i * 2 + 1] = weight;
		sampleWeights[i * 2 + 2] = weight;

		totalWeights += weight * 2;
	
		float sampleOffset = i * 2 + 1.5f;

		Vect2f delta = Vect2f(dx, dy) * sampleOffset;

		sampleOffsets[i * 2 + 1] = delta;
		sampleOffsets[i * 2 + 2] = -(delta);
	}

	// Normalize the list of sample weightings, so they will always sum to one.
	for (int i = 0; i < BLOOM_BLUR_SAMPLE_COUNT; i++)
	{
		sampleWeights[i] /= totalWeights;
	}

	// Tell the effect about our new filter settings.
	m_BloomEffect->GetD3DEffect()->SetFloatArray(m_SampleWeightsParameter, sampleWeights, BLOOM_BLUR_SAMPLE_COUNT);
	m_BloomEffect->GetD3DEffect()->SetFloatArray(m_SampleOffsetsParameter, (float*)sampleOffsets, BLOOM_BLUR_SAMPLE_COUNT * 2);
}

float CBloomPostProcess::ComputeGaussianBlur(float n)
{
    float theta = m_Settings.m_BlurAmount;

	return (float)((1.0 / sqrt(2 * D3DX_PI * theta)) *
					exp(-(n * n) / (2 * theta * theta)));
}

bool CBloomPostProcess::RenderBloomExtract()
{
	m_RT1->SetAsRenderTarget(0);
	
	m_BloomEffect->GetD3DEffect()->SetTexture(m_SceneTextureParameter, m_Scene->GetDXTexture());
	m_BloomEffect->GetD3DEffect()->SetFloat(m_BloomThresholdParameter, m_Settings.m_BloomThreshold);

	CORE->GetRenderManager()->DrawQuad2DTexturedInPixelsInFullScreen( m_BloomExtract );

	m_RT1->UnsetAsRenderTarget(0);

	return true;
}

bool CBloomPostProcess::RenderBloomGaussianBlur()
{
	m_RT2->SetAsRenderTarget(0);
	
	SetBlurParameters(1.0f / (float)m_RT1->GetWidth(), 0.0f);
	m_BloomEffect->GetD3DEffect()->SetTexture(m_GaussianBlurTextureParameter, m_RT1->GetDXTexture());

	CORE->GetRenderManager()->DrawQuad2DTexturedInPixelsInFullScreen( m_GaussianBlur );

	m_RT2->UnsetAsRenderTarget(0);

	//Do Gaussian Blur Vertical
	m_RT1->SetAsRenderTarget(0);
	
	SetBlurParameters(0.0f, 1.0f / (float)m_RT1->GetHeight());
	m_BloomEffect->GetD3DEffect()->SetTexture(m_GaussianBlurTextureParameter, m_RT2->GetDXTexture());

	CORE->GetRenderManager()->DrawQuad2DTexturedInPixelsInFullScreen( m_GaussianBlur );

	m_RT1->UnsetAsRenderTarget(0);

	return true;
}	 

bool CBloomPostProcess::RenderBloomCombine()
{
	m_BloomEffect->GetD3DEffect()->SetTexture(m_SceneTextureParameter, m_Scene->GetDXTexture());
	m_BloomEffect->GetD3DEffect()->SetTexture(m_PostBloomTextureParameter, m_RT1->GetDXTexture());

	m_BloomEffect->GetD3DEffect()->SetFloat(m_BloomIntensityParameter, m_Settings.m_BloomIntensity);
	m_BloomEffect->GetD3DEffect()->SetFloat(m_BaseIntensityParameter, m_Settings.m_BaseIntensity);
	m_BloomEffect->GetD3DEffect()->SetFloat(m_BloomSaturationParameter, m_Settings.m_BloomSaturation);
	m_BloomEffect->GetD3DEffect()->SetFloat(m_BaseSaturationParameter, m_Settings.m_BaseSaturation);

	CORE->GetRenderManager()->DrawQuad2DTexturedInPixelsInFullScreen( m_BloomCombine );

	return true;
}
