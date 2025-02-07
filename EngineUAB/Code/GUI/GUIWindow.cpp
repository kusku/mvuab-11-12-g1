#include "GUIWindow.h"
#include "InputManager.h"
#include "Textures\Texture.h"
#include "Textures\TextureManager.h"
#include "RenderManager.h"
#include "Logger\Logger.h"
#include "Scripting\ScriptManager.h"
#include "Core.h"
#include "Base.h"
#include "XML\XMLTreeNode.h"

//---Elementos que puede contener una Windows--
#include "Controls\GUIButton.h"
#include "Controls\GUICheckButton.h"
#include "Controls\GUISlider.h"
#include "Controls\GUIEditableTextBox.h"
#include "Controls\GUIDialogBox.h"
#include "Controls\GUIRadioBox.h"
#include "Controls\GUIImage.h"
#include "Controls\GUIAnimatedImage.h"
#include "Controls\GUIStaticText.h"
#include "Controls\GUIProgressBar.h"
//---------------------------------------------

CGUIWindow::~CGUIWindow()
{
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());	
	while (it != itEnd)
	{
		delete (*it);
		it++;
	}
	m_GuiElementsVector.clear();
}

bool CGUIWindow::AddGuiElement( CGuiElement * inGuiElement )
{
	//Primero comprobamos que no exista ya el guiElement a introducir
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	while (it != itEnd)
	{
		CGuiElement* guiElement = *it;
		if (guiElement == inGuiElement)
		{
			LOGGER->AddNewLog(ELL_ERROR, "Windows:: El GuiElement %s ya se ha introducido en la windows %s", inGuiElement->GetName().c_str(),m_sWindowsName.c_str());
			return false;
		}
		it++;
	}

	//Finalmente lo insertamos en el vector de GuiElements
	m_GuiElementsVector.push_back(inGuiElement);
	m_pCurrentSelectedElement = inGuiElement;
	return true;
}

bool CGUIWindow::ReleaseGuiElement( const std::string & name )
{
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());

	while (it != itEnd)
	{
		if( (*it)->GetName().compare(name) == 0 )
		{
			CGuiElement *element = (*it);
			m_GuiElementsVector.erase(it);

			CHECKED_DELETE(element);
			return true;
		}
		it++;
	}

	LOGGER->AddNewLog(ELL_WARNING, "Windows:: Se ha intentado eliminar el GuiElement %s pero no existe en la windows %s", name.c_str(),m_sWindowsName.c_str());
	return false;
}

void CGUIWindow::Render (CRenderManager *renderManager, CFontManager* fm)
{
	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());

	while (it != itEnd)
	{
    CGuiElement* guiElement = *it;
		guiElement->Render (renderManager, fm);
		it++;
	}
}


void CGUIWindow::Update (CInputManager* intputManager, float elapsedTime)
{

	if (m_sLuaCode_OnUpdateWindows.compare(""))
	{
		//Lanzar acci�n en Lua:
		CScriptManager* scriptManager = CORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnUpdateWindows);
		//scriptManager->RunScript(m_sLuaCode_OnUpdateWindows);
	}

	std::vector<CGuiElement*>::reverse_iterator revIt;
	std::vector<CGuiElement*>::reverse_iterator revItEnd(m_GuiElementsVector.rend());
	std::vector<CGuiElement*>::reverse_iterator revIt_aux;
	bool focus_aux = false;
	//Primero de todo quitamos el focus a todos los GuiElements 
	for( revIt = m_GuiElementsVector.rbegin(); revIt != revItEnd; ++revIt )
	{
		// De haber alguno que tenga focus nos lo guardamos, ya que al recalcular el focus si no lo tiene nadie
		// como m�nimo se lo damos al que lo ten�a.
		if( (*revIt)->HasFocus() )
		{
			focus_aux = true;
			revIt_aux = revIt;
		}

		(*revIt)->LoseFocus();
	}

	bool elemWithFocus = false;
	for( revIt = m_GuiElementsVector.rbegin(); revIt != revItEnd; ++revIt )
	{
		(*revIt)->Update(intputManager, elapsedTime);
		if( (*revIt)->HasFocus() )
		{
			elemWithFocus = true;
			break;
		}
	}

	if( elemWithFocus )
	{
		CGuiElement* guiElement = (*revIt);
		m_GuiElementsVector.erase(revIt.base() - 1);
		m_GuiElementsVector.push_back( guiElement );
	}
	else if( focus_aux )
	{
		(*revIt_aux)->GainFocus();
		(*revIt_aux)->Update(intputManager, elapsedTime);
	}

	IsKeyDown(intputManager);
}


void CGUIWindow::RegisterElements( std::map<std::string,CGuiElement*>& elements )
{
	std::map<std::string,CGuiElement*>::const_iterator it_aux;

	std::vector<CGuiElement*>::iterator it(m_GuiElementsVector.begin());
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	while (it != itEnd)
	{
		CGuiElement* element = (*it);
		//Antes de insertarlo en el mapa debemos comprobar que no este registrado ya un GuiElement con el mismo identificado(mismo string):
		if (elements.find(element->GetName()) != elements.end())
		{
			LOGGER->AddNewLog(ELL_ERROR, "CGUIWindow:: El GuiElement %s ya se ha registrado en el GuiManager", element->GetName().c_str());
		}
		else
		{
			elements.insert(std::pair<std::string,CGuiElement*>(element->GetName(), element));
		}
		it++;
	}
}

