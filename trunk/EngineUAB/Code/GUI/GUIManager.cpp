#include "GUIManager.h"
#include "Core.h"
#include "Base.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "Logger/Logger.h"
#include "GUIWindow.h"
#include "XML/XMLTreeNode.h"
#include "Textures/TextureManager.h"
#include "ScriptManager.h"
#include "GraphicsDefs.h"

//--Includes GuiElements---
#include "Exceptions/Exception.h"
#include "Controls\GUITextBox.h"
#include "Controls\GUIPointerMouse.h"
#include "Controls\GUIButton.h"
#include "Controls\GUICheckButton.h"
#include "Controls\GUIDialogBox.h"
#include "Controls\GUIEditableTextBox.h"
#include "Controls\GUIImage.h"
#include "Controls\GUITextBox.h"
#include "Controls\GUIProgressBar.h"
#include "Controls\GUIRadioBox.h"
#include "Controls\GUISlider.h"
#include "Controls\GUIStatictext.h"
//-------------------------

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
CGUIManager::CGUIManager(const Vect2i& resolution)
: m_sCurrentWindows("Main.xml")
, m_TextBox(NULL)
, m_PointerMouse(NULL)
, m_bRenderError(false)
, m_bUpdateError(false)
, m_ScreenResolution(resolution)
, m_bIsOk(false)
, m_bLoadedGuiFiles(false)
, m_sLastLoadpathGUI_XML("")
, m_bFirstUpdate(true)
, m_bVisiblePointerMouse(true)
{
  int jorls  = 0;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CGUIManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CGUIManager::Release () 
{
	LOGGER->AddNewLog(ELL_INFORMATION, "GUIManager:: shutting down GUI");

	std::map<std::string, CGUIWindow*>::iterator it;
	std::map<std::string, CGUIWindow*>::iterator itEnd(m_WindowsMap.end());
	for( it = m_WindowsMap.begin(); it != itEnd; ++it)
	{
		CGUIWindow* windows = it->second;
		CHECKED_DELETE(windows);
	}
	m_WindowsMap.clear();

	CHECKED_DELETE(m_TextBox);
	CHECKED_DELETE(m_PointerMouse);

	LOGGER->AddNewLog(ELL_INFORMATION, "GUIManager:: offline (ok)");
}

//----------------------------------------------------------------------------
// Init
//----------------------------------------------------------------------------
bool CGUIManager::Init (const std::string& initGuiXML)
{
	
	m_bIsOk = false;

	LOGGER->AddNewLog(ELL_INFORMATION, "CGUIManager:: calling initialization");

	CXMLTreeNode parser;
	if (!parser.LoadFile(initGuiXML.c_str()))
	{
		std::string msg_error = "CGUIManager::Init-> Error al leer el archivo de configuracion GUI: " + initGuiXML;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
	else
	{
		CTextureManager* textureM = CORE->GetTextureManager();

		CXMLTreeNode  m = parser["GuiFiles"];
		if (m.Exists())
		{
			std::string path = m.GetPszProperty("path","");
			if (path.compare("") != 0)
				m_bIsOk = LoadGuiFiles(path);
			else
			{
				m_bIsOk = true;
			}
		}
		else
		{
			std::string msg_error = "CGUIManager::Init-> Error al intentar leer el tag <GuiFiles> del archivo de configuracion GUI: " + initGuiXML;
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			throw CException(__FILE__, __LINE__, msg_error);
		}

		if (m_bIsOk)
		{
			m = parser["TextBox"];
			if (m.Exists())
			{
				float posx								= m.GetFloatProperty("posx",					30.f);
				float posy								= m.GetFloatProperty("posy",					30.f);
				float w									= m.GetFloatProperty("width",					35.f);
				float h									= m.GetFloatProperty("height",					35.f);
				float button_w							= m.GetFloatProperty("button_width",			5.f);
				float button_h							= m.GetFloatProperty("button_height",			5.f);
				std::string buttonClose_normal			= m.GetPszProperty("buttonClose_normal",		"./Data/General/Textures/CloseDialegBox.jpg");
				std::string buttonClose_over			= m.GetPszProperty("buttonClose_over",			"./Data/General/Textures/CloseDialegBoxO.jpg");
				std::string buttonClose_clicked			= m.GetPszProperty("buttonClose_clicked",		"./Data/General/Textures/CloseDialegBoxC.jpg");
				std::string buttonClose_deactivated		= m.GetPszProperty("buttonClose_deactivated",	"./Data/General/Textures/CloseDialegBoxC.jpg" );
				std::string buttonMove_normal			= m.GetPszProperty("buttonMove_normal",			"./Data/General/Textures/ButtonDialegBoxN.jpg");
				std::string buttonMove_over				= m.GetPszProperty("buttonMove_over",			"./Data/General/Textures/ButtonDialegBoxO.jpg");
				std::string buttonMove_clicked			= m.GetPszProperty("buttonMove_clicked",		"./Data/General/Textures/ButtonDialegBoxC.jpg");
				std::string buttonMove_deactivated		= m.GetPszProperty("buttonMove_deactivated",	"./Data/General/Textures/ButtonDialegBoxC.jpg");
				std::string quad						= m.GetPszProperty("quad",						"./Data/General/Textures/BaseDialegBox.jpg");

				CTexture* Close_normal					= textureM->GetTexture(buttonClose_normal);
				CTexture* Close_over					= textureM->GetTexture(buttonClose_over);
				CTexture* Close_clicked					= textureM->GetTexture(buttonClose_clicked);
				CTexture* Close_deactivated				= textureM->GetTexture(buttonClose_deactivated);
				CTexture* Move_normal					= textureM->GetTexture(buttonMove_normal);
				CTexture* Move_over						= textureM->GetTexture(buttonMove_over);
				CTexture* Move_clicked					= textureM->GetTexture(buttonMove_clicked);
				CTexture* Move_deactivated				= textureM->GetTexture(buttonMove_deactivated);
				CTexture* back							= textureM->GetTexture(quad);

				m_TextBox =	new CGUITextBox(m_ScreenResolution.x, m_ScreenResolution.y, h, w, Vect2f(posx,posy), button_w, button_h);
				assert(m_TextBox);
				m_TextBox->SetName("TextBox");
				m_TextBox->SetCloseButtonTextures(Close_normal, Close_over, Close_clicked, Close_deactivated);
				m_TextBox->SetMoveButtonTextures(Move_normal, Move_over, Move_clicked, Move_deactivated);
				m_TextBox->SetDialogTexture(back);
				m_TextBox->SetVisible(false);
			}
			else
			{
				std::string msg_error = "CGUIManager::Init-> Error al intentar leer el tag <TextBox> del archivo de configuracion GUI: " + initGuiXML;
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
				throw CException(__FILE__, __LINE__, msg_error);
			}

			m = parser["PointerMouse"];
			if (m.Exists())
			{
				float posx													= m.GetFloatProperty("posx",			5.f);
				float posy													= m.GetFloatProperty("posy",			5.f);
				float w															= m.GetFloatProperty("width",			5.f);
				float h															= m.GetFloatProperty("height",		5.f);
				std::string texture	= m.GetPszProperty("texture",			"./Data/General/Textures/gui_pointer_mouse.tga");
				bool isQuadrant											= m.GetBoolProperty("isQuadrant",	true);
				
				CTexture* texture_pointer = textureM->GetTexture(texture);
				
				m_PointerMouse	= new CGUIPointerMouse(m_ScreenResolution.y,m_ScreenResolution.x, h,w,Vect2f(posx,posy));
				assert(m_PointerMouse);
				m_PointerMouse->SetTexture(texture_pointer,"default");
				m_PointerMouse->SetActiveTexture("default");
				m_PointerMouse->SetQuadrant(isQuadrant);
			}
			else
			{
				std::string msg_error = "CGUIManager::Init-> Error al intentar leer el tag <PointerMouse> del archivo de configuracion GUI: " + initGuiXML;
				LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
				throw CException(__FILE__, __LINE__, msg_error);
			}

			m_bIsOk = m_TextBox && m_PointerMouse;
			
		}//END if (m_bIsOk)
		
	} //END if (!parser.LoadFile(initGuiXML.c_str()))

	if (!m_bIsOk)
	{
		Release();
	}
	else
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CSoundManager:: online (ok)");
	}

	return m_bIsOk;

}

//----------------------------------------------------------------------------
// Render
//----------------------------------------------------------------------------
void CGUIManager::Render (CRenderManager *renderManager, CFontManager* fm)
{
	if (m_bIsOk)
	{
    CORE->GetRenderManager()->EnableAlphaBlend();
  
		if (m_bLoadedGuiFiles)
		{
			std::map<std::string, CGUIWindow*>::iterator it;
			it = m_WindowsMap.find( m_sCurrentWindows );
			if( it != m_WindowsMap.end() )
			{
				CGUIWindow * currentWindows = it->second;
				currentWindows->Render(renderManager, fm);
				RenderTransitionEffect(renderManager);
				m_bRenderError = false;
			}
			else
			{
				if (!m_bRenderError)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Se ha intentado pintar la windows %s no registrada ", m_sCurrentWindows.c_str());
					m_bRenderError = true;
				}
			}
		}//END if (m_bLoadedGuiFiles)
		
		//Siempre los últimos en pintarse
		assert(m_TextBox);
		m_TextBox->Render(renderManager, fm);
    RenderPointerMouse(renderManager, fm);
		
    CORE->GetRenderManager()->DisableAlphaBlend();
	}//END if (m_bIsOk)
}


