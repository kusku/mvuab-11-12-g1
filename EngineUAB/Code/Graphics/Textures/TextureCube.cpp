

#include "TextureCube.h"
#include "RenderManager.h"
#include "Core.h"


CTextureCube::CTextureCube()
	: m_DXCubeTexture(NULL)
{

}

CTextureCube::~CTextureCube()
{
	CHECKED_RELEASE(m_DXCubeTexture);
}

bool CTextureCube::Load(const std::string& Filename)
{
	m_Filename = Filename;

	return Reload();
}

bool CTextureCube::Reload()
{
	CHECKED_RELEASE(m_DXCubeTexture);

	LPDIRECT3DDEVICE9 Device = CORE->GetRenderManager()->GetDevice();
	HRESULT l_HR = D3DXCreateCubeTextureFromFile(Device, m_Filename.c_str(), &m_DXCubeTexture);
	return l_HR == S_OK ;
}