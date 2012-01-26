#include "StaticMesh.h"
#include "RenderManager.h"
#include "Vertexs\RenderableVertexs.h"
#include "Vertexs\IndexedVertexs.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "Vertexs\VertexType.h"
#include "Base.h"
#include "Core.h"
#include "Math\Matrix44.h"
#include "Effects\EffectTechnique.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStaticMesh::CStaticMesh()
	: m_NumVertexs(0)
	, m_NumFaces(0)
	, m_FileName("")
{
}

CStaticMesh::~CStaticMesh()
{
	Unload();
}

bool CStaticMesh::Load(const std::string &FileName)
{
	m_FileName = FileName;
	return Reload();
}

bool CStaticMesh::Reload()
{
	Unload();
	
	FILE* modelFile = NULL;

	fopen_s(&modelFile, m_FileName.c_str(), "rb");
	
	if(modelFile == NULL)
	{
		return false;
	}

	uint16 tmp = 0;

	//Read Header
	fread(&tmp, sizeof(uint16), 1, modelFile);

	if(tmp != 0xCACA)
	{
		//header was not present, file type not valid
		fclose(modelFile);
		return false;
	}
		
	//Extract Mesh from File
	if(!ExtractMesh(modelFile))
	{
		Unload();
		fclose(modelFile);
		return false;
	}

	//Extract Bounding Box and Sphere from File
	if(!GetBoundingBoxAndSphere(modelFile))
	{
		Unload();
		fclose(modelFile);
		return false;
	}

	fclose(modelFile);
	return true;
}

bool CStaticMesh::ExtractMesh(FILE* modelFile)
{
	uint16 vertexType = 0;
	uint16 tmp = 0;
	char* c_name = NULL;
	std::string name = "";

	//Read Name of Object
	fread(&tmp, sizeof(uint16), 1, modelFile);
	c_name = new char[tmp];
	memset(c_name, 0, tmp);
	fread(c_name, 1, tmp, modelFile);
	name = std::string(c_name);
	CHECKED_DELETE(c_name);
	
	//Read Amount of Sub Meshes (Multi materials)
	fread(&tmp, sizeof(uint16), 1, modelFile);

	for(int i = 0; i < tmp; i++)
	{
		//Read Vertex Type
		fread(&vertexType, sizeof(uint16), 1, modelFile);

		if(vertexType & VERTEX_TYPE_TEXTURE1)
		{
			std::vector<CTexture*> textVector;

			if(vertexType & VERTEX_TYPE_TEXTURE1)
			{
				if(!ExtractTexture(modelFile, textVector))
				{
					ClearTextureVector(textVector);
					return false;
				}
			}
		
			if(vertexType & VERTEX_TYPE_TEXTURE2)
			{
				if(!ExtractTexture(modelFile, textVector))
				{
					ClearTextureVector(textVector);
					return false;
				}
			}
		
			if(vertexType & VERTEX_TYPE_TANGENT)
			{
				if(!ExtractTexture(modelFile, textVector))
				{
					ClearTextureVector(textVector);
					return false;
				}
			}

			m_Textures.push_back(textVector);
		}

		CRenderableVertexs* rndVtx = ReadCreateVertexBuffer(modelFile, vertexType);

		if(rndVtx == NULL)
		{
			return false;
		}
	
		//rndVtx->SetName(name);
		m_RVs.push_back(rndVtx);
	}

	return true;
}