void	CGUIManager::RenderPointerMouse	(CRenderManager *renderManager, CFontManager* fontManager)
{
	if(m_bVisiblePointerMouse)
			m_PointerMouse->Render(renderManager, fontManager);
}
//----------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------

void CGUIManager::Update (float elapsedTime)
{
	if (m_bIsOk)
	{
		assert(m_TextBox);
		assert(m_PointerMouse);

		//Si es la primera vez que actualizamos la GUI debemos hacer un load de la main.xml:
		if (m_bFirstUpdate)
		{
			std::map<std::string, CGUIWindow*>::iterator itCurrentWindows;
			itCurrentWindows = m_WindowsMap.find( m_sCurrentWindows );
			if( itCurrentWindows != m_WindowsMap.end() )
			{
				CGUIWindow * currentWindow = itCurrentWindows->second;
				currentWindow->LoadWindows();
			}
			m_bFirstUpdate = false;
		}

		CInputManager* intputManager = CORE->GetInputManager();

		m_PointerMouse->Update(intputManager, elapsedTime);

		m_TextBox->Update(intputManager, elapsedTime);


		if( !m_TextBox->IsVisible() && m_bLoadedGuiFiles)
		{
			std::map<std::string, CGUIWindow*>::iterator it;
			it = m_WindowsMap.find( m_sCurrentWindows );
			if( it != m_WindowsMap.end() )
			{
				CGUIWindow * currentWindow = it->second;
				if (!UpdateTransitionEffect(elapsedTime))
				{
					currentWindow->Update(intputManager, elapsedTime);
				}
				m_bUpdateError = false;
			}
			else
			{
				if (!m_bUpdateError)
				{
					LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Se ha intentado updatear la windows %s no registrada ", m_sCurrentWindows.c_str());
					m_bUpdateError = true;
				}
			}
		}//END if( !m_TextBox.IsVisible() )
	}
}