void CGUIWindow::LoadWindows( void )
{
	std::vector<CGuiElement*>::iterator it;
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	for( it = m_GuiElementsVector.begin(); it != itEnd; ++it )
	{
		CGuiElement * guiElement = (*it);
		guiElement->OnLoadValue();
	}
	
	if (m_sLuaCode_OnLoadWindows.compare(""))
	{
		//Lanzar acci�n en Lua:
		CScriptManager* scriptManager = CORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnLoadWindows);
		//scriptManager->RunScript(m_sLuaCode_OnLoadWindows);
	}
	
}

void CGUIWindow::SaveWindows( void )
{

	std::vector<CGuiElement*>::iterator it;
	std::vector<CGuiElement*>::iterator itEnd(m_GuiElementsVector.end());
	for( it = m_GuiElementsVector.begin(); it != itEnd; ++it )
	{
		CGuiElement * guiElement = (*it);
		guiElement->OnSaveValue();
	}

	if (m_sLuaCode_OnSaveWindows.compare(""))
	{
		//Lanzar acci�n en Lua:
		CScriptManager* scriptManager = CORE->GetScriptManager();
		scriptManager->RunCode(m_sLuaCode_OnSaveWindows);
		//scriptManager->RunScript(m_sLuaCode_OnSaveWindows);
	}
}

void CGUIWindow::IsKeyDown( CInputManager* intputManager )
{
	if (m_sLuaCode_OnKeyDown.compare(""))
	{
		//Comprobar si el input se cumple
		if (intputManager->IsUpDown(IDV_KEYBOARD,m_uInputKeyDown))
		{
			//Lanzar acci�n en Lua:
			CScriptManager* scriptManager = CORE->GetScriptManager();
			scriptManager->RunCode(m_sLuaCode_OnKeyDown);
			//scriptManager->RunScript(m_sLuaCode_OnKeyDown);
		}
	}
}


bool CGUIWindow::LoadXML( const std::string &xmlGuiFile, const Vect2i& screenResolution)
{
	//Read the xml gui file
	LOGGER->AddNewLog(ELL_INFORMATION, "CGUIWindow:: Iniciando el parseo del fichero %s", xmlGuiFile.c_str());
	bool isOK = false;

	CXMLTreeNode newFile;
	if (!newFile.LoadFile(xmlGuiFile.c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIWindow:: No se ha podido leer correctamente el fichero ->%s", xmlGuiFile.c_str());
		isOK = false;
	}
	else
	{
		CTextureManager* textureM = CORE->GetTextureManager();
		CXMLTreeNode  windows = newFile["Windows"];

		m_sLuaCode_OnLoadWindows		= windows.GetPszProperty("OnLoadWindows");
		m_sLuaCode_OnSaveWindows		= windows.GetPszProperty("OnSaveWindows");
		m_sLuaCode_OnUpdateWindows	= windows.GetPszProperty("OnUpdateWindows");


		if (windows.Exists())
		{
			int count = windows.GetNumChildren();
			for (int i = 0; i < count; ++i)
			{
				CXMLTreeNode pNewNode = windows(i);

				//Para cada guielement leemos su informacion comun-->
				std::string name	= pNewNode.GetPszProperty("name", "defaultGuiElement");
				float posx			= pNewNode.GetFloatProperty("posx", 0.f);
				float posy			= pNewNode.GetFloatProperty("posy", 0.f);
				float w				= pNewNode.GetFloatProperty("width", 50.f);
				float h				= pNewNode.GetFloatProperty("height", 50.f);
				bool visible		= pNewNode.GetBoolProperty("visible", true);
				bool activated		= pNewNode.GetBoolProperty("active", true);

				std::string tagName = pNewNode.GetName();
				if (tagName.compare("Button")==0)
				{
					CGUIButton* newButton = NULL;
					LoadButton(&newButton,pNewNode,screenResolution, textureM);
					AddGuiElement(newButton);
				}
				else if (tagName.compare("CheckButton")==0)
				{
					CGUICheckButton* new_checkButton = NULL;
					LoadCheckButton(&new_checkButton,pNewNode,screenResolution, textureM);									
					AddGuiElement(new_checkButton);
				}
				else if (tagName.compare("Slider")==0)
				{
					CGUISlider* new_slider = NULL;
					LoadSlider(&new_slider,pNewNode,screenResolution, textureM);					
					AddGuiElement(new_slider);
				}
				else if (tagName.compare("DialogBox")==0)
				{
					CGUIDialogBox* new_dialogBox = NULL;
					LoadDialogBox(&new_dialogBox,pNewNode,screenResolution, textureM);					
					AddGuiElement(new_dialogBox);
				}
				else if (tagName.compare("EditableTextBox")==0)
				{
					CGUIEditableTextBox* new_editableTextBox = NULL;
					LoadEditableTextBox(&new_editableTextBox,pNewNode,screenResolution, textureM);					
					AddGuiElement(new_editableTextBox);
				}
				else if (tagName.compare("RadioBox")==0)
				{
					CGUIRadioBox* new_radioBox = NULL;
					LoadRadioBox(&new_radioBox,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_radioBox);
				}
				else if (tagName.compare("Image")==0)
				{
					CGUIImage* new_image = NULL;
					_LoadImage(&new_image,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_image);
				}
				else if (tagName.compare("AnimatedImage")==0)
				{
					CGUIAnimatedImage* new_image = NULL;
					LoadAnimatedImage(&new_image,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_image);
				}
				else if (tagName.compare("ProgressBar")==0)
				{
					CGUIProgressBar* new_progressBar = NULL;
					LoadProgressBar(&new_progressBar,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_progressBar);
				} 
				else if (tagName.compare("StaticText")==0)
				{
					CGUIStaticText* new_staticText = NULL;
					LoadStaticText(&new_staticText,pNewNode,screenResolution, textureM);		
					AddGuiElement(new_staticText);
				}
				else if (tagName.compare("KeyBoard_Back")==0)
				{
					//<KeyBoard_Back input="DIK_A" OnKeyDown="blablaLua"/>
					m_sLuaCode_OnKeyDown	= pNewNode.GetPszProperty("OnKeyDown", "");
					m_uInputKeyDown				= pNewNode.GetIntProperty("input", 0);
				}
				else if (tagName.compare("comment")!=0)
				{
					//Warning
					LOGGER->AddNewLog(ELL_WARNING, "GUIWindow:: No se reconoce el tag %s del fichero %s", tagName.c_str(), xmlGuiFile.c_str());
				}

			}
			isOK = true;
		}
		else
		{
			LOGGER->AddNewLog(ELL_ERROR, "GUIWindow:: No se ha podido leer el tag Windows del fichero ->%s", xmlGuiFile.c_str());
			isOK = false;
		}

	}//END else de if (!newFile.LoadFile(xmlGuiFile.c_str()))


	if (!isOK)
	{
		LOGGER->AddNewLog(ELL_ERROR, "GUIWindow:: No se ha podido leer correctamente el fichero -->%s", xmlGuiFile.c_str());
		isOK =  false;
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "GUIWindow:: Finalizado correctamente el parseo el fichero %s", xmlGuiFile.c_str());

	}

	return isOK;
}


