#include "stdafx.h"
#include "XMLManager.h"
#include "XML\XMLTreeNode.h"
#include "GUIManager.h"
#include "GUIWindow.h"
#include "Controls\GUITextBox.h"
#include "Controls\GUIButton.h"
#include "Controls\GUIAnimatedImage.h"
#include "Controls\GUICheckButton.h"
#include "Controls\GUIPointerMouse.h"
#include "Controls\GUIDialogBox.h"
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

	SavePointerMouse(l_Node, l_pGUIManager->GetPointerMouse(), l_szPath);

	l_Node.EndElement();

	l_Node.EndNewFile();

	SaveWindow( l_Node, l_pGUIManager->GetWindow( l_pGUIManager->GetCurrentWindow() ), l_szPath);
}

void CXMLManager::SavePointerMouse(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIPointerMouse *pointer = static_cast<CGUIPointerMouse*>(element);

	node.StartElement("PointerMouse");

	node.WriteIntProperty("posx", (int)pointer->GetPositionPercent().x);
	node.WriteIntProperty("posy", (int)pointer->GetPositionPercent().y);
	node.WriteIntProperty("width", (int)pointer->GetWidthPercent());
	node.WriteIntProperty("height", (int)pointer->GetHeightPercent());
	node.WritePszProperty("texture", GetNameTexture( pointer->GetTexture( pointer->GetActiveTexture() )->GetFileName()).c_str() );
	node.WriteBoolProperty("isQuadrant", pointer->IsQuadrant());

	CopyTexture( pointer->GetTexture( pointer->GetActiveTexture() ), path );

	node.EndElement();
}

void CXMLManager::SaveWindow(CXMLTreeNode &node, CGUIWindow *window, const std::string &path)
{	 
	std::string l_Path = path + "\\GUI\\" + window->GetName() + ".xml";
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
		case CGuiElement::ANIMATED_IMAGE:
			{
				SaveAnimatedImage(node, element, path);
				break;
			}
		case CGuiElement::BUTTON:
			{
				SaveButton(node, element, path);
				break;
			}
		case CGuiElement::CHECKBUTTON:
			{
				SaveCheckButton(node, element, path);
				break;
			}
		case CGuiElement::DIALOG_BOX:
			{
				SaveDialogBox(node, element, path);
				break;
			}
		}
	}

	node.EndElement();
	node.EndNewFile();
}

void CXMLManager::SaveAnimatedImage(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIAnimatedImage *l_pAnimated = static_cast<CGUIAnimatedImage*>(element);

	node.StartElement("AnimatedImage");

	WriteCommonProperties(node, l_pAnimated);
	node.WriteIntProperty("initFrame", 0);
	//node.WriteBoolProperty("playOnLoad", l_pAnimated->GetPlayOnLoad());
	node.WritePszProperty("flip", "NONE_FLIP");
	node.WriteBoolProperty("backGround", false);
	node.WritePszProperty("OnLoadValue", l_pAnimated->GetOnLoad().c_str());
	node.WritePszProperty("OnSaveValue", l_pAnimated->GetOnSave().c_str());

	for( uint16 i=0; i<l_pAnimated->NumFrames(); ++i)
	{
		node.StartElement("Texture");
		
		node.WritePszProperty("name_texture", GetNameTexture( l_pAnimated->GetTexture(i)->GetFileName()).c_str() );
		CopyTexture( l_pAnimated->GetTexture(i), path );

		node.EndElement();
	}

	node.EndElement();
}