void CGUIManager::RenderTransitionEffect(CRenderManager *renderManager)
{
	if (m_sTransitionEffect.m_bDoEffect)
	{
		switch (m_sTransitionEffect.m_eType)
		{
			case TE_SHADOW:
			{
				//Dibujamos un quad2d en toda la pantalla:
				//	- Durante la primera mitad de tiempo irá de totalmente transparente a totalmente opaco negro
				//	- Durante la segunda mitad de tiempo irá de totalmente opaco negro a totalmente transparente
				CColor color = colBLACK;
				float alpha;
				if (m_sTransitionEffect.m_fTimeCounter < m_sTransitionEffect.m_fTransitionTime*0.5f)
				{
					//Durante la primera mitad del tiempo: alpha de 0.f -> 1.f
					alpha = m_sTransitionEffect.m_fTimeCounter / (m_sTransitionEffect.m_fTransitionTime*0.5f);
					color.SetAlpha(alpha);
				}
				else
				{
					//Durante la segunda mitad del tiempo: alpha de 1.f -> 0.f
					alpha = m_sTransitionEffect.m_fTimeCounter / (m_sTransitionEffect.m_fTransitionTime*0.5f); //esto va de 1->2
					color.SetAlpha(abs(alpha-2));
				}
				renderManager->DrawQuad2D(Vect2i(0,0),m_ScreenResolution.x,m_ScreenResolution.y,UPPER_LEFT,color);
			}
			break;
		
			case TE_FADE_TO_BLACK:
			{
				//Dibujamos un quad2d en toda la pantalla:
				//	- Irá de totalmente transparente a totalmente opaco negro
				CColor color = colBLACK;

        if (m_sTransitionEffect.m_fTimeCounter < m_sTransitionEffect.m_fTransitionTime)
				{
					// alpha de 0.f -> 1.f
					float alpha = m_sTransitionEffect.m_fTimeCounter / m_sTransitionEffect.m_fTransitionTime;
					color.SetAlpha(alpha);
				}
        else
        {
          color.SetAlpha(0.f);
        }

				renderManager->DrawQuad2D(Vect2i(0,0),m_ScreenResolution.x,m_ScreenResolution.y,UPPER_LEFT,color);
			}
			break;
		
			case TE_SHADOW_OFF:
			{
				//Dibujamos un quad2d en toda la pantalla:
				//	- Irá de totalmente opaco negro a totalmente transparente 
				CColor color = colBLACK;

        if (m_sTransitionEffect.m_fTimeCounter < m_sTransitionEffect.m_fTransitionTime)
				{
					// alpha de 1.f -> 0.f
					float alpha = m_sTransitionEffect.m_fTimeCounter / m_sTransitionEffect.m_fTransitionTime;
					color.SetAlpha(abs(alpha - 1));
				}

				renderManager->DrawQuad2D(Vect2i(0,0),m_ScreenResolution.x,m_ScreenResolution.y,UPPER_LEFT,color);
			}
			break;
		
			default:
			{
				LOGGER->AddNewLog(ELL_ERROR,"CGUIManager::RenderTransitionEffect-> No se reconoce el efecto a realizar!");
			}
		}
	}
}