void CGUIWindow::LoadButton (CGUIButton** button_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<Button	name="play"	posx="0" posy="0" height="10" width="10" visible="true" activated="true" 
	// texture_normal="blabla" texture_over="bla" texture_clicked="bla" texture_deactivated="bla"
	// OnClickedAction="blabla" OnOverAction="blabla" Literal="blabla" widthOffset="" heightOffset=""/>

  CGUIButton* button;

	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	std::string texture_normal			= pNewNode.GetPszProperty("texture_normal", "");
	std::string texture_over				= pNewNode.GetPszProperty("texture_over", "");
	std::string texture_clicked			= pNewNode.GetPszProperty("texture_clicked", "");
	std::string texture_deactivated	= pNewNode.GetPszProperty("texture_deactivated", "");
	std::string OnClickedAction			= pNewNode.GetPszProperty("OnClickedAction", "");
	std::string OnOverAction				= pNewNode.GetPszProperty("OnOverAction", "");
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);
	std::string OnSaveValue						= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue						= pNewNode.GetPszProperty("OnLoadValue", "");

	CTexture* normal			= tm->GetTexture(texture_normal);
	CTexture* over				= tm->GetTexture(texture_over);
	CTexture* clicked			= tm->GetTexture(texture_clicked);
	CTexture* deactivated	= tm->GetTexture(texture_deactivated);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	button	= new CGUIButton(screenResolution.y,screenResolution.x, h, w, Vect2f(posx,posy), l_literal, heightOffset, widthOffset, visible, activated);
	button->SetName(name);
	button->SetTextures(normal,over,clicked,deactivated);
	button->SetOnClickedAction(OnClickedAction);
	button->SetOnOverAction(OnOverAction);

	button->SetOnLoadValueAction( OnLoadValue );
	button->SetOnSaveValueAction( OnSaveValue );

  *button_aux = button;
}

void CGUIWindow::LoadCheckButton (CGUICheckButton** checkButton_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<CheckButton name="debug_thps" posx="0" posy="2" height="10" width="10" visible="true" active="true" isOn="true"
	// texture_on="blabla" texture_off="bla" texture_deactivated"bla"
	// OnCheckOn="blabla" OnCheckOff="blabla" OnOverButton="blabla"
	// OnSaveValue="blabl" OnLoadValue="" Literal="blabla" widthOffset="" heightOffset=""/>

  CGUICheckButton* checkButton;

	std::string name									= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx									= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy									= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w											= pNewNode.GetFloatProperty("width", 50.f);
	float 			h											= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible								= pNewNode.GetBoolProperty("visible", true);
	bool 				activated							= pNewNode.GetBoolProperty("active", true);
	bool 				isOn									= pNewNode.GetBoolProperty("isOn", "");
	std::string texture_on						= pNewNode.GetPszProperty("texture_on", "");
	std::string texture_off						= pNewNode.GetPszProperty("texture_off", "");
	std::string texture_deactivated		= pNewNode.GetPszProperty("texture_deactivated", "");
	std::string OnCheckOn							= pNewNode.GetPszProperty("OnCheckOn", "");
	std::string OnCheckOff						= pNewNode.GetPszProperty("OnCheckOff", "");
	std::string OnOverButton					= pNewNode.GetPszProperty("OnOverButton", "");
	std::string OnSaveValue						= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue						= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string l_literal							= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent		= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent		= pNewNode.GetFloatProperty("heightOffset", 0.f);


	CTexture* on					= tm->GetTexture(texture_on);
	CTexture* off					= tm->GetTexture(texture_off);
	CTexture* deactivated	= tm->GetTexture(texture_deactivated);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	checkButton = new CGUICheckButton(screenResolution.y,screenResolution.x, h, w, Vect2f(posx,posy), isOn, l_literal, heightOffset, widthOffset, visible, activated);
	checkButton->SetName(name);
	checkButton->SetTextures(on, off, deactivated);
	checkButton->SetOnCheckOnAction(OnCheckOn);
	checkButton->SetOnCheckOffAction(OnCheckOff);
	checkButton->SetOnLoadValueAction(OnLoadValue);
	checkButton->SetOnSaveValueAction(OnSaveValue);
	checkButton->SetOnOverAction(OnOverButton);

  *checkButton_aux = checkButton;
}

