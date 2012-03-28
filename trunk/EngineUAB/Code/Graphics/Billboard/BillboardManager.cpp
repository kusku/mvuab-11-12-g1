#include "BillboardManager.h"
#include "Cameras\Camera.h"
#include "RenderManager.h"
#include "Textures\TextureManager.h"
#include "BillboardAnimation.h"
#include "Textures\Texture.h"
#include "Core.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Base.h"

#if defined (_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CBillboardManager::CBillboardManager()
	: m_FileName("")
{
}

CBillboardManager::~CBillboardManager()
{
	CleanUp();
}

void CBillboardManager::CleanUp()
{
	for(uint16 i=0; i<m_BillboardInstancesVector.size(); ++i)
	{
		CHECKED_DELETE(m_BillboardInstancesVector[i]);
	}
	m_BillboardInstancesVector.clear();

	Destroy();
}

bool CBillboardManager::Load(const std::string &filename)
{
	m_FileName = filename;
	return LoadFile();
}

bool CBillboardManager::Reload()
{
	CleanUp();
	return LoadFile();
}

bool CBillboardManager::LoadFile()
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(m_FileName.c_str()))
	{
		std::string msg_error = "CBillboardManager::Load->Error al intentar leer el archivo xml de billboards: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}	
	
	CXMLTreeNode l_BillboardAnimations = newFile["BillboardAnimation"];
	if( l_BillboardAnimations.Exists() )
	{
		uint16 l_CountBA = l_BillboardAnimations.GetNumChildren();
		for( uint16 i=0; i<l_CountBA; ++i)
		{
			std::string l_BillboardAnimationType = l_BillboardAnimations(i).GetName();
			if( l_BillboardAnimationType == "Billboard" ) 
			{
				//Carga los billboards
				CXMLTreeNode l_Billboard = l_BillboardAnimations(i);

				uint16 l_CountBillboard = l_Billboard.GetNumChildren();
				for( uint16 j=0; j<l_CountBillboard; ++j)
				{
					std::string l_BillboardType = l_Billboard(j).GetName();
					if( l_BillboardType == "Animation" )
					{
						CXMLTreeNode l_Animation = l_Billboard(j);

						std::string l_Name = l_Animation.GetPszProperty("name", "");
						Vect4f l_ColorVector = l_Animation.GetVect4fProperty("color", Vect4f(1.0,1.0,1.0,1.0));
						CColor l_Color = CColor(l_ColorVector.x, l_ColorVector.y, l_ColorVector.z, l_ColorVector.w);
						float l_Size = l_Animation.GetFloatProperty("size", 1.f);
						float l_Time = l_Animation.GetFloatProperty("timePerImage", 1.0f);
						bool l_Loop = l_Animation.GetBoolProperty("loop", false);
						
						//Crea el core del billboard
						CBillboardCore *l_Core = new CBillboardCore(l_Size, l_Time, l_Loop, l_Color);

						uint16 l_CountAnimation = l_Animation.GetNumChildren();
						for(uint16 k=0; k<l_CountAnimation; ++k)
						{
							std::string l_TextureType = l_Animation(k).GetName();
							if( l_TextureType == "Texture" )
							{
								std::string l_FileNameTexture = l_Animation(k).GetPszProperty("file", "");
								CTexture *l_Texture = CORE->GetTextureManager()->GetTexture(l_FileNameTexture);
								l_Core->AddTexture(l_Texture);
							}
						}

						//Añade el core en el map
						AddResource(l_Name,l_Core);
					}
				}
			}
			else if( l_BillboardAnimationType == "Instances" )
			{
				//Carga las instancias de los billboards
				CXMLTreeNode l_Instances = l_BillboardAnimations(i);

				uint16 l_CountInstances = l_Instances.GetNumChildren();
				for(uint16 i=0; i<l_CountInstances; ++i)
				{
					std::string l_InstancesType = l_Instances(i).GetName();
					if( l_InstancesType == "Instance" )
					{
						std::string l_Id = l_Instances(i).GetPszProperty("id", "");
						Vect3f l_Pos = l_Instances(i).GetVect3fProperty("pos", Vect3f(0.f, 0.f, 0.f));

						//Crea la instancia
						CBillboardCore *l_Core = GetResource(l_Id);
						if( l_Core == NULL )
						{
							LOGGER->AddNewLog(ELL_WARNING, "CBillboardManager::LoadFile->No se ha podido crear la instancia del billboard: %s", l_Id.c_str());
						}
						else
						{
							CBillboardAnimation *l_BillboardAnimation = new CBillboardAnimation(
								l_Core->m_fSize, l_Core->m_fSize, l_Pos, 0.0f, l_Core->m_bLoop, l_Core->m_Color);
							l_BillboardAnimation->SetTimeToUpdate(l_Core->m_fTimePerImage);

							for(uint16 j=0; j<l_Core->m_TexturesVector.size(); ++j)
							{
								l_BillboardAnimation->AddTexture(l_Core->m_TexturesVector[j]);
							}

							//Añade la instancia al vector
							m_BillboardInstancesVector.push_back(l_BillboardAnimation);
						}
					}
				}
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

void CBillboardManager::Update(float elapsedTime, CCamera &camera)
{
	for(uint16 i=0; i<m_BillboardInstancesVector.size(); ++i)
	{
		m_BillboardInstancesVector[i]->Update(elapsedTime, camera);
	}
}

void CBillboardManager::Render(CRenderManager &RM)
{
	Mat44f mat;
	mat.SetIdentity();
	RM.SetTransform(mat);

	LPDIRECT3DDEVICE9 Device = RM.GetDevice();

	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for(uint16 i=0; i<m_BillboardInstancesVector.size(); ++i)
	{
		m_BillboardInstancesVector[i]->Render(RM);
	}

	Device->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