bool CGUIManager::UpdateTransitionEffect (float elapsedTime)
{
	if (m_sTransitionEffect.m_bDoEffect)
	{
		m_sTransitionEffect.m_fTimeCounter += elapsedTime;
    
    switch (m_sTransitionEffect.m_eType)
    {
      case TE_SHADOW:
			{
        if (!m_sTransitionEffect.m_bActiveWindows && m_sTransitionEffect.m_fTimeCounter > m_sTransitionEffect.m_fTransitionTime*0.5f)
		    {
			    ActiveWindows(m_sTransitionEffect.m_sWindowsName);
			    m_sTransitionEffect.m_bActiveWindows = true;

		    }
		    if (m_sTransitionEffect.m_fTimeCounter > m_sTransitionEffect.m_fTransitionTime)
		    {
			    m_sTransitionEffect.m_bDoEffect = false;
			    m_sTransitionEffect.m_fTimeCounter = 0.f;
			    m_sTransitionEffect.m_bActiveWindows = false;
		    }
      }
      break;

      case TE_FADE_TO_BLACK:
			{
        if (m_sTransitionEffect.m_fTimeCounter >= m_sTransitionEffect.m_fTransitionTime)
		    {
          ActiveWindows(m_sTransitionEffect.m_sWindowsName);
			    m_sTransitionEffect.m_bDoEffect = false;
			    m_sTransitionEffect.m_fTimeCounter = 0.f;
			    m_sTransitionEffect.m_bActiveWindows = true;
        }
      }
      break;

      case TE_SHADOW_OFF:
			{
        if (!m_sTransitionEffect.m_bActiveWindows)
        {
          ActiveWindows(m_sTransitionEffect.m_sWindowsName);
			    m_sTransitionEffect.m_bActiveWindows = true;
        }

        if (m_sTransitionEffect.m_fTimeCounter >= m_sTransitionEffect.m_fTransitionTime)
		    {
          //ActiveWindows(m_sTransitionEffect.m_sWindowsName);
			    m_sTransitionEffect.m_bDoEffect = false;
			    m_sTransitionEffect.m_fTimeCounter = 0.f;
			    //m_sTransitionEffect.m_bActiveWindows = true;
        }
      }
      break;
    }
		
		return true;
	}
	
	return false;
}