void CGUIWindow::LoadSlider (CGUISlider** slider_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<Slider	name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" value="0.5" buttonH="10" buttonW="10"
	// button_normal="blabla" button_over="bla" button_clicked"bla" button_deactivated="bla" quad="bla"
	// OnOverAction="" OnClickedAction="" OnChangeValue="" OnSaveValue="blabl" OnLoadValue="" Literal="blabla" widthOffset="" heightOffset=""/>

  CGUISlider* slider;

	std::string name					= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float posx							= pNewNode.GetFloatProperty("posx", 0.f);
	float posy							= pNewNode.GetFloatProperty("posy", 0.f);
	float w								= pNewNode.GetFloatProperty("width", 50.f);
	float h								= pNewNode.GetFloatProperty("height", 50.f);
	bool visible						= pNewNode.GetBoolProperty("visible", true);
	bool activated						= pNewNode.GetBoolProperty("active", true);
	float value							= pNewNode.GetBoolProperty("value", 0.f);
	float buttonH						= pNewNode.GetFloatProperty("buttonH", 10.f);
	float buttonW						= pNewNode.GetFloatProperty("buttonW", 10.f);
	std::string button_normal			= pNewNode.GetPszProperty("button_normal", "");
	std::string button_over				= pNewNode.GetPszProperty("button_over", "");
	std::string button_clicked			= pNewNode.GetPszProperty("button_clicked", "");
	std::string button_deactivated		= pNewNode.GetPszProperty("button_deactivated", "");
	std::string quad					= pNewNode.GetPszProperty("quad", "");
	std::string OnChangeValue			= pNewNode.GetPszProperty("OnChangeValue", "");
	std::string OnClickedAction			= pNewNode.GetPszProperty("OnClickedAction", "");
	std::string OnOverAction			= pNewNode.GetPszProperty("OnOverAction", "");
	std::string OnSaveValue				= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue				= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string l_literal				= pNewNode.GetPszProperty("Literal", "");
	float widthOffsetPercent			= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float heightOffsetPercent			= pNewNode.GetFloatProperty("heightOffset", 0.f);

	CTexture* normal	= tm->GetTexture(button_normal);
	CTexture* over		= tm->GetTexture(button_over);
	CTexture* deac		= tm->GetTexture(button_deactivated);
	CTexture* clicked	= tm->GetTexture(button_clicked);
	CTexture* back		= tm->GetTexture(quad);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	slider = new CGUISlider(	screenResolution.y, screenResolution.x, h, w, Vect2f(posx,posy), buttonW, buttonH, value, 
												l_literal, heightOffset, widthOffset, visible, activated);
	slider->SetName(name);
	slider->SetButtonTextures(normal, over, clicked, deac);
	slider->SetBackGroundTexture(back);
	slider->SetOnChangeValueAction(OnChangeValue);
	slider->SetOnClickedAction(OnClickedAction);
	slider->SetOnOverAction(OnOverAction);
	slider->SetOnLoadValueAction(OnLoadValue);
	slider->SetOnSaveValueAction(OnSaveValue);

  *slider_aux = slider;
}

