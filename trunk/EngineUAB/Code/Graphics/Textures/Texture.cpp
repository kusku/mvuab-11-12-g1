#include "Texture.h"
#include "Base.h"
#include "Core.h"
#include "RenderManager.h"
#include "Logger\Logger.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTexture::CTexture()
	: m_Texture(NULL)
	, m_DepthStencilRenderTargetTexture(NULL)
	, m_OldDepthStencilRenderTarget(NULL)
	, m_RenderTargetTexture(NULL)
	, m_OldRenderTarget(NULL)
	, m_Width(0)
	, m_Height(0)
	, m_FileName("")
{
}

CTexture::~CTexture()
{
	Unload();
}

bool CTexture::LoadFile()
{
	LPDIRECT3DDEVICE9 Device = CORE->GetRenderManager()->GetDevice();
	HRESULT l_HR = D3DXCreateTextureFromFile(Device, m_FileName.c_str(), &m_Texture);
	return l_HR == S_OK ;
}

void CTexture::Unload()
{
	CHECKED_RELEASE(m_Texture);
	CHECKED_RELEASE(m_DepthStencilRenderTargetTexture);
	CHECKED_RELEASE(m_OldDepthStencilRenderTarget);
	CHECKED_RELEASE(m_RenderTargetTexture);
	CHECKED_RELEASE(m_OldRenderTarget);
}

bool CTexture::Load(const std::string &FileName)
{
	m_FileName = FileName;
	return LoadFile();
}

bool CTexture::Reload()
{
	Unload();
	return LoadFile();
}

void CTexture::Activate(size_t StageId)
{
	CORE->GetRenderManager()->GetDevice()->SetTexture(StageId, m_Texture);
}

void CTexture::Deactivate(size_t StageId)
{
	CORE->GetRenderManager()->GetDevice()->SetTexture(StageId, NULL);
}

bool CTexture::Create(const std::string &Name, uint32 Width, uint32 Height, uint32 MipMaps,
						TUsageType UsageType, TPoolType PoolType, TFormatType FormatType)
{
	SetName(Name);
	D3DPOOL l_Pool = D3DPOOL_DEFAULT;
	DWORD l_UsageType = D3DUSAGE_DYNAMIC;
	D3DFORMAT l_Format = D3DFMT_A8R8G8B8;
	bool l_CreateDepthStenculSurface = false;

	switch(UsageType)
	{
		case DYNAMIC:
			l_UsageType = D3DUSAGE_DYNAMIC;
			break;
		case RENDERTARGET:
			l_CreateDepthStenculSurface = true;
			l_UsageType = D3DUSAGE_RENDERTARGET;
			break;
	}

	switch(PoolType)
	{
		case DEFAULT:
			l_Pool = D3DPOOL_DEFAULT;
			break;
		case SYSTEMMEM:
			l_Pool = D3DPOOL_SYSTEMMEM;
			break;
	}

	switch(FormatType)
	{
		case A8R8G8B8:
			l_Format = D3DFMT_A8R8G8B8;
			break;
		case A16B16G16R16:
			l_Format = D3DFMT_A16B16G16R16;
			break;
		case A16B16G16R16F:
			l_Format = D3DFMT_A16B16G16R16F;
			break;
		case R8G8B8:
			l_Format = D3DFMT_R8G8B8;
			break;
		case X8R8G8B8:
			l_Format = D3DFMT_X8R8G8B8;
			break;
		case R32F:
			l_Format = D3DFMT_R32F;
			break;
	}

	HRESULT hr = CORE->GetRenderManager()->GetDevice()->CreateTexture(Width, Height, MipMaps, l_UsageType, l_Format, l_Pool, &m_Texture, NULL);

	if( l_CreateDepthStenculSurface )
	{
		CORE->GetRenderManager()->GetDevice()->CreateDepthStencilSurface(Width, Height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_DepthStencilRenderTargetTexture, NULL);

		assert(m_DepthStencilRenderTargetTexture!=NULL);
	}

	assert(m_Texture!=NULL);
	assert(hr==D3D_OK);

	m_Width = Width;
	m_Height = Height;

	return hr != D3D_OK;
}

void CTexture::CaptureFrameBuffer(size_t IdStage)
{
	LPDIRECT3DDEVICE9 l_Device=CORE->GetRenderManager()->GetDevice();
	LPDIRECT3DSURFACE9 l_RenderTarget, l_Surface;

	m_Texture->GetSurfaceLevel(0,&l_Surface);
	l_Device->GetRenderTarget(IdStage,&l_RenderTarget);
	l_Device->StretchRect(l_RenderTarget,NULL, l_Surface,NULL,D3DTEXF_NONE);
	l_RenderTarget->Release();
}

bool CTexture::SetAsRenderTarget(size_t IdStage)
{
	LPDIRECT3DDEVICE9 l_Device = CORE->GetRenderManager()->GetDevice();
	l_Device->GetRenderTarget((DWORD)IdStage, &m_OldRenderTarget);
	if( FAILED( m_Texture->GetSurfaceLevel(0, &m_RenderTargetTexture) ) )
	{
		return false;
	}

	l_Device->SetRenderTarget( (DWORD)IdStage, m_RenderTargetTexture );
	CHECKED_RELEASE(m_RenderTargetTexture);

	if( FAILED( l_Device->GetDepthStencilSurface( &m_OldDepthStencilRenderTarget ) ) )
	{
		return false; 
	}

	l_Device->SetDepthStencilSurface( m_DepthStencilRenderTargetTexture );

	return true;
}

void CTexture::UnsetAsRenderTarget(size_t IdStage)
{
	LPDIRECT3DDEVICE9 l_Device = CORE->GetRenderManager()->GetDevice();

	l_Device->SetDepthStencilSurface( m_OldDepthStencilRenderTarget );
	CHECKED_RELEASE( m_OldDepthStencilRenderTarget );

	l_Device->SetRenderTarget( IdStage, m_OldRenderTarget );
	CHECKED_RELEASE( m_OldRenderTarget );
}

CTexture::TFormatType CTexture::GetFormatTypeFromString(const std::string &FormatType)
{
	if( FormatType == "R32F" )
		return CTexture::R32F;
	else if( FormatType == "A8R8G8B8" )
		return CTexture::A8R8G8B8;
	else if( FormatType == "A16B16G16R16" )
		return CTexture::A16B16G16R16;
	else if( FormatType == "A16B16G16R16F" )
		return CTexture::A16B16G16R16F;
	else if( FormatType == "R8G8B8" )
		return CTexture::R8G8B8;
	else if( FormatType == "X8R8G8B8" )
		return CTexture::X8R8G8B8;
	else
		LOGGER->AddNewLog(ELL_WARNING, "CTexture::GetFormatTypeFromString->Tipo de formato %s no reconocido.", FormatType.c_str());

	return CTexture::A8R8G8B8;
}