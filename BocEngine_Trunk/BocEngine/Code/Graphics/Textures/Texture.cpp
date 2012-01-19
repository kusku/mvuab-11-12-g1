#include "Texture.h"
#include "Base.h"
#include "Core.h"
#include "RenderManager.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CTexture::CTexture()
	: m_Texture(NULL)
	, m_FileName("")
	, m_LockFromFile(false)
{
}


CTexture::~CTexture()
{
	Unload();
}

bool CTexture::LoadFile()
{
	m_LockFromFile = true;

	LPDIRECT3DDEVICE9 Device = CORE->GetRenderManager()->GetDevice();
	HRESULT l_HR = D3DXCreateTextureFromFile(Device, m_FileName.c_str(), &m_Texture);
	return l_HR == S_OK ;
}

void CTexture::Unload()
{
	CHECKED_RELEASE(m_Texture);
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