void	CGUIWindow::LoadDialogBox(CGUIDialogBox** dialogBox_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<DialogBox name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" buttonH="10" buttonW="10"
	// buttonClose_normal="blabla" buttonClose_over="bla" buttonClose_clicked"bla"  buttonClose_deactivated"bla" buttonMove_normal="blabla" buttonMove_over="bla"
	// buttonMove_clicked"bla" buttonMove_deactivated"bla" quad="bla" Literal="blabla" widthOffset="" heightOffset=""/>

  CGUIDialogBox* dialogBox;

	std::string name										= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx										= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy										= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w												= pNewNode.GetFloatProperty("width", 50.f);
	float 			h												= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible									= pNewNode.GetBoolProperty("visible", true);
	bool 				activated								= pNewNode.GetBoolProperty("active", true);
	float 			buttonH									= pNewNode.GetFloatProperty("buttonH", 10.f);
	float 			buttonW									= pNewNode.GetFloatProperty("buttonW", 10.f);
	std::string buttonClose_normal			= pNewNode.GetPszProperty("buttonClose_normal", "");
	std::string buttonClose_over				= pNewNode.GetPszProperty("buttonClose_over", "");
	std::string buttonClose_clicked			= pNewNode.GetPszProperty("buttonClose_clicked", "");
	std::string buttonClose_deactivated	= pNewNode.GetPszProperty("buttonClose_deactivated", "");
	std::string buttonMove_normal				= pNewNode.GetPszProperty("buttonMove_normal", "");
	std::string buttonMove_over					= pNewNode.GetPszProperty("buttonMove_over", "");
	std::string buttonMove_clicked			= pNewNode.GetPszProperty("buttonMove_clicked", "");
	std::string buttonMove_deactivated	= pNewNode.GetPszProperty("buttonMove_deactivated", "");
	std::string quad										= pNewNode.GetPszProperty("quad");
	std::string l_literal								= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent			= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent			= pNewNode.GetFloatProperty("heightOffset", 0.f);
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");

	CTexture* Close_normal			= tm->GetTexture(buttonClose_normal);
	CTexture* Close_over				= tm->GetTexture(buttonClose_over);
	CTexture* Close_clicked			= tm->GetTexture(buttonClose_clicked);
	CTexture* Close_deactivated	= tm->GetTexture(buttonClose_deactivated);
	CTexture* Move_normal				= tm->GetTexture(buttonMove_normal);
	CTexture* Move_over					= tm->GetTexture(buttonMove_over);
	CTexture* Move_clicked			= tm->GetTexture(buttonMove_clicked);
	CTexture* Move_deactivated	= tm->GetTexture(buttonMove_deactivated);
	CTexture* back							= tm->GetTexture(quad);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	dialogBox = new CGUIDialogBox(	screenResolution.y,screenResolution.x, h, w, Vect2f(posx,posy), buttonW, buttonH, l_literal, 
															heightOffset, widthOffset, visible, activated);
	dialogBox->SetName(name);
	dialogBox->SetCloseButtonTextures(Close_normal, Close_over, Close_clicked, Close_deactivated);
	dialogBox->SetMoveButtonTextures(Move_normal, Move_over, Move_clicked, Move_deactivated);
	dialogBox->SetDialogTexture(back);

	dialogBox->SetOnLoadValueAction( OnLoadValue );
	dialogBox->SetOnSaveValueAction( OnSaveValue );

  *dialogBox_aux = dialogBox;
}

void	CGUIWindow::LoadEditableTextBox (CGUIEditableTextBox** editableTextBox_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<EditableTextBox name="debug_fps" posx="0" posy="2" height="10" width="10" visible="true" active="true" 
	// texture_quad="blabla" id_font="0" color_font_r="0" color_font_g="0" color_font_b="0"
	// buffer="" OnSaveValue="blabla" OnLoadValue="blabla" Literal="blabla" widthOffset="" heightOffset=""
	// Literal="blabla" widthOffset="" heightOffset=""/>

  CGUIEditableTextBox* editableTextBox;

	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	std::string quad								= pNewNode.GetPszProperty("texture_quad", "");
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	uint32			idFont							= pNewNode.GetIntProperty("id_font", 0);
	float color_font_r				= pNewNode.GetFloatProperty("color_font_r", 0.f);
	float color_font_g				= pNewNode.GetFloatProperty("color_font_g", 0.f);
	float color_font_b				= pNewNode.GetFloatProperty("color_font_b", 0.f);
	float background_font_r			= pNewNode.GetFloatProperty("color_background_r", 0.f);
	float background_font_g			= pNewNode.GetFloatProperty("color_background_g", 0.f);
	float background_font_b			= pNewNode.GetFloatProperty("color_background_b", 0.f);
	std::string buffer							= pNewNode.GetPszProperty("buffer", "");
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	CTexture* quad_texture	= tm->GetTexture(quad);

	editableTextBox = new CGUIEditableTextBox(	screenResolution.y,screenResolution.x,h, w, Vect2f(posx,posy),
																					CColor(color_font_r,color_font_g,color_font_b),idFont, l_literal,
																					heightOffset, widthOffset, visible, activated);
	editableTextBox->SetName(name);
	editableTextBox->SetBackGroundTexture(quad_texture);
	editableTextBox->SetBuffer(buffer);
	editableTextBox->SetOnLoadValueAction(OnSaveValue);
	editableTextBox->SetOnSaveValueAction(OnLoadValue);
	editableTextBox->SetBackGroundColor( CColor( background_font_r, background_font_g, background_font_b ) );
  *editableTextBox_aux = editableTextBox;
}


