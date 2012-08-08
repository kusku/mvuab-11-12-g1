#include "stdafx.h"
#include "XMLManager.h"
#include "XML\XMLTreeNode.h"
#include "Particles\ParticleSettingsManager.h"
#include "Particles\ParticleDef.h"
#include "Core.h"
#include "Base.h"

namespace XML
{
	void SaveData(const std::string &_Filename)
	{
		std::string l_szPath = "";

		int find = _Filename.rfind("\\");
		l_szPath = _Filename.substr(0, find);

		//Crea la carpeta para las partículas
		std::string textures_folder = l_szPath + "\\Textures\\";
		BOOL isOk = CreateDirectory( textures_folder.c_str(), NULL);
		textures_folder += "Particles\\";
		isOk = CreateDirectory( textures_folder.c_str(), NULL);

		CXMLTreeNode l_Node;
		l_Node.StartNewFile( _Filename.c_str() );

		l_Node.StartElement( "particle_settings" );

		uint16 l_SettingsCount = CORE->GetParticleSettingsManager()->GetResourcesVector().size();
		for(uint16 i=0; i<l_SettingsCount; ++i)
		{
			TParticleSystemSettings *l_Settings = CORE->GetParticleSettingsManager()->GetResourceById(i);

			l_Node.StartElement( "settings" );
			l_Node.WritePszProperty("name", l_Settings->m_Name.c_str());

			l_Node.StartElement("TextureName");
			int find = l_Settings->m_TextureName.rfind("\\");
			if( find == -1 )
			{
				find = l_Settings->m_TextureName.rfind("/");
			}
			std::string path = "./Data/General/Textures/Particles/" + l_Settings->m_TextureName.substr(find + 1, l_Settings->m_TextureName.size());
			l_Node.WritePszProperty("value", path.c_str());

			path = l_szPath + "\\Textures\\Particles\\" + l_Settings->m_TextureName.substr(find + 1, l_Settings->m_TextureName.size());
			CopyFile( l_Settings->m_TextureName.c_str(), path.c_str(), false);

			l_Node.EndElement();

			l_Node.StartElement("MaxParticles");
			l_Node.WriteIntProperty("value", l_Settings->m_MaxParticles);
			l_Node.EndElement();

			l_Node.StartElement("Duration");
			l_Node.WriteFloatProperty("value", l_Settings->m_Duration);
			l_Node.EndElement();

			l_Node.StartElement("DurationRandomness");
			l_Node.WriteFloatProperty("value", l_Settings->m_DurationRandomness);
			l_Node.EndElement();

			l_Node.StartElement("EmitterVelocitySensitivity");
			l_Node.WriteFloatProperty("value", l_Settings->m_EmitterVelocitySensitivity);
			l_Node.EndElement();

			l_Node.StartElement("MinHorizontalVelocity");
			l_Node.WriteFloatProperty("value", l_Settings->m_MinHorizontalVelocity);
			l_Node.EndElement();

			l_Node.StartElement("MaxHorizontalVelocity");
			l_Node.WriteFloatProperty("value", l_Settings->m_MaxHorizontalVelocity);
			l_Node.EndElement();

			l_Node.StartElement("MinVerticalVelocity");
			l_Node.WriteFloatProperty("value", l_Settings->m_MinVerticalVelocity);
			l_Node.EndElement();

			l_Node.StartElement("MaxVerticalVelocity");
			l_Node.WriteFloatProperty("value", l_Settings->m_MaxVerticalVelocity);
			l_Node.EndElement();

			l_Node.StartElement("Gravity");
			l_Node.WriteVect3fProperty("value", l_Settings->m_Gravity);
			l_Node.EndElement();

			l_Node.StartElement("EndVelocity");
			l_Node.WriteFloatProperty("value", l_Settings->m_EndVelocity);
			l_Node.EndElement();

			l_Node.StartElement("MinColor");
			Vect4f l_minColor = Vect4f(l_Settings->m_MinColor.GetRed(), l_Settings->m_MinColor.GetGreen(), l_Settings->m_MinColor.GetBlue(), l_Settings->m_MinColor.GetAlpha());
			l_minColor.x *= 255.f; l_minColor.y *= 255.f; l_minColor.z *= 255.f; l_minColor.w *= 255.f;
			l_Node.WriteVect4fProperty("value", l_minColor);
			l_Node.EndElement();

			l_Node.StartElement("MaxColor");
			Vect4f l_maxColor = Vect4f(l_Settings->m_MaxColor.GetRed(), l_Settings->m_MaxColor.GetGreen(), l_Settings->m_MaxColor.GetBlue(), l_Settings->m_MaxColor.GetAlpha());
			l_maxColor.x *= 255.f; l_maxColor.y *= 255.f; l_maxColor.z *= 255.f; l_maxColor.w *= 255.f;
			l_Node.WriteVect4fProperty("value", l_maxColor);
			l_Node.EndElement();

			l_Node.StartElement("MinRotateSpeed");
			l_Node.WriteFloatProperty("value", l_Settings->m_MinRotateSpeed);
			l_Node.EndElement();

			l_Node.StartElement("MaxRotateSpeed");
			l_Node.WriteFloatProperty("value", l_Settings->m_MaxRotateSpeed);
			l_Node.EndElement();

			l_Node.StartElement("MinStartSize");
			l_Node.WriteFloatProperty("value", l_Settings->m_MinStartSize);
			l_Node.EndElement();

			l_Node.StartElement("MaxStartSize");
			l_Node.WriteFloatProperty("value", l_Settings->m_MaxStartSize);
			l_Node.EndElement();

			l_Node.StartElement("MinEndSize");
			l_Node.WriteFloatProperty("value", l_Settings->m_MinEndSize);
			l_Node.EndElement();

			l_Node.StartElement("MaxEndSize");
			l_Node.WriteFloatProperty("value", l_Settings->m_MaxEndSize);
			l_Node.EndElement();

			l_Node.StartElement("BlendState");
			l_Node.WritePszProperty("value", l_Settings->m_BlendName.c_str());
			l_Node.EndElement();

			l_Node.EndElement();
		}

		l_Node.EndElement();
		l_Node.EndNewFile();
	}


} //namespace XML