void CGUIManager::ActiveWindowsWithEffect	(const std::string& inNameWindow, EtypeTransitionEffect type, float transitionTime )
{
	m_sTransitionEffect.m_bActiveWindows	= false; 
	m_sTransitionEffect.m_bDoEffect				= true;
	m_sTransitionEffect.m_eType						= type;
	m_sTransitionEffect.m_fTransitionTime	= transitionTime;
	m_sTransitionEffect.m_fTimeCounter		= 0.f;
	m_sTransitionEffect.m_sWindowsName		= inNameWindow;
}

void CGUIManager::ActiveWindows( const std::string& inNameWindow )
{
	std::map<std::string, CGUIWindow*>::iterator it;
	it = m_WindowsMap.find( inNameWindow );
	if( it != m_WindowsMap.end() )
	{
		//Primero finalizamos la ventana actual
		std::map<std::string, CGUIWindow*>::iterator itCurrentWindows;
		itCurrentWindows = m_WindowsMap.find( m_sCurrentWindows );
		if( itCurrentWindows != m_WindowsMap.end() )
		{
			CGUIWindow * currentWindow = itCurrentWindows->second;
			currentWindow->SaveWindows();

			//A continuación leemos los valores de la nueva ventana
			CGUIWindow* windows = it->second;
			windows->LoadWindows();
			m_sCurrentWindows = inNameWindow;
		}
		else
		{
			LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Al intentar cambiar de windows, la actual-->%s no se ha encontrado registrada", m_sCurrentWindows.c_str());
		}
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Al intentar cambiar a la windows-->%s esta no se ha encontrado registrada", inNameWindow.c_str());
	}
}


void CGUIManager::PushWindows (const std::string& inNameWindow )
{
  std::map<std::string, CGUIWindow*>::iterator it;
  it = m_WindowsMap.find( inNameWindow );
  if( it != m_WindowsMap.end() )
  {
		m_PrevWindows.push_back(m_sCurrentWindows);
		ActiveWindows(inNameWindow);    
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::PushWindows Al intentar cambiar a la windows-->%s esta no se ha encontrado registrada", inNameWindow.c_str());
  }
}

void CGUIManager::PopWindows ()
{
  if (m_PrevWindows.size() == 0)
  {
    LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::PopWindows -> El vector de PrevWindows esta vacío!");
  }
  else
  {
    std::string popWindows = m_PrevWindows[m_PrevWindows.size()-1];
    m_PrevWindows.pop_back();
    ActiveWindows(popWindows);
  }
}

void CGUIManager::AddWindows(const std::string& inNameWindow)
{
	std::map<std::string, CGUIWindow*>::iterator it;
	it = m_WindowsMap.find( inNameWindow );
	if( it == m_WindowsMap.end() )
	{
		CGUIWindow *window = new CGUIWindow();
		window->SetName(inNameWindow);
		window->RegisterElements(m_ElementsMap);
		m_WindowsMap[ inNameWindow ] = window;
	}
}

void CGUIManager::SetScreenResolution(const Vect2i& resolution)
{
	std::map<std::string, CGuiElement*>::iterator it(m_ElementsMap.begin());
	std::map<std::string, CGuiElement*>::iterator itEnd(m_ElementsMap.end());

	while (it != itEnd)
	{
		CGuiElement* guiElement = it->second;
		guiElement->SetWindowsWidth(resolution.x);
		guiElement->SetWindowsHeight(resolution.y);
		it++;
	}
	
}