void	CGUIWindow::LoadRadioBox(CGUIRadioBox** radioBox_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<RadioBox name="pepito" posx="0" posy="2" height="10" width="10" default_checkButton="bla" visible="true" active="true" texture_back=""
	// columns="3" rows="3" OnCheckOn="blabla" OnCheckOff="blabla" OnOverButton="blabla"
	// OnSaveValue="blabl" OnLoadValue=""	Literal="blabla" widthOffset="" heightOffset=""/>
	//	<texture name="q3dm1" on="gui_q3dm1_on.jpg" off="gui_q3dm1_off.jpg"  deactivated="gui_q3dm1_on.jpg" />
	//	<texture name="q3dm2" on="gui_q3dm2_on.jpg" off="gui_q3dm2_off.jpg" deactivated="gui_q3dm2_on.jpg" />
	//  ...
	//</RadioBox>

  CGUIRadioBox* radioBox;

	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float w										= pNewNode.GetFloatProperty("width", 50.f);
	float h										= pNewNode.GetFloatProperty("height", 50.f);
	bool visible							= pNewNode.GetBoolProperty("visible", true);
	bool activated						= pNewNode.GetBoolProperty("active", true);
	std::string default_checkButton = pNewNode.GetPszProperty("default_checkButton", "");
	std::string texture_back				= pNewNode.GetPszProperty("texture_back", "");
	uint32 columns							= pNewNode.GetIntProperty("columns", 0);
	uint32 rows								= pNewNode.GetIntProperty("rows", 0);
	std::string OnCheckOn						= pNewNode.GetPszProperty("OnCheckOn", "");
	std::string OnCheckOff					= pNewNode.GetPszProperty("OnCheckOff", "");
	std::string OnOverButton				= pNewNode.GetPszProperty("OnOverButton", "");
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	radioBox = new CGUIRadioBox(	screenResolution.y,screenResolution.x,h, w, Vect2f(posx,posy), columns, rows, default_checkButton, 
														l_literal, heightOffset, widthOffset, visible, activated);
	if (texture_back!="")
	{
		CTexture* quad	= tm->GetTexture(texture_back);
		radioBox->SetTextureBack(quad);
	}
	radioBox->SetName(name);
	radioBox->SetOnLoadValueAction(OnLoadValue);
	radioBox->SetOnSaveValueAction(OnSaveValue);
	radioBox->SetCheckButtonActions(OnCheckOn, OnCheckOff, OnOverButton);
	int count = pNewNode.GetNumChildren();
	for (int j = 0; j < count; ++j)
	{
		CXMLTreeNode pTexture = pNewNode(j);
		std::string tagName = pTexture.GetName();
		if (tagName.compare("texture")==0)
		{
			std::string name				= pTexture.GetPszProperty("name");
			std::string on					= pTexture.GetPszProperty("on");
			std::string off					= pTexture.GetPszProperty("off");
			std::string deactivated = pTexture.GetPszProperty("deactivated");

			CTexture* texture_on					= tm->GetTexture(on);
			CTexture* texture_off					= tm->GetTexture(off);
			CTexture* texture_deactivated	= tm->GetTexture(deactivated);
			radioBox->SetCheckButton(name, texture_on, texture_off, texture_deactivated);
		}
	}

  *radioBox_aux = radioBox;
}

void CGUIWindow::_LoadImage(CGUIImage** image_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<Image	name="imageRoomSelected_gameserver2" posx="35" posy="20" height="40" width="30" visible="true" active="true" 
	//default="q3dm1"	isAnimated="true", time="0" loop="true" OnSaveValue="blabla"  OnLoadValue="blabla"  Literal="blabla"
	//widthOffset="" heightOffset="" flip="" Literal="blabla" widthOffset="" heightOffset=""/>
	//	<texture name="q3dm1" texture="gui_q3dm1_off.jpg"/>
	//	<texture name="q3dm2" texture="gui_q3dm2_off.jpg"/>
	//	...
	//</Image>
	CGUIImage* image;
	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	std::string default_image				= pNewNode.GetPszProperty("default", "");
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string flip								= pNewNode.GetPszProperty("flip", "");
	bool				backGround					= pNewNode.GetBoolProperty("backGround", false);
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	image = new CGUIImage(	screenResolution.y,screenResolution.x, h, w, Vect2f(posx,posy),
											l_literal, heightOffset, widthOffset, visible, activated);
	image->SetName(name);
	image->SetActiveTexture(default_image);

	image->SetOnLoadValueAction(OnLoadValue);
	image->SetOnSaveValueAction(OnSaveValue);
	image->SetBackGround(backGround);
	if( flip.compare("FLIP_X") == 0)
	{
		image->SetFlip(FLIP_X);
	}
	else if (flip.compare("FLIP_Y") == 0)
	{
		image->SetFlip(FLIP_Y);
	}
	else
	{
		image->SetFlip(NONE_FLIP);
	}
	int count = pNewNode.GetNumChildren();

  if (count > 200)
  {
    for (int j = 0; j < count; j += 4)
	{
		CXMLTreeNode pTexture = pNewNode(j);
	    std::string tagName = pTexture.GetName();
	    if (tagName.compare("Texture")==0)
	    {	
		    std::string name				=  pNewNode(j).GetPszProperty("name");
		    std::string texture			=  pNewNode(j).GetPszProperty("name_texture");
			CTexture* texture_image		= tm->GetTexture(texture);
		    image->SetTexture(texture_image, name);
		}
	 }
  }
  else
  {
    for (int j = 0; j < count; ++j)
	  {
	    CXMLTreeNode pTexture = pNewNode(j);
	    std::string tagName = pTexture.GetName();
	    if (tagName.compare("texture")==0)
	    {	
		    std::string name		=  pNewNode(j).GetPszProperty("name");
		    std::string texture		=  pNewNode(j).GetPszProperty("name_texture");
			CTexture* texture_image	= tm->GetTexture(texture);
		    image->SetTexture(texture_image, name);
	    }
	  }
  }
	
  *image_aux = image;
}


