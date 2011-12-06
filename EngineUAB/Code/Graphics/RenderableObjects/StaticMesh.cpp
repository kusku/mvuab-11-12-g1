#include "StaticMesh.h"
#include "..\RenderManager.h"
#include "..\Vertexs\RenderableVertexs.h"
#include "..\Textures\Texture.h"
#include "..\Vertexs\VertexType.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStaticMesh::CStaticMesh()
	: m_NumVertexs(0)
	, m_NumFaces(0)
{
}

CStaticMesh::~CStaticMesh()
{
	Unload();
}

bool CStaticMesh::Load(const std::string &FileName)
{
	m_FileName = FileName;
	return LoadFile();
}

bool CStaticMesh::Reload()
{
	Unload();
	return LoadFile();
}

void CStaticMesh::Unload()
{
	std::vector<CRenderableVertexs*>::iterator l_It = m_RVs.begin();
	std::vector<CRenderableVertexs*>::iterator l_End = m_RVs.end();

	for(; l_It != l_End; ++l_It)
	{
		CHECKED_DELETE( (*l_It) );
	}

	m_RVs.clear();

	std::vector<std::vector<CTexture*>>::iterator l_ItTex = m_Textures.begin();
	std::vector<std::vector<CTexture*>>::iterator l_EndTex = m_Textures.end();
	for(; l_ItTex != l_EndTex; ++l_ItTex)
	{
		(*l_ItTex).clear();
	}

	m_Textures.clear();
}

bool CStaticMesh::LoadFile()
{
	/*TCOLORED_VERTEX *l_Vtx;
	FILE *l_File = NULL;
	fopen_s(&l_File, "C:\\a.m3d", "rb");
	if(l_File != NULL)
	{
		int l_Data, l_fData;

		fread(&l_Data, sizeof(int), 1, l_File);
		fread(&l_fData, sizeof(float), 1, l_File);

		l_Vtx = new TCOLORED_VERTEX[200];
		fread(&l_Vtx, sizeof(TCOLORED_VERTEX), 200, l_File);

		fclose(l_File);
	}*/
	return true;
}

void CStaticMesh::Render(CRenderManager *RM) const
{
	int l_Size = static_cast<int>(m_RVs.size());
	for(int i=0; i<l_Size; ++i)
	{
		m_RVs[i]->Render(RM);
	}
}