bool CGUIManager::LoadGuiFiles (const std::string& pathGUI_XML)
{
	m_bLoadedGuiFiles = false;
	LOGGER->AddNewLog(ELL_INFORMATION, "GUIManager::LoadGuiFiles-> Empezando a leer los .xml del directorio->%s",pathGUI_XML.c_str());
	//Read xml files:
	std::map<std::string, CGUIWindow*>::iterator it(m_WindowsMap.begin());
	std::map<std::string, CGUIWindow*>::iterator itEnd(m_WindowsMap.end());
	while (it != itEnd)
	{
		CGUIWindow* windows = it->second;
		CHECKED_DELETE(windows);
		++it;
	}

	m_WindowsMap.clear();
	m_ElementsMap.clear();
	m_sCurrentWindows = "Main";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	// We read the XmlGui files	
	m_sLastLoadpathGUI_XML = pathGUI_XML;
	std::string path_xmls = pathGUI_XML+"/*.xml";
	hFind = FindFirstFile(path_xmls.c_str(), &FindFileData);

	// we check the existence of the XmlGui directory
	if ( hFind == INVALID_HANDLE_VALUE ) 
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager:: Error al intentar leer los .xml del directorio %s", pathGUI_XML.c_str());
		return false;
	}
	else 
	{
		std::string FileName = FindFileData.cFileName;
		CGUIWindow* windows = new CGUIWindow();
		std::string pathFile = pathGUI_XML+"/";
		pathFile += FileName;
		bool isOk = false;
		isOk = windows->LoadXML(pathFile,m_ScreenResolution);
		if (!isOk)
		{
			return false;
		}
		windows->RegisterElements(m_ElementsMap);

		int find = FileName.rfind(".");
		FileName = FileName.substr(0, find);

		windows->SetName(FileName);
		m_WindowsMap.insert( std::pair<std::string,CGUIWindow*>(FileName,windows) );

		while( FindNextFile(hFind, &FindFileData) != 0 )
		{
			std::string FileName = FindFileData.cFileName;
			CGUIWindow* windows = new CGUIWindow();
			std::string pathFile = pathGUI_XML+"/";
			pathFile += FileName;
			isOk = windows->LoadXML(pathFile,m_ScreenResolution);
			if (!isOk)
			{
				return false;
			}
			windows->RegisterElements(m_ElementsMap);

			find = FileName.rfind(".");
			FileName = FileName.substr(0, find);

			windows->SetName(FileName);
			m_WindowsMap.insert( std::pair<std::string,CGUIWindow*>(FileName,windows) );
		}

		FindClose(hFind);
	}	

	m_bLoadedGuiFiles = true;
	return true;
}

bool CGUIManager::ReloadGuiFiles ()
{
	bool isOk = false;
	std::string windows = m_sCurrentWindows;
	isOk = LoadGuiFiles(m_sLastLoadpathGUI_XML);
	
	std::map<std::string, CGUIWindow*>::iterator it;
	it = m_WindowsMap.find( windows );
	if (it != m_WindowsMap.end())
	{
		m_sCurrentWindows = windows;
	}
	
	return isOk;
}

void CGUIManager::SetMessageBox( const std::string& text )
{
	if (m_bIsOk)
	{
		assert(m_TextBox);
		if( !m_TextBox->IsVisible() )
		{
			m_TextBox->SetMessage( text );
			m_TextBox->SetVisible( true );
			m_TextBox->SetActive( true );
			m_TextBox->SetPosition(Vect2i(30,30));
		}
	}
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//				Funciones para modificar los GuiElements
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void CGUIManager::SetActiveGuiElement (const std::string& inNameGuiElement, bool flag)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		it->second->SetActive(flag);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetActiveGuiElement-> No se ha encontrado el guiElement %s al ejecutar la funcion SetActiveGuiElement", inNameGuiElement.c_str());
	}
}

bool CGUIManager::GetVisibleGuiElement (const std::string& inNameGuiElement)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
    return it->second->IsVisible();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetVisibleGuiElement-> No se ha encontrado el guiElement %s al ejecutar la funcion SetVisibleGuiElement", inNameGuiElement.c_str());
	}
  return false;
}

void CGUIManager::SetVisibleGuiElement (const std::string& inNameGuiElement, bool flag)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		it->second->SetVisible(flag);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetVisibleGuiElement-> No se ha encontrado el guiElement %s al ejecutar la funcion SetVisibleGuiElement", inNameGuiElement.c_str());
	}
}

bool CGUIManager::GetProgressBarValue (const std::string& inNameGuiElement, float& outValue)
{

	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		CGUIProgressBar * progress = (CGUIProgressBar*) (it->second);
		outValue = progress->GetProgress();
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetProgressBarValue-> No se ha encontrado el guiElement %s al ejecutar la funcion GetProgressBarValue", inNameGuiElement.c_str());
	}

	return false;
}