void	CGUIWindow::LoadAnimatedImage(CGUIAnimatedImage** image_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<AnimatedImage	name="imageRoomSelected_gameserver2" posx="35" posy="20" height="40" width="30" visible="true" active="true" 
	//default="q3dm1"	isAnimated="true", time="0" loop="true" OnSaveValue="blabla"  OnLoadValue="blabla"  Literal="blabla"
	//widthOffset="" heightOffset="" flip="" Literal="blabla" widthOffset="" heightOffset=""/>
	//	<texture name="q3dm1" texture="gui_q3dm1_off.jpg"/>
	//	<texture name="q3dm2" texture="gui_q3dm2_off.jpg"/>
	//	...
	//</Image>
	CGUIAnimatedImage* image;
	std::string name								= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float 			posx								= pNewNode.GetFloatProperty("posx", 0.f);
	float 			posy								= pNewNode.GetFloatProperty("posy", 0.f);
	float 			w										= pNewNode.GetFloatProperty("width", 50.f);
	float 			h										= pNewNode.GetFloatProperty("height", 50.f);
	bool 				visible							= pNewNode.GetBoolProperty("visible", true);
	bool 				activated						= pNewNode.GetBoolProperty("active", true);
	uint16 			initFrame						= pNewNode.GetIntProperty("initFrame", true);
	bool        playOnLoad	  			= pNewNode.GetBoolProperty("playOnLoad", true);
	std::string OnSaveValue					= pNewNode.GetPszProperty("OnSaveValue", "");
	std::string OnLoadValue					= pNewNode.GetPszProperty("OnLoadValue", "");
	std::string flip								= pNewNode.GetPszProperty("flip", "");
	bool				backGround					= pNewNode.GetBoolProperty("backGround", false);
	std::string l_literal						= pNewNode.GetPszProperty("Literal", "");
	float				widthOffsetPercent	= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float				heightOffsetPercent	= pNewNode.GetFloatProperty("heightOffset", 0.f);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

  image = new CGUIAnimatedImage(	screenResolution.y,screenResolution.x, h, w, Vect2f(posx,posy),
											l_literal, heightOffset, widthOffset, visible, activated);
	image->SetName(name);
  
	image->SetOnLoadValueAction(OnLoadValue);
	image->SetOnSaveValueAction(OnSaveValue);
	image->SetBackGround(backGround);
	if( flip.compare("FLIP_X") == 0)
	{
		image->SetFlip(FLIP_X);
	}
	else if (flip.compare("FLIP_Y") == 0)
	{
		image->SetFlip(FLIP_Y);
	}
	else
	{
		image->SetFlip(NONE_FLIP);
	}
	int count = pNewNode.GetNumChildren();
	for (int j = 0; j < count; ++j)
	{
		CXMLTreeNode pTexture = pNewNode(j);
		std::string tagName = pTexture.GetName();
		if (tagName.compare("Texture")==0)
		{	
			std::string texture		=  pNewNode(j).GetPszProperty("name_texture");
			CTexture* texture_image	=  tm->GetTexture(texture);
			image->AddFrame(texture_image);
		}
	}

  image->GoToFrame(initFrame);
	
  if (playOnLoad)
  {
	//image->PlayAnimation();
  }

  *image_aux = image;
}


void	CGUIWindow::LoadProgressBar(CGUIProgressBar** progressBar_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<ProgressBar name="progressBar1" posx="200" posy="600" height="10" width="100" visible="true" active="true"
	//	texture_back="hola" texture_bar="hola2"  id_font="0" color_font_r="0" color_font_g="0" color_font_b="0"
	//	Literal="blabla" widthOffset="" heightOffset="" OnComplete="blabla"/>

	CGUIProgressBar* progressBar;
	std::string name				= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float posx						= pNewNode.GetFloatProperty("posx", 0.f);
	float posy						= pNewNode.GetFloatProperty("posy", 0.f);
	float w							= pNewNode.GetFloatProperty("width", 50.f);
	float h							= pNewNode.GetFloatProperty("height", 50.f);
	bool visible					= pNewNode.GetBoolProperty("visible", true);
	bool activated					= pNewNode.GetBoolProperty("active", true);
	std::string texture_bar			= pNewNode.GetPszProperty("texture_bar", "");
	std::string texture_back		= pNewNode.GetPszProperty("texture_back", "");
	std::string OnComplete			= pNewNode.GetPszProperty("OnComplete", "");
	uint32 idFont					= pNewNode.GetIntProperty("id_font", 0);
	float color_font_r				= pNewNode.GetFloatProperty("color_font_r", 0.f);
	float color_font_g				= pNewNode.GetFloatProperty("color_font_g", 0.f);
	float color_font_b				= pNewNode.GetFloatProperty("color_font_b", 0.f);
	float color_background_r		= pNewNode.GetFloatProperty("color_background_r", 0.f);
	float color_background_g		= pNewNode.GetFloatProperty("color_background_g", 0.f);
	float color_background_b		= pNewNode.GetFloatProperty("color_background_b", 0.f);
	float color_progress_r			= pNewNode.GetFloatProperty("color_progress_r", 0.f);
	float color_progress_g			= pNewNode.GetFloatProperty("color_progress_g", 0.f);
	float color_progress_b			= pNewNode.GetFloatProperty("color_progress_b", 0.f);
	std::string l_literal			= pNewNode.GetPszProperty("Literal", "");
	float widthOffsetPercent		= pNewNode.GetFloatProperty("widthOffset", 0.f);
	float heightOffsetPercent		= pNewNode.GetFloatProperty("heightOffset", 0.f);

	CTexture* bar					= tm->GetTexture(texture_bar);
	CTexture* back				= tm->GetTexture(texture_back);

	uint32 widthOffset	= (uint32) (screenResolution.x	* 0.01f * widthOffsetPercent );
	uint32 heightOffset	= (uint32) (screenResolution.y	* 0.01f * heightOffsetPercent );

	progressBar = new CGUIProgressBar(	screenResolution.y,screenResolution.x, h, w, Vect2f(posx, posy), 
																	l_literal, heightOffset, widthOffset, visible, activated);
	progressBar->SetName(name);
	progressBar->SetTextures(back, bar);
	progressBar->SetFont(idFont, CColor(color_font_r,color_font_g,color_font_b));
	progressBar->SetColors( CColor(color_background_r, color_background_g, color_background_b), CColor(color_progress_r, color_progress_g, color_progress_b));
	progressBar->SetOnComplete(OnComplete);

  *progressBar_aux = progressBar;
}

