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
#include "RenderableObjects\RenderableObjectTechniqueManager.h"
#include "RenderableObjects\RenderableObjectTechnique.h"
#include "Logger\Logger.h"
#include "Cameras\Frustum.h"
#include "Cameras\Camera.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CStaticMesh::CStaticMesh()
	: m_NumVertexs(0)
	, m_NumFaces(0)
	, m_FileName("")
	, m_RenderableObjecTechniqueName("")
	, m_MeshName					("")
	, m_IndxBuffer					( NULL )
	, m_VtxsBuffer					( NULL )
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
	ClearTextures();
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
		ClearTextures();
		Unload();
		fclose(modelFile);
		return false;
	}

	//Extract Bounding Box and Sphere from File
	if(!GetBoundingBoxAndSphere(modelFile))
	{
		ClearTextures();
		Unload();
		fclose(modelFile);
		return false;
	}

	fclose(modelFile);

	if(!GetRenderableObjectTechnique())
	{
		ClearTextures();
		Unload();
		return false;
	}

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

	m_MeshName = name;
	
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

			if(vertexType & VERTEX_TYPE_RNM)
			{
				//Extract X
				if(!ExtractTexture(modelFile, textVector))
				{
					ClearTextureVector(textVector);
					return false;
				}
				
				//Extract Y
				if(!ExtractTexture(modelFile, textVector))
				{
					ClearTextureVector(textVector);
					return false;
				}

				//Extract Z
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
			AddTexVecToTexManager(textVector);
		}

		//Pone el tipo de vértice en un vector.
		m_VertexTypes.push_back(vertexType);

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

	uint32 numVertex = 0;
	uint32 numIndex = 0;
	
	uint16* l_IdxBuffer = NULL;
	void*	l_VtxBuffer = NULL;


	CRenderableVertexs* ret = NULL;

	/*****************************************************/

	//Read number of indexes
	fread(&numIndex, sizeof(uint32), 1, modelFile);
	
	//Create Index Buffer
	l_IdxBuffer = new uint16[numIndex];
	memset(l_IdxBuffer, 0, numIndex);

	//Read Index Buffer
	fread(l_IdxBuffer, sizeof(uint16), numIndex, modelFile);
	
	/*****************************************************/

	//Read number of vertexs
	fread(&numVertex, sizeof(uint32), 1, modelFile);

	if(vertexType == TNORMALCOLORED_VERTEX::GetVertexType())
	{
		//Create Vertex Buffer
		l_VtxBuffer = LoadCreateVertexBuffer<TNORMALCOLORED_VERTEX>(modelFile, numVertex);
		
		CreateVect3fVertexsList<TNORMALCOLORED_VERTEX>( l_VtxBuffer, numVertex );
		CreateVect3fFacesList( l_IdxBuffer, numIndex );

		//Create CIndexVertexs
		CIndexedVertexs<TNORMALCOLORED_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMALCOLORED_VERTEX>(CORE->GetRenderManager(), l_VtxBuffer, l_IdxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	else if(vertexType == TNORMALTEXTURE2_VERTEX::GetVertexType() )
	{
		//Create Vertex Buffer
		l_VtxBuffer = LoadCreateVertexBuffer<TNORMALTEXTURE2_VERTEX>(modelFile, numVertex);
		
		CreateVect3fVertexsList<TNORMALTEXTURE2_VERTEX> ( l_VtxBuffer, numVertex );
		CreateVect3fFacesList( l_IdxBuffer, numIndex );

		//Create CIndexVertexs
		CIndexedVertexs<TNORMALTEXTURE2_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMALTEXTURE2_VERTEX>(CORE->GetRenderManager(), l_VtxBuffer, l_IdxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	else if(vertexType == TNORMALTEXTURE1_VERTEX::GetVertexType() )
	{
		//Create Vertex Buffer
		l_VtxBuffer = LoadCreateVertexBuffer<TNORMALTEXTURE1_VERTEX>(modelFile, numVertex);
		
		CreateVect3fVertexsList<TNORMALTEXTURE1_VERTEX> ( l_VtxBuffer, numVertex );
		CreateVect3fFacesList( l_IdxBuffer, numIndex );

		//Create CIndexVertexs
		CIndexedVertexs<TNORMALTEXTURE1_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMALTEXTURE1_VERTEX>(CORE->GetRenderManager(), l_VtxBuffer, l_IdxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	else if(vertexType == TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType() || 
			vertexType == (TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexType() | VERTEX_TYPE_PARALLAX))
	{
		//Create Vertex Buffer
		l_VtxBuffer = LoadCreateVertexBuffer<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>(modelFile, numVertex);
		
		CreateVect3fVertexsList<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX> ( l_VtxBuffer, numVertex );
		CreateVect3fFacesList( l_IdxBuffer, numIndex );

		CalcTangentsAndBinormals(l_VtxBuffer, l_IdxBuffer, numVertex, numIndex, sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX),
				0, 12, 28, 44, 60);

		//Create CIndexVertexs
		CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX>(CORE->GetRenderManager(), l_VtxBuffer, l_IdxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	else if(vertexType == TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX::GetVertexType())
	{
		//Create Vertex Buffer
		l_VtxBuffer = LoadCreateVertexBuffer<TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX>(modelFile, numVertex);
		
		CreateVect3fVertexsList<TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX> ( l_VtxBuffer, numVertex );
		CreateVect3fFacesList( l_IdxBuffer, numIndex );

		CalcTangentsAndBinormals(l_VtxBuffer, l_IdxBuffer, numVertex, numIndex, sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX),
				0, 12, 28, 44, 60);

		//Create CIndexVertexs
		CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX>* idxVtx = 
			new CIndexedVertexs<TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX>(CORE->GetRenderManager(), l_VtxBuffer, l_IdxBuffer, numVertex, numIndex);

		ret = idxVtx;
	}
	
	CHECKED_DELETE(l_IdxBuffer);
	CHECKED_DELETE(l_VtxBuffer);

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
	texture->SetName(m_MeshName + "_" + sPath);
			
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

void CStaticMesh::AddTexVecToTexManager(std::vector<CTexture*>& textVector)
{
	for(uint32 i = 0; i < textVector.size(); ++i)
	{
		CORE->GetTextureManager()->AddResource(textVector[i]->GetName(), textVector[i]);
	}
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

		if(!CORE->GetTextureManager()->RemoveResource(texture->GetName()))
		{
			CHECKED_DELETE(texture);
		}
	}

	textVector.clear();
}

void CStaticMesh::Unload()
{
	ClearRenderableVertex();
	m_VertexTypes.clear();
	m_RenderableObjectsTechniques.clear();
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
			CORE->GetEffectManager()->SetTextureDim(Vect2f((float)m_Textures[i][j]->GetWidth(), (float)m_Textures[i][j]->GetHeight()));
			m_Textures[i][j]->Activate(j);
		}
		m_RVs[i]->Render( RM, m_RenderableObjectsTechniques[i]->GetEffectTechnique() );
	}
}

void CStaticMesh::RenderInstance(CRenderManager* RM, LPDIRECT3DVERTEXBUFFER9 instanceBuffer, uint32 count) const
{
	uint16 l_Size = static_cast<uint16>(m_RVs.size());

	for(uint16 i=0; i<l_Size; ++i)
	{
		uint16 l_NumTexs = static_cast<uint16>(m_Textures[i].size());
		for( uint16 j=0; j < l_NumTexs; ++j)
		{
			CORE->GetEffectManager()->SetTextureDim(Vect2f((float)m_Textures[i][j]->GetWidth(), (float)m_Textures[i][j]->GetHeight()));
			m_Textures[i][j]->Activate(j);
		}
		
		m_RVs[i]->RenderInstance(RM, m_RenderableObjectsTechniquesInstance[i]->GetEffectTechnique(), instanceBuffer, count);
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
				CORE->GetEffectManager()->SetTextureDim(Vect2f((float)m_Textures[i][j]->GetWidth(), (float)m_Textures[i][j]->GetHeight()));
				m_Textures[i][j]->Activate(j);
			}
		}

		m_RVs[i]->Render(RM, technique);
	}
}

bool CStaticMesh::GetRenderableObjectTechnique()
{	
	bool l_Ok = true;
	m_RenderableObjectsTechniques.clear();
	m_RenderableObjectsTechniquesInstance.clear();

	CRenderableObjectTechniqueManager *l_ROTM = CORE->GetROTManager();
	
	for(size_t i=0; i<m_VertexTypes.size(); ++i)
	{
		if( m_RenderableObjecTechniqueName == "" )
		{
			m_RenderableObjecTechniqueName = l_ROTM->GetRenderableObjectTechniqueNameByVertexType(m_VertexTypes[i]);
		}

		CRenderableObjectTechnique *l_ROT = l_ROTM->GetResource(m_RenderableObjecTechniqueName);
		m_RenderableObjectsTechniques.push_back(l_ROT);

		if( l_ROT == NULL)
		{
			std::string warn = "CStaticMesh::GetRenderableObjectTechnique->Error intentando obtener el Renderable Object Technique: " +
				m_RenderableObjecTechniqueName;
			LOGGER->AddNewLog( ELL_ERROR, warn.c_str() );
		}



		std::string instanceTechniqueName = l_ROTM->GetRenderableObjectTechniqueNameByVertexType(m_VertexTypes[i] | VERTEX_TYPE_INSTANCE);
		CRenderableObjectTechnique* InstanceROT = l_ROTM->GetResource(instanceTechniqueName);
		assert(InstanceROT);
		m_RenderableObjectsTechniquesInstance.push_back(InstanceROT);
		
		l_Ok = l_Ok && l_ROT!=NULL;
	}

	return l_Ok;
}

// --------------------------------------------------------------------------------------------------------------
// CreateVect3fVertexsList : Crea una lista de vertices que almacenamos en el propio código para crear la maya física
// --------------------------------------------------------------------------------------------------------------
template <typename T>
void CStaticMesh::CreateVect3fVertexsList( const void *_VtxBuffer, uint32 _NumVertex )
{
	std::vector<Vect3f> l_Vect ;

	unsigned char  *l_Vtxs = (unsigned char *) _VtxBuffer;
	for ( uint32 i = 0; i < _NumVertex; ++i )
	{
		Vect3f *l_Vtx = (Vect3f *) l_Vtxs;
		m_VtxsBuffer.push_back(*l_Vtx);
		l_Vtxs += sizeof(T);
	}
}

// --------------------------------------------------------------------------------------------------------------
// CreateVect3fFacesList : Crea una lista de indices que almacenamos en el propio código para crear la maya física
// --------------------------------------------------------------------------------------------------------------
void CStaticMesh::CreateVect3fFacesList( const void *_IndxBuffer, uint32 _NumIndex )
{
	uint16* l_IndxBuffer = (uint16 *) _IndxBuffer;
	for ( uint32 i = 0; i < _NumIndex; ++i )
	{
		uint16 l_Valor = l_IndxBuffer[i];
		m_IndxBuffer.push_back(l_Valor);
	}
}
