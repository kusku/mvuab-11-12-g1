#include "OmniLight.h"
#include "RenderManager.h"
#include "Math\Matrix44.h"
#include "XML\XMLTreeNode.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

COmniLight::COmniLight()
{
	m_Type = OMNI;
}

COmniLight::COmniLight(CXMLTreeNode *XMLNode)
{
	m_Type = OMNI;
	m_Name = XMLNode->GetPszProperty("name", "");
	m_Position = XMLNode->GetVect3fProperty("pos", Vect3f(0.0f, 0.0f, 0.0f));
	m_StartRangeAttenuation = XMLNode->GetFloatProperty("att_start_range", 0.0f);
	m_EndRangeAttenuation = XMLNode->GetFloatProperty("att_end_range", 0.0f);
	m_RenderShadows = XMLNode->GetBoolProperty("render_shadows", false);
	Vect3f l_Color = XMLNode->GetVect3fProperty("color", Vect3f(0.0f, 0.0f, 0.0f));
	
	m_Color = CColor( l_Color.x, l_Color.y, l_Color.z );
}

COmniLight::~COmniLight()
{
}

void COmniLight::Render(CRenderManager *RM)
{
	Mat44f mat;
	mat.SetIdentity();
	mat.Translate( GetPosition() );

	RM->SetTransform(mat);
	RM->DrawSphere( 0.3f, 5, colYELLOW );
}