void	CGUIWindow::LoadStaticText(CGUIStaticText** staticText_aux, CXMLTreeNode& pNewNode, const Vect2i& screenResolution, CTextureManager* tm)
{
	//<StaticText name="pepito" posx="0" posy="2" literal="blabla" visible="true" active="true"/>
	
	CGUIStaticText* staticText;

	std::string name			= pNewNode.GetPszProperty("name", "defaultGuiElement");
	float posx					= pNewNode.GetFloatProperty("posx", 0.f);
	float posy					= pNewNode.GetFloatProperty("posy", 0.f);
	float w						= pNewNode.GetFloatProperty("width", 50.f);
	float h						= pNewNode.GetFloatProperty("height", 50.f);
	bool visible				= pNewNode.GetBoolProperty("visible", true);
	bool activated				= pNewNode.GetBoolProperty("active", true);
	std::string l_literal		= pNewNode.GetPszProperty("Literal", "");
	
	staticText = new CGUIStaticText(screenResolution.y, screenResolution.x, h, w, Vect2f(posx,posy), l_literal, visible, activated);
	staticText->SetName(name);

  *staticText_aux = staticText;
}

CGuiElement* CGUIWindow::GetElement( const std::string &name )
{
	std::vector<CGuiElement*>::iterator it = m_GuiElementsVector.begin();
	std::vector<CGuiElement*>::iterator itEnd = m_GuiElementsVector.end();

	for( ; it != itEnd; ++it )
	{
		if( (*it)->GetName() == name )
		{
			return *it;
		}
	}

	return NULL;
}

void CGUIWindow::MoveElementToFront(CGuiElement *element)
{
	std::vector<CGuiElement*>::iterator it = m_GuiElementsVector.begin();
	std::vector<CGuiElement*>::iterator itEnd = m_GuiElementsVector.end();

	while( it != itEnd )
	{
		if( *it == element )
		{
			m_GuiElementsVector.erase(it);
			m_GuiElementsVector.push_back(element);
			break;
		}

		++it;
	}
}

void CGUIWindow::MoveElementForward(CGuiElement *element)
{
	std::vector<CGuiElement*>::iterator it = m_GuiElementsVector.begin();
	std::vector<CGuiElement*>::iterator itEnd = m_GuiElementsVector.end();
	std::vector<CGuiElement*>::iterator l_NextIt = m_GuiElementsVector.begin() + 1;

	while( it != itEnd )
	{
		if( *it == element )
		{
			if( l_NextIt != itEnd )
			{
				m_GuiElementsVector.erase(it);
				m_GuiElementsVector.insert(l_NextIt, element);
			}
			break;
		}

		++it;
		l_NextIt = it + 1;
	}
}

void CGUIWindow::MoveElementBack(CGuiElement *element)
{
	std::vector<CGuiElement*>::iterator it = m_GuiElementsVector.begin();
	std::vector<CGuiElement*>::iterator itEnd = m_GuiElementsVector.end();
	std::vector<CGuiElement*>::iterator l_PrevIt = m_GuiElementsVector.begin();

	while( it != itEnd )
	{
		if( *it == element )
		{
			if( l_PrevIt != it )
			{
				m_GuiElementsVector.erase(it);
				m_GuiElementsVector.insert(l_PrevIt, element);
			}
			break;
		}
		l_PrevIt = it;
		++it;
	}
}

void CGUIWindow::MoveElementToBack(CGuiElement *element)
{
	std::vector<CGuiElement*>::iterator it = m_GuiElementsVector.begin();
	std::vector<CGuiElement*>::iterator itEnd = m_GuiElementsVector.end();

	for( ; it != itEnd; ++it )
	{
		if( *it == element )
		{
			m_GuiElementsVector.erase(it);
			m_GuiElementsVector.insert( m_GuiElementsVector.begin(), element);
			break;
		}
	}
}

void CGUIWindow::ClearSelectElements()
{
	if( m_GuiElementsVector.size() > 0)
	{
		std::vector<CGuiElement*>::iterator it = m_GuiElementsVector.begin();
		std::vector<CGuiElement*>::iterator itEnd = m_GuiElementsVector.end();

		for( ; it != itEnd; ++it )
		{
			(*it)->SetIsSelected( false );
		}
	}
}