bool CGUIManager::SetProgressBarValue (const std::string& inNameGuiElement, float inValue)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		CGUIProgressBar * progress = (CGUIProgressBar*) (it->second);
		progress->SetProgress(inValue);
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetProgressBarValue-> No se ha encontrado el guiElement %s al ejecutar la funcion SetProgressBarValue", inNameGuiElement.c_str());
	}
	return false;
}

std::string CGUIManager::GetButtonCheckInRadioBox (const std::string& inNameRadioBox)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CGUIRadioBox * rb = (CGUIRadioBox*) (it->second);
		return rb->GetDefaultCheckButton();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetButtonCheckInRadioBox-> No se ha encontrado el guiElement %s al ejecutar la funcion GetButtonCheckInRadioBox", inNameRadioBox.c_str());
	}

	return "";
}

void CGUIManager::SetButtonCheckInRadioBox(  const std::string& inNameRadioBox, const std::string& button )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CGUIRadioBox * rb = (CGUIRadioBox*) (it->second);
		rb->SetDefaultCheckButton(button);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetButtonCheckInRadioBox-> No se ha encontrado el guiElement %s al ejecutar la funcion GetButtonCheckInRadioBox", inNameRadioBox.c_str());
	}
}

void CGUIManager::SetStateCheckButton ( const std::string& inCheckButtonName, bool state )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inCheckButtonName);
	if( it!= m_ElementsMap.end() )
	{
		CGUICheckButton * checkButton = (CGUICheckButton*) (it->second);
		checkButton->SetOn(state);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetStateCheckButton-> No se ha encontrado el guiElement %s al ejecutar la funcion SetStateCheckButton", inCheckButtonName.c_str());
	}
}

bool CGUIManager::GetStateCheckButton( const std::string& inCheckButtonName )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inCheckButtonName);
	if( it!= m_ElementsMap.end() )
	{
		CGUICheckButton * checkButton = (CGUICheckButton*) (it->second);
		return checkButton->GetState();

	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetStateCheckButton-> No se ha encontrado el guiElement %s al ejecutar la funcion GetStateCheckButton", inCheckButtonName.c_str());
	}
	return false;
}

void CGUIManager::SetEditableTextBox( const std::string& inEditableText, const std::string& text )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inEditableText);
	if( it!= m_ElementsMap.end() )
	{
		CGUIEditableTextBox * editableTextBox = (CGUIEditableTextBox*) (it->second);
		editableTextBox->SetBuffer(text);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetEditableTextBox-> No se ha encontrado el guiElement %s al ejecutar la funcion SetEditableTextBox", inEditableText.c_str());
	}
}

std::string CGUIManager::GetEditableTextBox( const std::string& inEditableText )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inEditableText);
	if( it!= m_ElementsMap.end() )
	{
		CGUIEditableTextBox * editableTextBox = (CGUIEditableTextBox*) (it->second);
		return editableTextBox->GetBuffer();
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetEditableTextBox-> No se ha encontrado el guiElement %s al ejecutar la funcion GetEditableTextBox", inEditableText.c_str());
	}
	return "";
}

void CGUIManager::SetImage( const std::string& inImageName, const std::string& activeImage )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CGUIImage * image = (CGUIImage*) (it->second);
		image->SetActiveTexture(activeImage);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetImage-> No se ha encontrado el guiElement %s al ejecutar la funcion SetImage", inImageName.c_str());
	}
}

std::string CGUIManager::GetImage( const std::string& inImageName )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CGUIImage * image = (CGUIImage*) (it->second);
		return image->GetActiveTexture();

	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetImage-> No se ha encontrado el guiElement %s al ejecutar la funcion GetImage", inImageName.c_str());
	}
	return "";
}

void CGUIManager::PlayImage	(const std::string& inImageName, float timePerImage, bool loop)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CGUIImage * image = (CGUIImage*) (it->second);
		image->PlayAnimation(timePerImage, loop);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::PlayImage-> No se ha encontrado el guiElement %s al ejecutar la funcion GetImage", inImageName.c_str());
	}
}

