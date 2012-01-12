#include "LightManager.h"
#include "RenderManager.h"
#include "OmniLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "XML\XMLTreeNode.h"
#include "Math\Vector3.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
	Destroy();
	m_LightsNameVector.clear();
}

bool CLightManager::Load(const std::string &Filename)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(Filename.c_str()))
	{
		std::string msg_error = "CLightManager::Load->Error al intentar leer el archivo xml de luces: " + Filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		return false;
	}	
	
	CXMLTreeNode l_Lights = newFile["lights"];
	if( l_Lights.Exists() )
	{
		uint16 l_Count = l_Lights.GetNumChildren();
		for(uint16 i=0; i<l_Count; ++i)
		{
			std::string l_TypeLight = l_Lights(i).GetName();
			if( l_TypeLight == "omni" )
			{
				std::string l_Name = l_Lights(i).GetPszProperty("name", "");
				Vect3f l_Pos = l_Lights(i).GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));
				Vect3f l_Color = l_Lights(i).GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));
				float l_StartRangeAtt = l_Lights(i).GetFloatProperty("att_start_range", 0.0f);
				float l_EndRangeAtt = l_Lights(i).GetFloatProperty("att_end_range", 0.0f);
				bool l_Shadows = l_Lights(i).GetBoolProperty("render_shadows", false);

				COmniLight *l_Light = new COmniLight();

				l_Light->SetType(CLight::OMNI);
				l_Light->SetName(l_Name);
				l_Light->SetPosition(l_Pos);
				l_Light->SetColor( CColor( l_Color.x, l_Color.y, l_Color.z) );
				l_Light->SetStartRangeAttenuation(l_StartRangeAtt);
				l_Light->SetEndRangeAttenuation(l_EndRangeAtt);
				l_Light->SetRenderShadows(l_Shadows);
				
				AddResource(l_Name, l_Light);
				m_LightsNameVector.push_back(l_Name);
				l_Light = NULL;
			}
			else if( l_TypeLight == "directional" )
			{
				std::string l_Name = l_Lights(i).GetPszProperty("name", "");
				Vect3f l_Pos = l_Lights(i).GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));
				Vect3f l_Dir = l_Lights(i).GetVect3fProperty("dir", Vect3f(0.0f, 0.0f, 0.0f));
				Vect3f l_Color = l_Lights(i).GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));
				float l_StartRangeAtt = l_Lights(i).GetFloatProperty("att_start_range", 0.0f);
				float l_EndRangeAtt = l_Lights(i).GetFloatProperty("att_end_range", 0.0f);
				bool l_Shadows = l_Lights(i).GetBoolProperty("render_shadows", false);

				CDirectionalLight *l_Light = new CDirectionalLight();

				l_Light->SetType(CLight::DIRECTIONAL);
				l_Light->SetName(l_Name);
				l_Light->SetPosition(l_Pos);
				l_Light->SetDirection(l_Dir);
				l_Light->SetColor( CColor( l_Color.x, l_Color.y, l_Color.z) );
				l_Light->SetStartRangeAttenuation(l_StartRangeAtt);
				l_Light->SetEndRangeAttenuation(l_EndRangeAtt);
				l_Light->SetRenderShadows(l_Shadows);
				l_Light->CalculateOrientationDebugRender();

				AddResource(l_Name, l_Light);
				m_LightsNameVector.push_back(l_Name);
				l_Light = NULL;

			}
			else if( l_TypeLight == "spot" )
			{
				std::string l_Name = l_Lights(i).GetPszProperty("name", "");
				Vect3f l_Pos = l_Lights(i).GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));
				Vect3f l_Dir = l_Lights(i).GetVect3fProperty("dir", Vect3f(0.0f, 0.0f, 0.0f));
				Vect3f l_Color = l_Lights(i).GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));
				float l_Angle = l_Lights(i).GetFloatProperty("angle", 0.0f);
				float l_FallOff = l_Lights(i).GetFloatProperty("fall_off", 0.0f);
				float l_StartRangeAtt = l_Lights(i).GetFloatProperty("att_start_range", 0.0f);
				float l_EndRangeAtt = l_Lights(i).GetFloatProperty("att_end_range", 0.0f);
				bool l_Shadows = l_Lights(i).GetBoolProperty("render_shadows", false);

				CSpotLight *l_Light = new CSpotLight();

				l_Light->SetType(CLight::SPOT);
				l_Light->SetName(l_Name);
				l_Light->SetPosition(l_Pos);
				l_Light->SetDirection(l_Dir);
				l_Light->SetAngle(l_Angle);
				l_Light->SetFallOff(l_FallOff);
				l_Light->SetColor( CColor( l_Color.x, l_Color.y, l_Color.z) );
				l_Light->SetStartRangeAttenuation(l_StartRangeAtt);
				l_Light->SetEndRangeAttenuation(l_EndRangeAtt);
				l_Light->SetRenderShadows(l_Shadows);
				l_Light->CalculateOrientationDebugRender();

				AddResource(l_Name, l_Light);
				m_LightsNameVector.push_back(l_Name);
				l_Light = NULL;
			}

		}
	}
	else
	{
		return false;
	}

	return true;
}

void CLightManager::Render(CRenderManager *RM)
{
	uint16 l_NumLights = m_LightsNameVector.size();
	for(uint16 i=0; i<l_NumLights; ++i)
	{
		CLight* l_Light = GetResource(m_LightsNameVector[i]);
		l_Light->Render(RM);
	}
}

const std::string& CLightManager::GetLightNameByIndex(uint16 index)
{
	return m_LightsNameVector[index];
}