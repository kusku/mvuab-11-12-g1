#include "ActionToInput.h"
#include "InputManager.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"

CActionToInput::CActionToInput()
{
}


CActionToInput::~CActionToInput()
{
	Release();
}

void CActionToInput::Release()
{
	std::map< std::string, std::vector< SInputInfo > >::iterator it;
	for( it = m_ActionsMap.begin(); it != m_ActionsMap.end(); ++it )
	{
		it->second.clear();
	}

	CHECKED_DELETE(m_pInputManager);
}

void CActionToInput::Init(HWND hWnd, const Vect2i& screenRes, bool exclusiveModeinMouse)
{
	m_pInputManager = new CInputManager();
	m_pInputManager->Init( hWnd, screenRes, exclusiveModeinMouse );
}

void CActionToInput::LoadXML(const std::string &filename)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(filename.c_str()))
	{
		assert(0);
		return;
	}

	CXMLTreeNode l_InputNode = newFile["Actions"];
	if( l_InputNode.Exists() )
	{
		int count = l_InputNode.GetNumChildren();
		for(int i=0; i<count; ++i)
		{
			std::string l_Name = l_InputNode.GetName();
			if( l_Name == "action" )
			{
				int childs = l_InputNode(i).GetNumChildren();
				for(int j=0; i<childs; ++j)
				{
					l_Name = l_InputNode.GetName();
					if( l_Name == "input" )
					{

					}
				}
			}
		}
	}
}