void CGUIManager::SetAlphaImage(const std::string& inImageName, float _Alpha)
{
  std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CGUIImage * image = (CGUIImage*) (it->second);
    image->SetAlpha(_Alpha);
	}
	else
	{
    LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetAlphaImage-> No se ha encontrado el guiElement %s", inImageName.c_str());
	}
}

void CGUIManager::FadeOutImage(const std::string& inImageName, float startTime, float fadePerSecond)
{
  std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inImageName);
	if( it!= m_ElementsMap.end() )
	{
		CGUIImage * image = (CGUIImage*) (it->second);
		image->FadeOut(startTime, fadePerSecond);
	}
	else
	{
    LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::FadeOutImage-> No se ha encontrado el guiElement %s", inImageName.c_str());
	}
}

void CGUIManager::SetStateSlider( const std::string& inSliderName, float amount )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inSliderName);
	if( it!= m_ElementsMap.end() )
	{
		CGUISlider * slider= (CGUISlider*) (it->second);
		slider->SetValue(amount);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetStateSlider-> No se ha encontrado el guiElement %s al ejecutar la funcion GetStateSlider", inSliderName.c_str());
	}
}

float CGUIManager::GetStateSlider( const std::string& inSliderName )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inSliderName);
	if( it!= m_ElementsMap.end() )
	{
		CGUISlider * slider= (CGUISlider*) (it->second);
		float kk=slider->GetValue();
		return slider->GetValue();

	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetStateSlider-> No se ha encontrado el guiElement %s al ejecutar la funcion GetStateSlider", inSliderName.c_str());
	}
	return 0.f;
}


void CGUIManager::SetLiteralInStaticText( const std::string& inStaticText, const std::string& lit )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inStaticText);
	if( it!= m_ElementsMap.end() )
	{
		CGUIStaticText * st = (CGUIStaticText*) (it->second);
		st->SetLiteral(lit);
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::SetLiteralInStaticText-> No se ha encontrado el guiElement %s al ejecutar la funcion SetVariableText", inStaticText.c_str());
	}
}


bool CGUIManager::NextBlockInRadioBox(  const std::string& inNameRadioBox )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CGUIRadioBox * rb = (CGUIRadioBox*) (it->second);
		rb->NextBlock();
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::NextBlockInRadioBox-> No se ha encontrado el guiElement %s al ejecutar la funcion NextBlockInRadioBox", inNameRadioBox.c_str());
	}
	return false;
}

bool CGUIManager::PrevBlockInRadioBox(  const std::string& inNameRadioBox )
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameRadioBox);
	if( it!= m_ElementsMap.end() )
	{
		CGUIRadioBox * rb = (CGUIRadioBox*) (it->second);
		rb->PrevBlock();
		return true;
	}
	else
	{
		LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::PrevBlockInRadioBox-> No se ha encontrado el guiElement %s al ejecutar la funcion PrevBlockInRadioBox", inNameRadioBox.c_str());
	}
	return false;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

CGuiElement* CGUIManager::GetGUIElement(const std::string& inNameGuiElement)
{
	std::map<std::string, CGuiElement*>::iterator it;
	it = m_ElementsMap.find(inNameGuiElement);
	if( it!= m_ElementsMap.end() )
	{
		return it->second;
	}
	else
	{
    LOGGER->AddNewLog(ELL_ERROR, "CGUIManager::GetGUIElement-> No se ha encontrado el guiElement %s", inNameGuiElement.c_str());
	}
	return NULL;
}

bool CGUIManager::ChangeWindowName(const std::string &window, const std::string &newName)
{
	TWindowsMap::iterator it;
	
	//Mira si la window no existe
	it = m_WindowsMap.find(window);
	if( it == m_WindowsMap.end() )
	{
		return false;
	}

	//Mira si el nuevo nombre existe
	if( m_WindowsMap.find(newName) != m_WindowsMap.end() )
	{
		return false;
	}

	CGUIWindow *l_pWindow = it->second;
	m_WindowsMap.erase(it);
	m_WindowsMap[ newName ] = l_pWindow;
	m_sCurrentWindows = newName;

	return true;
}

CGUIWindow*	CGUIManager::GetWindow( const std::string &window )
{
	return m_WindowsMap[window];
}