void CXMLManager::SaveButton(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIButton* l_pButton = static_cast<CGUIButton*>(element);

	node.StartElement("Button");

	WriteCommonProperties(node, l_pButton);
	node.WritePszProperty("texture_normal", GetNameTexture( l_pButton->GetNormalTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_over", GetNameTexture( l_pButton->GetOverTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_clicked", GetNameTexture( l_pButton->GetClickedTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_deactivated", GetNameTexture( l_pButton->GetDeactivatedTexture()->GetFileName() ).c_str());
	node.WritePszProperty("OnLoadValue", l_pButton->GetOnLoad().c_str());
	node.WritePszProperty("OnSaveValue", l_pButton->GetOnSave().c_str());
	node.WritePszProperty("OnOverAction", l_pButton->GetOnOver().c_str());
	node.WritePszProperty("OnClickedAction", l_pButton->GetOnClicked().c_str());

	node.EndElement();

	CopyTexture( l_pButton->GetNormalTexture(), path);
	CopyTexture( l_pButton->GetOverTexture(), path);
	CopyTexture( l_pButton->GetClickedTexture(), path);
	CopyTexture( l_pButton->GetDeactivatedTexture(), path);
}

void CXMLManager::SaveCheckButton(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUICheckButton *l_pCheckButton = static_cast<CGUICheckButton*>(element);

	node.StartElement("CheckButton");

	WriteCommonProperties(node, l_pCheckButton);
	node.WriteBoolProperty("isOn", false);
	node.WritePszProperty("texture_on", GetNameTexture( l_pCheckButton->GetOnTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_off", GetNameTexture( l_pCheckButton->GetOffTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_deactivated", GetNameTexture( l_pCheckButton->GetDeactivatedTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("OnCheckOn", l_pCheckButton->GetOnCheckOn().c_str() );
	node.WritePszProperty("OnCheckOff", l_pCheckButton->GetOnCheckOff().c_str() );
	node.WritePszProperty("OnOverButton", l_pCheckButton->GetOnOver().c_str() );
	node.WritePszProperty("OnLoadValue", l_pCheckButton->GetOnLoad().c_str());
	node.WritePszProperty("OnSaveValue", l_pCheckButton->GetOnSave().c_str());

	node.EndElement();

	CopyTexture( l_pCheckButton->GetOnTexture(), path );
	CopyTexture( l_pCheckButton->GetOffTexture(), path );
	CopyTexture( l_pCheckButton->GetDeactivatedTexture(), path );
}

void CXMLManager::SaveDialogBox(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIDialogBox *l_pDialogBox = static_cast<CGUIDialogBox*>(element);

	node.StartElement("DialogBox");

	WriteCommonProperties(node, l_pDialogBox);
	node.WriteFloatProperty("buttonH", l_pDialogBox->GetButtonHeight());
	node.WriteFloatProperty("buttonW", l_pDialogBox->GetButtonWidth());
	node.WritePszProperty("buttonClose_normal", GetNameTexture( l_pDialogBox->GetNormalButtonClose()->GetFileName() ).c_str() );
	node.WritePszProperty("buttonClose_over", GetNameTexture( l_pDialogBox->GetOverButtonClose()->GetFileName() ).c_str() );
	node.WritePszProperty("buttonClose_clicked", GetNameTexture( l_pDialogBox->GetClickedButtonClose()->GetFileName() ).c_str() );
	node.WritePszProperty("buttonClose_deactivated", GetNameTexture( l_pDialogBox->GetDeactivatedButtonClose()->GetFileName() ).c_str() );
	node.WritePszProperty("buttonMove_normal", GetNameTexture( l_pDialogBox->GetNormalButtonMove()->GetFileName() ).c_str() );
	node.WritePszProperty("buttonMove_over", GetNameTexture( l_pDialogBox->GetOverButtonMove()->GetFileName() ).c_str() );
	node.WritePszProperty("buttonMove_clicked", GetNameTexture( l_pDialogBox->GetClickedButtonMove()->GetFileName() ).c_str() );
	node.WritePszProperty("buttonMove_deactivated", GetNameTexture( l_pDialogBox->GetDeactivatedButtonMove()->GetFileName() ).c_str() );
	node.WritePszProperty("quad", GetNameTexture( l_pDialogBox->GetBackgroundTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("OnLoadValue", l_pDialogBox->GetOnLoad().c_str() );
	node.WritePszProperty("OnSaveValue", l_pDialogBox->GetOnSave().c_str() );
	node.EndElement();

	CopyTexture( l_pDialogBox->GetNormalButtonClose(), path );
	CopyTexture( l_pDialogBox->GetOverButtonClose(), path );
	CopyTexture( l_pDialogBox->GetClickedButtonClose(), path );
	CopyTexture( l_pDialogBox->GetDeactivatedButtonClose(), path );
	CopyTexture( l_pDialogBox->GetNormalButtonMove(), path );
	CopyTexture( l_pDialogBox->GetOverButtonMove(), path );
	CopyTexture( l_pDialogBox->GetClickedButtonMove(), path );
	CopyTexture( l_pDialogBox->GetDeactivatedButtonMove(), path );
	CopyTexture( l_pDialogBox->GetBackgroundTexture(), path );
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

void CXMLManager::WriteCommonProperties(CXMLTreeNode &node, CGuiElement *element)
{
	node.WritePszProperty("name", element->GetName().c_str());
	node.WriteFloatProperty("posx", element->GetPositionPercent().x);
	node.WriteFloatProperty("posy", element->GetPositionPercent().y);
	node.WriteFloatProperty("width", (float)element->GetWidthPercent());
	node.WriteFloatProperty("height", (float)element->GetHeightPercent());
	node.WriteFloatProperty("widthOffset", (float)element->GetTextWidthOffset());
	node.WriteFloatProperty("heightOffset", (float)element->GetTextHeightOffset());
	node.WriteBoolProperty("visible", element->IsVisible());
	node.WriteBoolProperty("active", element->IsActive());
	node.WritePszProperty("Literal", element->GetLiteral().c_str());
}