CRenderableVertexs* CStaticMesh::ReadCreateVertexBuffer(FILE* modelFile, uint16 vertexType)
{
	assert(modelFile);

	uint16 numVertex = 0;
	uint16 numIndex = 0;
	uint16* idxBuffer = NULL;
	void* vtxBuffer = NULL;
	CRenderableVertexs* ret = NULL;

	/*****************************************************/

	//Read number of indexes
	fread(&numIndex, sizeof(uint16), 1, modelFile);
	
	//Create Index Buffer
	idxBuffer = new uint16[numIndex];
	memset(idxBuffer, 0, numIndex);

	//Read Index Buffer
	fread(idxBuffer, sizeof(uint16), numIndex, modelFile);
	
	/*****************************************************/

	//Read number of vertexs
	fread(&numVertex, sizeof(uint16), 1, modelFile);

	if(vertexType == TNORMALCOLORED_VERTEX::GetVertexType())
	{
		//Create Vertex Buffer
		vtxBuffer = LoadCreateVertexBuffer<TNORMALCOLORED_VERTEX>(modelFile, numVertex);
		
		//Create CIndexVertexs
		CIndexedVertexs<TNORMALCOLORED_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMALCOLORED_VERTEX>(CORE->GetRenderManager(), vtxBuffer, idxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	else if(vertexType == TNORMALTEXTURE2_VERTEX::GetVertexType() )
	{
		//Create Vertex Buffer
		vtxBuffer = LoadCreateVertexBuffer<TNORMALTEXTURE2_VERTEX>(modelFile, numVertex);
		
		//Create CIndexVertexs
		CIndexedVertexs<TNORMALTEXTURE2_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMALTEXTURE2_VERTEX>(CORE->GetRenderManager(), vtxBuffer, idxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	else if(vertexType == TNORMALTEXTURE1_VERTEX::GetVertexType() )
	{
		//Create Vertex Buffer
		vtxBuffer = LoadCreateVertexBuffer<TNORMALTEXTURE1_VERTEX>(modelFile, numVertex);
		
		//Create CIndexVertexs
		CIndexedVertexs<TNORMALTEXTURE1_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMALTEXTURE1_VERTEX>(CORE->GetRenderManager(), vtxBuffer, idxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	else if(vertexType == TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType() )
	{
		//Create Vertex Buffer
		vtxBuffer = LoadCreateVertexBuffer<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>(modelFile, numVertex);
		
		CalcTangentsAndBinormals(vtxBuffer, idxBuffer, numVertex, numIndex, sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX),
				0, 12, 28, 44, 60);

		//Create CIndexVertexs
		CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>(CORE->GetRenderManager(), vtxBuffer, idxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	
	CHECKED_DELETE(idxBuffer);
	CHECKED_DELETE(vtxBuffer);

	return ret;
}

bool CStaticMesh::GetBoundingBoxAndSphere(FILE* modelFile)
{
	assert(modelFile);

	float tmp = 0;
	Vect3f vect(0.0f, 0.0f, 0.0f);
	
	/***************************************/

	//Bounding Box

	//Read Min Position
	fread(&vect, sizeof(Vect3f), 1, modelFile);

	//Set Min Position
	m_BoundingBox.m_MinPos = vect;

	//Read Max Position
	fread(&vect, sizeof(Vect3f), 1, modelFile);
	
	//Set Max Position
	m_BoundingBox.m_MaxPos = vect;

	/***************************************/
	
	//Read Center Position
	fread(&vect, sizeof(Vect3f), 1, modelFile);

	//Set Center
	m_BoundingSphere.m_Center = vect;
	
	//Read Radius
	fread(&tmp, sizeof(float), 1, modelFile);

	//Set Radius
	m_BoundingSphere.m_Radius = tmp;

	/***************************************/

	return true;
}

template<class T>
void* CStaticMesh::LoadCreateVertexBuffer(FILE* modelFile, uint16 numVertex)
{	
	//Create Vertex Buffer
	void* vtxBuffer = new T[numVertex];

	//Set to 0 all buffer
	memset(vtxBuffer, 0, numVertex * sizeof(T));

	//Read Vertex Buffer
	fread(vtxBuffer, sizeof(T), numVertex, modelFile);

	return vtxBuffer;
}

bool CStaticMesh::ExtractTexture(FILE* modelFile, std::vector<CTexture*>& textVector)
{
	assert(modelFile);

	uint16 length = 0;
	char* path;
	std::string sPath = "";

	//Read Length of the texture path
	fread(&length, sizeof(uint16), 1, modelFile);

	path = new char[length];
	memset(path, 0, length);
			
	//Read Path
	fread(path, 1, length, modelFile);
	sPath = std::string(path);
	CHECKED_DELETE(path);

	CTexture* texture = new CTexture();
			
	if(!texture->Load(sPath))
	{
		CHECKED_DELETE(texture);
		texture = CORE->GetTextureManager()->GetNoTexture();

		std::string err = "CStaticMesh::ExtractTexture->No se ha podido crear la textura: " + sPath;
		LOGGER->AddNewLog(ELL_WARNING, err.c_str() );
	}

	textVector.push_back(texture);

	return true;
}

void CStaticMesh::ClearRenderableVertex()
{
	for(uint32 i = 0; i < m_RVs.size(); ++i)
	{
		CRenderableVertexs* rndVtx = m_RVs[i];

		CHECKED_DELETE(rndVtx);
	}

	m_RVs.clear();
}

void CStaticMesh::ClearTextures()
{
	for(uint16 i = 0; i < m_Textures.size(); ++i)
	{
		std::vector<CTexture*> vec = m_Textures[i];

		ClearTextureVector(vec);
	}

	m_Textures.clear();
}

void CStaticMesh::ClearTextureVector(std::vector<CTexture*>& textVector)
{
	for(uint16 i = 0; i < textVector.size(); ++i)
	{
		CTexture* texture = textVector[i];

		CHECKED_DELETE(texture);
	}

	textVector.clear();
}

void CStaticMesh::Unload()
{
	ClearTextures();
	ClearRenderableVertex();
	m_NumVertexs = 0;
	m_NumFaces = 0;
	m_BoundingBox = TBoundingBox();
	m_BoundingSphere = TBoundingSphere();
}

void CStaticMesh::Render(CRenderManager *RM) const
{
	uint16 l_Size = static_cast<uint16>(m_RVs.size());
	for(uint16 i=0; i<l_Size; ++i)
	{
		uint16 l_NumTexs = static_cast<uint16>(m_Textures[i].size());
		for( uint16 j=0; j < l_NumTexs; ++j)
		{
			m_Textures[i][j]->Activate(j);
		}
		m_RVs[i]->Render(RM);
	}
}

void CStaticMesh::Render(CRenderManager *RM, CEffectTechnique* technique) const
{
	uint16 l_Size = static_cast<uint16>(m_RVs.size());
	for(uint16 i=0; i<l_Size; ++i)
	{
		if(m_Textures.size() > 0)
		{
			uint16 l_NumTexs = static_cast<uint16>(m_Textures[i].size());
			for( uint16 j=0; j < l_NumTexs; ++j)
			{
				m_Textures[i][j]->Activate(j);
			}
		}

		m_RVs[i]->Render(RM, technique);
	}
}