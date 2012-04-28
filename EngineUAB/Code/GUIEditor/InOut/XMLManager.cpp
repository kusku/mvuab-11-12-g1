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
#include "Controls\GUIEditableTextBox.h"
#include "Controls\GUIProgressBar.h"
#include "Controls\GUIImage.h"
#include "Controls\GUISlider.h"
#include "Controls\GUIStaticText.h"
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
		case CGuiElement::EDITABLE_TEXT_BOX:
			{
				SaveEditableTextBox(node, element, path);
				break;
			}
		case CGuiElement::IMAGE:
			{
				SaveImage(node, element, path);
				break;
			}
		case CGuiElement::PROGRESS_BAR:
			{
				SaveProgressBar(node, element, path);
				break;
			}
		case CGuiElement::SLIDER:
			{
				SaveSlider(node, element, path);
				break;
			}
		case CGuiElement::STATIC_TEXT:
			{
				SaveStaticText(node, element, path);
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

void CXMLManager::SaveEditableTextBox(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIEditableTextBox *l_pEditable = static_cast<CGUIEditableTextBox*>(element);

	node.StartElement("EditableTextBox");

	CColor color = l_pEditable->GetTextColor();
	CColor backgroundColor = l_pEditable->GetBackGroundColor();
	CTexture *texture = l_pEditable->GetBackGroundTexture();

	WriteCommonProperties( node, l_pEditable );
	node.WriteIntProperty("id_font", (int)l_pEditable->GetFontID() );
	node.WriteFloatProperty("color_font_r", color.GetRed() );
	node.WriteFloatProperty("color_font_g", color.GetGreen() );
	node.WriteFloatProperty("color_font_b", color.GetBlue() );
	node.WriteFloatProperty("color_background_r", backgroundColor.GetRed() );
	node.WriteFloatProperty("color_background_g", backgroundColor.GetGreen() );
	node.WriteFloatProperty("color_background_b", backgroundColor.GetBlue() );
	node.WritePszProperty("buffer", "");

	if( texture == NULL )
	{
		node.WritePszProperty("texture_quad", "" );
	}
	else
	{
		node.WritePszProperty("texture_quad", GetNameTexture( texture->GetFileName() ).c_str() );
	}

	node.WritePszProperty("OnLoadValue", l_pEditable->GetOnLoad().c_str() );
	node.WritePszProperty("OnSaveValue", l_pEditable->GetOnSave().c_str() );
	
	node.EndElement();

	if( texture != NULL )
	{
		CopyTexture( texture, path );
	}
}

void CXMLManager::SaveImage(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIImage *l_pImage = static_cast<CGUIImage*>(element);
	CTexture *texture = l_pImage->GetTexture( l_pImage->GetActiveTexture() );


	node.StartElement("Image");

	WriteCommonProperties(node, l_pImage);
	node.WritePszProperty("default", "tex1");
	switch( l_pImage->GetFlip() )
	{
	case NONE_FLIP:
		{
			node.WritePszProperty("flip", "NONE_FLIP");
			break;
		}
	case FLIP_X:
		{
			node.WritePszProperty("flip", "FLIP_X");
			break;
		}
	case FLIP_Y:
		{
			node.WritePszProperty("flip", "FLIP_Y");
			break;
		}
	}	

	node.WriteBoolProperty("backGround", false);
	node.WritePszProperty("OnLoadValue", l_pImage->GetOnLoad().c_str() );
	node.WritePszProperty("OnSaveValue", l_pImage->GetOnSave().c_str() );

	node.StartElement("Texture");
	
	node.WritePszProperty("name", "tex1");
	node.WritePszProperty("name_texture", GetNameTexture( texture->GetFileName() ).c_str() );

	node.EndElement();

	node.EndElement();

	CopyTexture( texture, path );
}

void CXMLManager::SaveProgressBar(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIProgressBar *l_pProgressBar = static_cast<CGUIProgressBar*>(element);

	CColor background_color = l_pProgressBar->GetBackGroundColor();
	CColor text_color = l_pProgressBar->GetTextColor();
	CColor progress_color = l_pProgressBar->GetProgressColor();

	node.StartElement("ProgressBar");

	WriteCommonProperties(node, l_pProgressBar);
	node.WriteIntProperty("id_font", (int)l_pProgressBar->GetFontID());
	node.WriteFloatProperty("color_font_r", text_color.GetRed());
	node.WriteFloatProperty("color_font_g", text_color.GetGreen());
	node.WriteFloatProperty("color_font_b", text_color.GetBlue());
	node.WriteFloatProperty("color_background_r", background_color.GetRed());
	node.WriteFloatProperty("color_background_g", background_color.GetGreen());
	node.WriteFloatProperty("color_background_b", background_color.GetBlue());
	node.WriteFloatProperty("color_progress_r", progress_color.GetRed());
	node.WriteFloatProperty("color_progress_g", progress_color.GetGreen());
	node.WriteFloatProperty("color_progress_b", progress_color.GetBlue());

	node.WritePszProperty("texture_bar", GetNameTexture( l_pProgressBar->GetProgressTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("texture_back", GetNameTexture( l_pProgressBar->GetBackgroundTexture()->GetFileName() ).c_str() );

	node.WritePszProperty("OnComplete", l_pProgressBar->GetOnComplete().c_str() );
	node.WritePszProperty("OnLoadValue", l_pProgressBar->GetOnLoad().c_str() );
	node.WritePszProperty("OnSaveValue", l_pProgressBar->GetOnSave().c_str() );

	node.EndElement();

	CopyTexture( l_pProgressBar->GetProgressTexture(), path );
}

void CXMLManager::SaveSlider(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUISlider *l_pSlider = static_cast<CGUISlider*>(element);

	node.StartElement("Slider");

	WriteCommonProperties(node, l_pSlider);
	node.WriteFloatProperty("buttonH", l_pSlider->GetButtonHeight());
	node.WriteFloatProperty("buttonW", l_pSlider->GetButtonWidth());

	node.WritePszProperty("button_normal", GetNameTexture( l_pSlider->GetNormalButtonTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("button_over", GetNameTexture( l_pSlider->GetOverButtonTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("button_clicked", GetNameTexture( l_pSlider->GetClickedButtonTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("button_deactivated", GetNameTexture( l_pSlider->GetDeactivatedButtonTexture()->GetFileName() ).c_str() );
	node.WritePszProperty("quad", GetNameTexture( l_pSlider->GetBackgroundTexture()->GetFileName() ).c_str() );

	
	node.WritePszProperty("OnChangeValue", l_pSlider->GetOnChangeValue().c_str() );
	node.WritePszProperty("OnClickedAction", l_pSlider->GetOnClickedAction().c_str() );
	node.WritePszProperty("OnOverAction", l_pSlider->GetOnOverAction().c_str() );
	node.WritePszProperty("OnLoadValue", l_pSlider->GetOnLoad().c_str() );
	node.WritePszProperty("OnSaveValue", l_pSlider->GetOnSave().c_str() );

	node.EndElement();

	CopyTexture( l_pSlider->GetNormalButtonTexture(), path );
	CopyTexture( l_pSlider->GetOverButtonTexture(), path );
	CopyTexture( l_pSlider->GetClickedButtonTexture(), path );
	CopyTexture( l_pSlider->GetDeactivatedButtonTexture(), path );
	CopyTexture( l_pSlider->GetBackgroundTexture(), path );
}

void CXMLManager::SaveStaticText(CXMLTreeNode &node, CGuiElement *element, const std::string &path)
{
	CGUIStaticText *l_pStaticText = static_cast<CGUIStaticText*>(element);

	node.StartElement("StaticText");

	WriteCommonProperties( node, l_pStaticText );

	node.EndElement();
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