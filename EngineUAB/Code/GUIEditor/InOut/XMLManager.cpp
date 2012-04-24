#include "stdafx.h"
#include "XMLManager.h"
#include "XML\XMLTreeNode.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "Controls\GUITextBox.h"
#include "Controls\GUIButton.h"
#include "Controls\GUIPointerMouse.h"
#include "Core.h"
#include "Base.h"

void CXMLManager::SaveFiles(const std::string &filename)
{
	std::string l_szPath = "";

	int find = filename.rfind("\\");
	l_szPath = filename.substr(0, find);

	CGUIManager *l_pGUIManager = CORE->GetGUIManager();

	CreateDirectories(l_szPath);

	CXMLTreeNode l_Node;
	l_Node.StartNewFile( filename.c_str() );

	l_Node.StartElement( "Gui_config" );

	l_Node.StartElement( "GuiFile" );
	l_Node.WritePszProperty("path", "./Data/XML/GUI/");
	l_Node.EndElement();

	SavePointerMouse(l_Node, l_pGUIManager->GetPointerMouse());

	l_Node.EndElement();

	l_Node.EndNewFile();

	SaveWindow( l_Node, l_pGUIManager->GetWindow( l_pGUIManager->GetCurrentWindow() ), l_szPath);
}

void CXMLManager::SavePointerMouse(CXMLTreeNode &node, CGuiElement *element)
{
	CGUIPointerMouse *pointer = static_cast<CGUIPointerMouse*>(element);

	node.StartElement("PointerMouse");

	node.WriteIntProperty("posx", (int)pointer->GetPositionPercent().x);
	node.WriteIntProperty("posy", (int)pointer->GetPositionPercent().y);
	node.WriteIntProperty("width", (int)pointer->GetWidthPercent());
	node.WriteIntProperty("height", (int)pointer->GetHeightPercent());
	node.WritePszProperty("texture", pointer->GetTexture( pointer->GetActiveTexture() )->GetFileName().c_str() );
	node.WriteBoolProperty("isQuadrant", pointer->IsQuadrant());

	node.EndElement();
}

void CXMLManager::SaveWindow(CXMLTreeNode &node, CGUIWindow *window, const std::string &path)
{	 
	std::string l_Path = path + "\\GUI\\" + window->GetName();
	node.StartNewFile( l_Path.c_str() );

	node.StartElement("Windows");
	node.WritePszProperty("OnLoadWindows", window->GetOnLoadWindows().c_str());
	node.WritePszProperty("OnSaveWindows", window->GetOnSaveWindows().c_str());
	node.WritePszProperty("OnUpdateWindows", window->GetOnUpdateWindows().c_str());

	uint32 count = window->GetNumElements();
	for(uint32 i=0; i<count; ++i)
	{
		CGuiElement *element = window->GetElementById(i);
		switch(element->GetType())
		{
		case CGuiElement::BUTTON:
			{
				SaveButton(node, element, path);
				break;
			}
		}
	}

	node.EndElement();
	node.EndNewFile();
}

void CXMLManager::SaveButton(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIButton* l_pButton = static_cast<CGUIButton*>(element);

	node.StartElement("Button");

	node.WritePszProperty("name", l_pButton->GetName().c_str());
	node.WriteFloatProperty("posx", l_pButton->GetPositionPercent().x);
	node.WriteFloatProperty("posy", l_pButton->GetPositionPercent().y);
	node.WriteFloatProperty("width", (float)l_pButton->GetWidthPercent());
	node.WriteFloatProperty("height", (float)l_pButton->GetHeightPercent());
	node.WriteBoolProperty("visible", l_pButton->IsVisible());
	node.WriteBoolProperty("active", l_pButton->IsActive());
	node.WritePszProperty("texture_normal", GetNameTexture( l_pButton->GetNormalTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_over", GetNameTexture( l_pButton->GetOverTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_clicked", GetNameTexture( l_pButton->GetClickedTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_deactivated", GetNameTexture( l_pButton->GetDeactivatedTexture()->GetFileName() ).c_str());
	node.WritePszProperty("OnLoad", l_pButton->GetOnLoad().c_str());
	node.WritePszProperty("OnSave", l_pButton->GetOnSave().c_str());
	node.WritePszProperty("OnOverAction", l_pButton->GetOnOver().c_str());
	node.WritePszProperty("OnClickedAction", l_pButton->GetOnClicked().c_str());

	node.EndElement();

	CopyTexture( l_pButton->GetNormalTexture(), path);
	CopyTexture( l_pButton->GetOverTexture(), path);
	CopyTexture( l_pButton->GetClickedTexture(), path);
	CopyTexture( l_pButton->GetDeactivatedTexture(), path);
}

std::string CXMLManager::GetNameTexture(const std::string &file)
{
	int find = file.rfind("\\");
	if( find == -1 )
	{
		find = file.rfind("/");
	}
	std::string path = "./Data/General/Textures/GUI/" + file.substr(find + 1, file.size());

	return path;
}

void CXMLManager::CopyTexture(CTexture *texture, const std::string &path)
{
	int find = texture->GetFileName().rfind("\\");
	if( find == -1 )
	{
		find = texture->GetFileName().rfind("/");
	}
	
	std::string l_Out = path + "\\Textures\\GUI\\" + texture->GetFileName().substr( find + 1, texture->GetFileName().size());

	CopyFile( texture->GetFileName().c_str(), l_Out.c_str(), false);
}

void CXMLManager::CreateDirectories(const std::string &path)
{
	std::string textures_folder = path + "\\Textures\\";
	BOOL isOk = CreateDirectory( textures_folder.c_str(), NULL);
	textures_folder += "GUI\\";
	isOk = CreateDirectory( textures_folder.c_str(), NULL);

	std::string xml_folder = path + "\\GUI\\";
	isOk = CreateDirectory( xml_folder.c_str(), NULL);
}