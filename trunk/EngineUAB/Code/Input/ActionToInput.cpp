#include "ActionToInput.h"
#include "InputManager.h"
#include "XML\XMLTreeNode.h"
#include "Logger\Logger.h"
#include "Exceptions\Exception.h"
#include "Base.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

CActionToInput::CActionToInput()
	: m_bIsOk(false)
	, m_pInputManager(NULL)
	, m_bMouseInverted(false)
{
}


CActionToInput::~CActionToInput()
{
	Done();
}

void CActionToInput::Release()
{
	TMapActions::iterator l_It = m_ActionsMap.begin();
	TMapActions::iterator l_End = m_ActionsMap.end();
	for(; l_It != l_End; ++l_It )
	{
		std::vector<SInputInfo*>::iterator l_ItVector = l_It->second.begin();
		std::vector<SInputInfo*>::iterator l_ItVectorEnd = l_It->second.end();
		for(; l_ItVector != l_ItVectorEnd; ++l_ItVector)
		{
			CHECKED_DELETE((*l_ItVector));
		}
		
		l_It->second.clear();
	}
	m_ActionsMap.clear();
	m_String2Code.clear();

	CHECKED_DELETE(m_pInputManager);
}

void CActionToInput::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

bool CActionToInput::Init(HWND hWnd, const Vect2i& screenRes, bool exclusiveModeinMouse)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CActionToInput:: Inicializando ActionToInput");

	m_pInputManager = new CInputManager();
	m_bIsOk = m_pInputManager->Init( hWnd, screenRes, exclusiveModeinMouse );

	InitString2Input();
	InitString2Name();

	if ( !m_bIsOk )
	{
		Release();
		std::string msg_error = "Error al inicializar CActionToInput";
		LOGGER->AddNewLog(ELL_ERROR, "CActionToInput:: Error al inicializar ActionToInput");
		throw CException(__FILE__, __LINE__, msg_error);
	}

	return m_bIsOk;
}

void CActionToInput::Update()
{
	m_pInputManager->Update();
}

bool CActionToInput::DoAction(const std::string &action)
{
 	std::vector<SInputInfo*> l_Actions = m_ActionsMap[ action ];

	if( l_Actions.size() > 0 )
	{
		std::vector<SInputInfo*>::iterator l_It = l_Actions.begin();
		std::vector<SInputInfo*>::iterator l_End = l_Actions.end();

		for(; l_It != l_End; ++l_It)
		{
			INPUT_EVENT_TYPE l_Event = (*l_It)->eventType;
			INPUT_AXIS_TYPE l_Axis = (*l_It)->axisType;
			INPUT_DEVICE_TYPE l_Device = (*l_It)->deviceType;

			if( l_Event != EVENT_NOTHING )
			{
				switch(l_Event)
				{
				case EVENT_DOWN:
					{
						if( !m_pInputManager->IsDown( l_Device, (*l_It)->key ) )
						{
							return false;
						}
						break;
					}
				case EVENT_DOWN_UP:
					{
						if( !m_pInputManager->IsDownUp( l_Device, (*l_It)->key ) )
						{
							return false;
						}
						break;
					}
				case EVENT_UP_DOWN:
					{
						if( !m_pInputManager->IsUpDown( l_Device, (*l_It)->key ) )
						{
							return false;
						}
						break;
					}
				}
			}
			else
			{
				switch(l_Axis)
				{
				case AXIS_MOUSE_X:
					{
						if( m_pInputManager->GetMouseDelta().x == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_MOUSE_Y:
					{
						if( m_pInputManager->GetMouseDelta().y == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_MOUSE_Z:
					{
						if( m_pInputManager->GetMouseDelta().z == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_LEFT_THUMB_X:
					{
						float dx;
						m_pInputManager->GetGamePadLeftThumbDeflection(&dx, NULL, l_Device);
						if( dx == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_LEFT_THUMB_Y:
					{
						float dy;
						m_pInputManager->GetGamePadLeftThumbDeflection(NULL, &dy, l_Device);
						if( dy == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_RIGHT_THUMB_X:
					{
						float dx;
						m_pInputManager->GetGamePadRightThumbDeflection(&dx, NULL, l_Device);
						if( dx == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_RIGHT_THUMB_Y:
					{
						float dy;
						m_pInputManager->GetGamePadRightThumbDeflection(NULL, &dy, l_Device);
						if( dy == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_DELTA_TRIGGER_LEFT:
					{
						float left;
						m_pInputManager->GetGamePadDeltaTriggers(&left, NULL, l_Device);
						if( left == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_DELTA_TRIGGER_RIGHT:
					{
						float right;
						m_pInputManager->GetGamePadDeltaTriggers(NULL, &right, l_Device);
						if( right == 0.0f )
						{
							return false;
						}
						break;
					}
				}
			}
		}

		//Si ha pasado todo el análisis de teclas, entonces es que se ha hecho la acción
		return true; 
	}

	return false;
}

bool CActionToInput::DoAction(const std::string &action, float &delta_)
{
	std::vector<SInputInfo*> l_Actions = m_ActionsMap[ action ];
	delta_ = 0.0f;

	if( l_Actions.size() > 0 )
	{
		std::vector<SInputInfo*>::iterator l_It = l_Actions.begin();
		std::vector<SInputInfo*>::iterator l_End = l_Actions.end();

		for(; l_It != l_End; ++l_It)
		{
			INPUT_EVENT_TYPE l_Event = (*l_It)->eventType;
			INPUT_AXIS_TYPE l_Axis = (*l_It)->axisType;
			INPUT_DEVICE_TYPE l_Device = (*l_It)->deviceType;

			if( l_Event != EVENT_NOTHING )
			{
				switch(l_Event)
				{
				case EVENT_DOWN:
					{
						if( !m_pInputManager->IsDown( l_Device, (*l_It)->key ) )
						{
							return false;
						}
						break;
					}
				case EVENT_DOWN_UP:
					{
						if( !m_pInputManager->IsDownUp( l_Device, (*l_It)->key ) )
						{
							return false;
						}
						break;
					}
				case EVENT_UP_DOWN:
					{
						if( !m_pInputManager->IsUpDown( l_Device, (*l_It)->key ) )
						{
							return false;
						}
						break;
					}
				}
			}
			else
			{
				float l_Delta = (*l_It)->delta;

				switch(l_Axis)
				{
				case AXIS_MOUSE_X:
					{
						delta_ = static_cast<float>(m_pInputManager->GetMouseDelta().x) * l_Delta;
						if( delta_ == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_MOUSE_Y:
					{
						delta_ = static_cast<float>(m_pInputManager->GetMouseDelta().y) * l_Delta;
						if( delta_ == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_MOUSE_Z:
					{
						delta_ = static_cast<float>(m_pInputManager->GetMouseDelta().z) * l_Delta;
						if( delta_ == 0.0f )
						{
							return false;
						}
						break;
					}
				case AXIS_LEFT_THUMB_X:
					{
						m_pInputManager->GetGamePadLeftThumbDeflection(&delta_, NULL, l_Device);
						if( delta_ == 0.0f )
						{
							return false;
						}
						else
						{
							delta_ *= l_Delta;
						}
						break;
					}
				case AXIS_LEFT_THUMB_Y:
					{
						m_pInputManager->GetGamePadLeftThumbDeflection(NULL, &delta_, l_Device);
						if( delta_ == 0.0f )
						{
							return false;
						}
						else
						{
							delta_ *= l_Delta;
						}
						break;
					}
				case AXIS_RIGHT_THUMB_X:
					{
						m_pInputManager->GetGamePadRightThumbDeflection(&delta_, NULL, l_Device);
						if( delta_ == 0.0f )
						{
							return false;
						}
						else
						{
							delta_ *= l_Delta;
						}
						break;
					}
				case AXIS_RIGHT_THUMB_Y:
					{
						m_pInputManager->GetGamePadRightThumbDeflection(NULL, &delta_, l_Device);
						if( delta_ == 0.0f )
						{
							return false;
						}
						else
						{
							delta_ *= l_Delta;
						}
						break;
					}
				case AXIS_DELTA_TRIGGER_LEFT:
					{
						m_pInputManager->GetGamePadDeltaTriggers(&delta_, NULL, l_Device);
						if( delta_ == 0.0f )
						{
							return false;
						}
						else
						{
							delta_ *= l_Delta;
						}
						break;
					}
				case AXIS_DELTA_TRIGGER_RIGHT:
					{
						m_pInputManager->GetGamePadDeltaTriggers(NULL, &delta_, l_Device);
						if( delta_ == 0.0f )
						{
							return false;
						}
						else
						{
							delta_ *= l_Delta;
						}
						break;
					}
				}
			}
		}

		if( m_bMouseInverted )
		{
			delta_ = -delta_;
		}

		//Si ha pasado todo el análisis de teclas, entonces es que se ha hecho la acción
		return true; 
	}

	return false;
}

float CActionToInput::DoActionMouse(const std::string &action)
{
	std::vector<SInputInfo*> l_Actions = m_ActionsMap[ action ];
	float delta = 0.0f;

	if( l_Actions.size() > 0 )
	{
		std::vector<SInputInfo*>::iterator l_It = l_Actions.begin();
		std::vector<SInputInfo*>::iterator l_End = l_Actions.end();

		for(; l_It != l_End; ++l_It)
		{
			INPUT_EVENT_TYPE l_Event = (*l_It)->eventType;
			INPUT_AXIS_TYPE l_Axis = (*l_It)->axisType;
			INPUT_DEVICE_TYPE l_Device = (*l_It)->deviceType;

			if( l_Event == EVENT_NOTHING )
			{
				float l_Delta = (*l_It)->delta;

				switch(l_Axis)
				{
				case AXIS_MOUSE_X:
					{
						delta = static_cast<float>(m_pInputManager->GetMouseDelta().x) * l_Delta;
						break;
					}
				case AXIS_MOUSE_Y:
					{
						delta = static_cast<float>(m_pInputManager->GetMouseDelta().y) * l_Delta;
						break;
					}
				case AXIS_MOUSE_Z:
					{
						delta = static_cast<float>(m_pInputManager->GetMouseDelta().z) * l_Delta;
						break;
					}
				}
			}
		}
	}

	if( m_bMouseInverted )
	{
		delta = -delta;
	}

	return delta;
}

void CActionToInput::GetActionInfo( const std::string &action, std::string &keys_ )
{
	std::vector<SInputInfo*> l_Actions = m_ActionsMap[ action ];
	std::vector<SInputInfo*>::iterator l_It = l_Actions.begin();
	std::vector<SInputInfo*>::iterator l_End = l_Actions.end();
	
	for(; l_It != l_End; ++l_It)
	{
		std::string key = (*l_It)->keyName;
		if( key != "" )
		{
			if( keys_ == "" )
			{
				keys_ += key;
			}
			else
			{
				keys_ += " + " + key;
			}
		}
	}
}

void CActionToInput::Reload()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CActionToInput: Reload de los inputs.");
	//Limpia todo el map con la información de las acciones
	TMapActions::iterator l_It = m_ActionsMap.begin();
	TMapActions::iterator l_End = m_ActionsMap.end();
	for(; l_It != l_End; ++l_It )
	{
		std::vector<SInputInfo*>::iterator l_ItVector = l_It->second.begin();
		std::vector<SInputInfo*>::iterator l_ItVectorEnd = l_It->second.end();
		for(; l_ItVector != l_ItVectorEnd; ++l_ItVector)
		{
			CHECKED_DELETE((*l_ItVector));
		}
		
		l_It->second.clear();
	}

	//Vuelve a leer el fichero con el mismo path
	LoadXML(m_FilePath);
}

void CActionToInput::LoadXML(const std::string &filename)
{
	CXMLTreeNode newFile;
	if (!newFile.LoadFile(filename.c_str()))
	{
		std::string msg_error = "CActionToInput::LoadXML->Error al intentar leer el archivo de acciones: " + filename;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}

	m_FilePath = filename;

	CXMLTreeNode l_InputNode = newFile["Actions"];
	if( l_InputNode.Exists() )
	{
		int count = l_InputNode.GetNumChildren();
		for(int i=0; i<count; ++i)
		{
			//Lee una acción
			std::string l_Name = l_InputNode(i).GetName();
			if( l_Name == "action" )
			{
				//Lee el nombre vinculado a una acción
				std::string l_ActionName = l_InputNode(i).GetPszProperty( "name", "" );
				std::vector<SInputInfo*> l_InputVector;

				int childs = l_InputNode(i).GetNumChildren();
				for(int j=0; j<childs; ++j)
				{
					//Lee todos los inputs de una acción concreta
					l_Name = l_InputNode(i)(j).GetName();
					if( l_Name == "input" )
					{
						std::string l_InputTypes;
						SInputInfo* l_Info = new SInputInfo();
						
						l_InputTypes = l_InputNode(i)(j).GetPszProperty( "deviceType", "", false );
						l_Info->deviceType = static_cast<INPUT_DEVICE_TYPE>(m_String2Code[ l_InputTypes ]);

						l_InputTypes = l_InputNode(i)(j).GetPszProperty( "EventType", "EVENT_NOTHING", false );
						l_Info->eventType = static_cast<INPUT_EVENT_TYPE>(m_String2Code[ l_InputTypes ]);

						l_InputTypes = l_InputNode(i)(j).GetPszProperty( "AxisType", "AXIS_NOTHING", false );
						l_Info->axisType = static_cast<INPUT_AXIS_TYPE>(m_String2Code[ l_InputTypes ]);

						l_InputTypes = l_InputNode(i)(j).GetPszProperty( "Code", "", false );
						l_Info->key = m_String2Code[ l_InputTypes ];
						l_Info->keyName = m_String2Name[ l_InputTypes ];

						l_Info->delta = l_InputNode(i)(j).GetFloatProperty( "Delta", 0.0f, false );

						l_InputVector.push_back( l_Info );
						l_Info = NULL;
					}
				}

				//Guarda la información leida en el mapa
				m_ActionsMap[ l_ActionName ] = l_InputVector;
			}
		}
	}
}

void CActionToInput::InitString2Name()
{
	typedef std::pair<std::string, std::string> PairString2Code;

	//----------------MOUSE_BUTTON---------------------//
	m_String2Name.insert( PairString2Code("MOUSE_BUTTON_LEFT",                  "MOUSE LEFT") );
	m_String2Name.insert( PairString2Code("MOUSE_BUTTON_RIGHT",                 "MOUSE RIGHT") );
	m_String2Name.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",                "MOUSE MIDDLE") );

	//----------------Gamepad codes from XInput.h------//
	m_String2Name.insert( PairString2Code("PAD_DPAD_UP",                        "PAD UP") );
	m_String2Name.insert( PairString2Code("PAD_DPAD_DOWN",                      "PAD DOWN") );
	m_String2Name.insert( PairString2Code("PAD_DPAD_LEFT",                      "PAD LEFT") );
	m_String2Name.insert( PairString2Code("PAD_DPAD_RIGHT",                     "PAD RIGHT") );
	m_String2Name.insert( PairString2Code("PAD_DPAD_START",                     "PAD START") );
	m_String2Name.insert( PairString2Code("PAD_DPAD_BACK",                      "PAD BACK") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_LEFT_THUMB",				"PAD LEFT THUMB") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_RIGHT_THUMB",				"PAD RIGHT THUMB") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_LEFT_SHOULDER",			"PAD LEFT SHOULDER") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_RIGHT_SHOULDER",			"PAD RIGHT SHOULDER") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_A",						"PAD A") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_B",						"PAD B") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_X",                       "PAD X") );
	m_String2Name.insert( PairString2Code("PAD_BUTTON_Y",                       "PAD Y") );

	//-----------Keyboard scan codes from dinput.h ------//
	m_String2Name.insert( PairString2Code("KEY_ESCAPE",                         "ESCAPE") );
	m_String2Name.insert( PairString2Code("KEY_TAB",                            "TAB") );
	m_String2Name.insert( PairString2Code("KEY_SPACE",                          "SPACE") );
	m_String2Name.insert( PairString2Code("KEY_RETURN",                         "RETURN") );
	m_String2Name.insert( PairString2Code("KEY_BACK",                           "BACK") );
	m_String2Name.insert( PairString2Code("KEY_CAPITAL",                        "CAPITAL") );
	m_String2Name.insert( PairString2Code("KEY_MINUS",                          "MINUS") );
	m_String2Name.insert( PairString2Code("KEY_EQUALS",                         "EQUALS") );
	m_String2Name.insert( PairString2Code("KEY_LBRACKET",                       "LBRACKET") );
	m_String2Name.insert( PairString2Code("KEY_RBRACKET",                       "RBRACKET") );
	m_String2Name.insert( PairString2Code("KEY_SEMICOLON",                      "SEMICOLON") );
	m_String2Name.insert( PairString2Code("KEY_APOSTROPHE",                     "APOSTROPHE") );
	m_String2Name.insert( PairString2Code("KEY_GRAVE",                          "GRAVE") );
	m_String2Name.insert( PairString2Code("KEY_BACKSLASH",                      "BACKSLASH") );
	m_String2Name.insert( PairString2Code("KEY_COMMA",                          "COMMA") );
	m_String2Name.insert( PairString2Code("KEY_PERIOD",                         "PERIOD") );
	m_String2Name.insert( PairString2Code("KEY_SLASH",                          "SLASH") );
	m_String2Name.insert( PairString2Code("KEY_A",                              "A") );
	m_String2Name.insert( PairString2Code("KEY_S",                              "S") );
	m_String2Name.insert( PairString2Code("KEY_D",                              "D") );
	m_String2Name.insert( PairString2Code("KEY_F",                              "F") );
	m_String2Name.insert( PairString2Code("KEY_G",								"G") );
	m_String2Name.insert( PairString2Code("KEY_H",                              "H") );
	m_String2Name.insert( PairString2Code("KEY_J",                              "J") );
	m_String2Name.insert( PairString2Code("KEY_K",                              "K") );
	m_String2Name.insert( PairString2Code("KEY_L",								"L") );
	m_String2Name.insert( PairString2Code("KEY_Z",								"Z") );
	m_String2Name.insert( PairString2Code("KEY_X",								"X") );
	m_String2Name.insert( PairString2Code("KEY_C",                              "C") );
	m_String2Name.insert( PairString2Code("KEY_B",                              "B") );
	m_String2Name.insert( PairString2Code("KEY_N",								"N") );
	m_String2Name.insert( PairString2Code("KEY_M",                              "M") );
	m_String2Name.insert( PairString2Code("KEY_Q",                              "Q") );
	m_String2Name.insert( PairString2Code("KEY_W",								"W") );
	m_String2Name.insert( PairString2Code("KEY_E",                              "E") );
	m_String2Name.insert( PairString2Code("KEY_R",								"R") );
	m_String2Name.insert( PairString2Code("KEY_T",								"T") );
	m_String2Name.insert( PairString2Code("KEY_Y",                              "Y") );
	m_String2Name.insert( PairString2Code("KEY_U",								"U") );
	m_String2Name.insert( PairString2Code("KEY_I",								"I") );
	m_String2Name.insert( PairString2Code("KEY_O",                              "O") );
	m_String2Name.insert( PairString2Code("KEY_P",                              "P") );
	m_String2Name.insert( PairString2Code("KEY_1",								"1") );
	m_String2Name.insert( PairString2Code("KEY_2",								"2") );
	m_String2Name.insert( PairString2Code("KEY_3",								"3") );
	m_String2Name.insert( PairString2Code("KEY_4",                              "4") );
	m_String2Name.insert( PairString2Code("KEY_5",                              "5") );
	m_String2Name.insert( PairString2Code("KEY_6",                              "6") );
	m_String2Name.insert( PairString2Code("KEY_7",                              "7") );
	m_String2Name.insert( PairString2Code("KEY_8",                              "8") );
	m_String2Name.insert( PairString2Code("KEY_9",                              "9") );
	m_String2Name.insert( PairString2Code("KEY_0",                              "0") );
	m_String2Name.insert( PairString2Code("KEY_F1",                             "F1") );
	m_String2Name.insert( PairString2Code("KEY_F2",                             "F2") );
	m_String2Name.insert( PairString2Code("KEY_F3",                             "F3") );
	m_String2Name.insert( PairString2Code("KEY_F4",								"F4") );
	m_String2Name.insert( PairString2Code("KEY_F5",                             "F5") );
	m_String2Name.insert( PairString2Code("KEY_F6",                             "F6") );
	m_String2Name.insert( PairString2Code("KEY_F7",								"F7") );
	m_String2Name.insert( PairString2Code("KEY_F8",                             "F8") );
	m_String2Name.insert( PairString2Code("KEY_F9",                             "F9") );
	m_String2Name.insert( PairString2Code("KEY_F10",                            "F10") );
	m_String2Name.insert( PairString2Code("KEY_F11",                            "F11") );
	m_String2Name.insert( PairString2Code("KEY_F12",							"F12") );
	m_String2Name.insert( PairString2Code("KEY_NPPLUS",                         "NUMPAD PLUS") );
	m_String2Name.insert( PairString2Code("KEY_NPMINUS",                        "NUMPAD MINUS") );
	m_String2Name.insert( PairString2Code("KEY_NPDECIMAL",						"NUMPAD DECIMAL") );
	m_String2Name.insert( PairString2Code("KEY_NPCOMMA",                        "NUMPAD COMMA") );
	m_String2Name.insert( PairString2Code("KEY_NPDIVIDE",                       "NUMPAD DIVIDE") );
	m_String2Name.insert( PairString2Code("KEY_NPMULTIPLY",                     "NUMPAD MULTIPLY") );
	m_String2Name.insert( PairString2Code("KEY_NPENTER",                        "NUMPAD ENTER") );
	m_String2Name.insert( PairString2Code("KEY_NUMLOCK",                        "NUMLOCK") );
	m_String2Name.insert( PairString2Code("KEY_NP1",                            "NUMPAD 1") );
	m_String2Name.insert( PairString2Code("KEY_NP2",							"NUMPAD 2") );
	m_String2Name.insert( PairString2Code("KEY_NP3",							"NUMPAD 3") );
	m_String2Name.insert( PairString2Code("KEY_NP4",                            "NUMPAD 4") );
	m_String2Name.insert( PairString2Code("KEY_NP5",                            "NUMPAD 5") );
	m_String2Name.insert( PairString2Code("KEY_NP6",                            "NUMPAD 6") );
	m_String2Name.insert( PairString2Code("KEY_NP7",                            "NUMPAD 7") );
	m_String2Name.insert( PairString2Code("KEY_NP8",							"NUMPAD 8") );
	m_String2Name.insert( PairString2Code("KEY_NP9",                            "NUMPAD 9") );
	m_String2Name.insert( PairString2Code("KEY_NP0",                            "NUMPAD 0") );
	m_String2Name.insert( PairString2Code("KEY_RSHIFT",                         "RSHIFT") );
	m_String2Name.insert( PairString2Code("KEY_LSHIFT",							"LSHIFT") );
	m_String2Name.insert( PairString2Code("KEY_RCTRL",                          "RCTRL") );
	m_String2Name.insert( PairString2Code("KEY_LCTRL",                          "LCTRL") );
	m_String2Name.insert( PairString2Code("KEY_RALT",                           "RALT") );
	m_String2Name.insert( PairString2Code("KEY_LALT",                           "LALT") );
	m_String2Name.insert( PairString2Code("KEY_LWIN",                           "LWIN") );
	m_String2Name.insert( PairString2Code("KEY_RWIN",                           "RWIN") );
	m_String2Name.insert( PairString2Code("KEY_UP",                             "UP") );
	m_String2Name.insert( PairString2Code("KEY_DOWN",                           "DOWN") );
	m_String2Name.insert( PairString2Code("KEY_LEFT",							"LEFT") );
	m_String2Name.insert( PairString2Code("KEY_RIGHT",                          "RIGHT") );
	m_String2Name.insert( PairString2Code("KEY_INSERT",                         "INSERT") );
	m_String2Name.insert( PairString2Code("KEY_DELETE",                         "DELETE") );
	m_String2Name.insert( PairString2Code("KEY_HOME",                           "HOME") );
	m_String2Name.insert( PairString2Code("KEY_END",							"END") );
	m_String2Name.insert( PairString2Code("KEY_PGDOWN",                         "PGDOWN") );
	m_String2Name.insert( PairString2Code("KEY_PGUP",                           "PGUP") );
	m_String2Name.insert( PairString2Code("KEY_PAUSE",                          "PAUSE") );
	m_String2Name.insert( PairString2Code("KEY_SCROLL",							"SCROLL") );  
}

void CActionToInput::InitString2Input ()
{
	typedef std::pair<std::string, uint32> PairString2Code;

    //----------------INPUT_DEVICE_TYPE---------------//
	m_String2Code.insert( std::pair<std::string, uint32>("IDV_KEYBOARD",         IDV_KEYBOARD) );
	m_String2Code.insert( PairString2Code("IDV_MOUSE",                           IDV_MOUSE) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD1",                        IDV_GAMEPAD1) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD2",                        IDV_GAMEPAD2) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD3",                        IDV_GAMEPAD3) );
	m_String2Code.insert( PairString2Code("IDV_GAMEPAD4",                        IDV_GAMEPAD4) );
	m_String2Code.insert( PairString2Code("IDV_NOTHING",                         IDV_NOTHING) );

	//----------------INPUT_AXIS_TYPE-----------------//
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_X",                        AXIS_MOUSE_X) );
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_Y",                        AXIS_MOUSE_Y) );
	m_String2Code.insert( PairString2Code("AXIS_MOUSE_Z",						AXIS_MOUSE_Z) );
	m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_X",                   AXIS_LEFT_THUMB_X) );
	m_String2Code.insert( PairString2Code("AXIS_LEFT_THUMB_Y",                   AXIS_LEFT_THUMB_Y) );
	m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_X",                  AXIS_RIGHT_THUMB_X) );
	m_String2Code.insert( PairString2Code("AXIS_RIGHT_THUMB_Y",                  AXIS_RIGHT_THUMB_Y) );
	m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_RIGHT",			AXIS_DELTA_TRIGGER_RIGHT) );
	m_String2Code.insert( PairString2Code("AXIS_DELTA_TRIGGER_LEFT",				AXIS_DELTA_TRIGGER_LEFT) );
	m_String2Code.insert( PairString2Code("AXIS_NOTHING",                        AXIS_NOTHING) );
       
	//----------------INPUT_EVENT_TYPE-----------------//           
	m_String2Code.insert( PairString2Code("EVENT_DOWN",                          EVENT_DOWN) );
	m_String2Code.insert( PairString2Code("EVENT_UP_DOWN",                       EVENT_UP_DOWN) );
	m_String2Code.insert( PairString2Code("EVENT_DOWN_UP",                       EVENT_DOWN_UP) );
	m_String2Code.insert( PairString2Code("EVENT_NOTHING",                       EVENT_NOTHING) );

	//----------------MOUSE_BUTTON---------------------//
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_LEFT",                   MOUSE_BUTTON_LEFT) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_RIGHT",                  MOUSE_BUTTON_RIGHT) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_MIDDLE",                 MOUSE_BUTTON_MIDDLE) );
	m_String2Code.insert( PairString2Code("MOUSE_BUTTON_NOTHING",                MOUSE_BUTTON_NOTHING) );


	//----------------Gamepad codes from XInput.h------//
	m_String2Code.insert( PairString2Code("PAD_DPAD_UP",                         PAD_DPAD_UP) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_DOWN",                       PAD_DPAD_DOWN) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_LEFT",                       PAD_DPAD_LEFT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_RIGHT",                      PAD_DPAD_RIGHT) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_START",                      PAD_DPAD_START) );
	m_String2Code.insert( PairString2Code("PAD_DPAD_BACK",                       PAD_DPAD_BACK) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_THUMB",				PAD_BUTTON_LEFT_THUMB) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_THUMB",				PAD_BUTTON_RIGHT_THUMB) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_LEFT_SHOULDER",			PAD_BUTTON_LEFT_SHOULDER) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_RIGHT_SHOULDER",			PAD_BUTTON_RIGHT_SHOULDER) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_A",						PAD_BUTTON_A) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_B",                        PAD_BUTTON_B) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_X",                        PAD_BUTTON_X) );
	m_String2Code.insert( PairString2Code("PAD_BUTTON_Y",                        PAD_BUTTON_Y) );

	//-----------Keyboard scan codes from dinput.h ------//
	m_String2Code.insert( PairString2Code("KEY_ESCAPE",                          KEY_ESCAPE) );
	m_String2Code.insert( PairString2Code("KEY_TAB",                             KEY_TAB) );
	m_String2Code.insert( PairString2Code("KEY_SPACE",                           KEY_SPACE) );
	m_String2Code.insert( PairString2Code("KEY_RETURN",                          KEY_RETURN) );
	m_String2Code.insert( PairString2Code("KEY_BACK",                            KEY_BACK) );
	m_String2Code.insert( PairString2Code("KEY_CAPITAL",                         KEY_CAPITAL) );
	m_String2Code.insert( PairString2Code("KEY_MINUS",                           KEY_MINUS) );
	m_String2Code.insert( PairString2Code("KEY_EQUALS",                          KEY_EQUALS) );
	m_String2Code.insert( PairString2Code("KEY_LBRACKET",                        KEY_LBRACKET) );
	m_String2Code.insert( PairString2Code("KEY_RBRACKET",                        KEY_RBRACKET) );
	m_String2Code.insert( PairString2Code("KEY_SEMICOLON",                       KEY_SEMICOLON) );
	m_String2Code.insert( PairString2Code("KEY_APOSTROPHE",                      KEY_APOSTROPHE) );
	m_String2Code.insert( PairString2Code("KEY_GRAVE",                           KEY_GRAVE) );
	m_String2Code.insert( PairString2Code("KEY_BACKSLASH",                       KEY_BACKSLASH) );
	m_String2Code.insert( PairString2Code("KEY_COMMA",                           KEY_COMMA) );
	m_String2Code.insert( PairString2Code("KEY_PERIOD",                          KEY_PERIOD) );
	m_String2Code.insert( PairString2Code("KEY_SLASH",                           KEY_SLASH) );
	m_String2Code.insert( PairString2Code("KEY_A",                               KEY_A) );
	m_String2Code.insert( PairString2Code("KEY_S",                               KEY_S) );
	m_String2Code.insert( PairString2Code("KEY_D",                               KEY_D) );
	m_String2Code.insert( PairString2Code("KEY_F",                               KEY_F) );
	m_String2Code.insert( PairString2Code("KEY_G",									KEY_G) );
	m_String2Code.insert( PairString2Code("KEY_H",                               KEY_H) );
	m_String2Code.insert( PairString2Code("KEY_J",                               KEY_J) );
	m_String2Code.insert( PairString2Code("KEY_K",                               KEY_K) );
	m_String2Code.insert( PairString2Code("KEY_L",								KEY_L) );
	m_String2Code.insert( PairString2Code("KEY_Z",								KEY_Z) );
	m_String2Code.insert( PairString2Code("KEY_X",								KEY_X) );
	m_String2Code.insert( PairString2Code("KEY_C",                               KEY_C) );
	m_String2Code.insert( PairString2Code("KEY_B",                               KEY_B) );
	m_String2Code.insert( PairString2Code("KEY_N",								KEY_N) );
	m_String2Code.insert( PairString2Code("KEY_M",                               KEY_M) );
	m_String2Code.insert( PairString2Code("KEY_Q",                               KEY_Q) );
	m_String2Code.insert( PairString2Code("KEY_W",								KEY_W) );
	m_String2Code.insert( PairString2Code("KEY_E",                               KEY_E) );
	m_String2Code.insert( PairString2Code("KEY_R",								KEY_R) );
	m_String2Code.insert( PairString2Code("KEY_T",								KEY_T) );
	m_String2Code.insert( PairString2Code("KEY_Y",                               KEY_Y) );
	m_String2Code.insert( PairString2Code("KEY_U",								KEY_U) );
	m_String2Code.insert( PairString2Code("KEY_I",								KEY_I) );
	m_String2Code.insert( PairString2Code("KEY_O",                               KEY_O) );
	m_String2Code.insert( PairString2Code("KEY_P",                               KEY_P) );
	m_String2Code.insert( PairString2Code("KEY_1",								KEY_1) );
	m_String2Code.insert( PairString2Code("KEY_2",								KEY_2) );
	m_String2Code.insert( PairString2Code("KEY_3",								KEY_3) );
	m_String2Code.insert( PairString2Code("KEY_4",                               KEY_4) );
	m_String2Code.insert( PairString2Code("KEY_5",                               KEY_5) );
	m_String2Code.insert( PairString2Code("KEY_6",                               KEY_6) );
	m_String2Code.insert( PairString2Code("KEY_7",                               KEY_7) );
	m_String2Code.insert( PairString2Code("KEY_8",                               KEY_8) );
	m_String2Code.insert( PairString2Code("KEY_9",                               KEY_9) );
	m_String2Code.insert( PairString2Code("KEY_0",                               KEY_0) );
	m_String2Code.insert( PairString2Code("KEY_F1",                              KEY_F1) );
	m_String2Code.insert( PairString2Code("KEY_F2",                              KEY_F2) );
	m_String2Code.insert( PairString2Code("KEY_F3",                              KEY_F3) );
	m_String2Code.insert( PairString2Code("KEY_F4",								KEY_F4) );
	m_String2Code.insert( PairString2Code("KEY_F5",                              KEY_F5) );
	m_String2Code.insert( PairString2Code("KEY_F6",                              KEY_F6) );
	m_String2Code.insert( PairString2Code("KEY_F7",								KEY_F7) );
	m_String2Code.insert( PairString2Code("KEY_F8",                              KEY_F8) );
	m_String2Code.insert( PairString2Code("KEY_F9",                              KEY_F9) );
	m_String2Code.insert( PairString2Code("KEY_F10",                             KEY_F10) );
	m_String2Code.insert( PairString2Code("KEY_F11",                             KEY_F11) );
	m_String2Code.insert( PairString2Code("KEY_F12",								KEY_F12) );
	m_String2Code.insert( PairString2Code("KEY_NPPLUS",                          KEY_NPPLUS) );
	m_String2Code.insert( PairString2Code("KEY_NPMINUS",                         KEY_NPMINUS) );
	m_String2Code.insert( PairString2Code("KEY_NPDECIMAL",						KEY_NPDECIMAL) );
	m_String2Code.insert( PairString2Code("KEY_NPCOMMA",                         KEY_NPCOMMA) );
	m_String2Code.insert( PairString2Code("KEY_NPDIVIDE",                        KEY_NPDIVIDE) );
	m_String2Code.insert( PairString2Code("KEY_NPMULTIPLY",                      KEY_NPMULTIPLY) );
	m_String2Code.insert( PairString2Code("KEY_NPENTER",                         KEY_NPENTER) );
	m_String2Code.insert( PairString2Code("KEY_NUMLOCK",                         KEY_NUMLOCK) );
	m_String2Code.insert( PairString2Code("KEY_NP1",                             KEY_NP1) );
	m_String2Code.insert( PairString2Code("KEY_NP2",								KEY_NP2) );
	m_String2Code.insert( PairString2Code("KEY_NP3",								KEY_NP3) );
	m_String2Code.insert( PairString2Code("KEY_NP4",                             KEY_NP4) );
	m_String2Code.insert( PairString2Code("KEY_NP5",                             KEY_NP5) );
	m_String2Code.insert( PairString2Code("KEY_NP6",                             KEY_NP6) );
	m_String2Code.insert( PairString2Code("KEY_NP7",                             KEY_NP7) );
	m_String2Code.insert( PairString2Code("KEY_NP8",								KEY_NP8) );
	m_String2Code.insert( PairString2Code("KEY_NP9",                             KEY_NP9) );
	m_String2Code.insert( PairString2Code("KEY_NP0",                             KEY_NP0) );
	m_String2Code.insert( PairString2Code("KEY_RSHIFT",                          KEY_RSHIFT) );
	m_String2Code.insert( PairString2Code("KEY_LSHIFT",							KEY_LSHIFT) );
	m_String2Code.insert( PairString2Code("KEY_RCTRL",                           KEY_RCTRL) );
	m_String2Code.insert( PairString2Code("KEY_LCTRL",                           KEY_LCTRL) );
	m_String2Code.insert( PairString2Code("KEY_RALT",                            KEY_RALT) );
	m_String2Code.insert( PairString2Code("KEY_LALT",                            KEY_LALT) );
	m_String2Code.insert( PairString2Code("KEY_LWIN",                            KEY_LWIN) );
	m_String2Code.insert( PairString2Code("KEY_RWIN",                            KEY_RWIN) );
	m_String2Code.insert( PairString2Code("KEY_UP",                              KEY_UP) );
	m_String2Code.insert( PairString2Code("KEY_DOWN",                            KEY_DOWN) );
	m_String2Code.insert( PairString2Code("KEY_LEFT",							KEY_LEFT) );
	m_String2Code.insert( PairString2Code("KEY_RIGHT",                           KEY_RIGHT) );
	m_String2Code.insert( PairString2Code("KEY_INSERT",                          KEY_INSERT) );
	m_String2Code.insert( PairString2Code("KEY_DELETE",                          KEY_DELETE) );
	m_String2Code.insert( PairString2Code("KEY_HOME",                            KEY_HOME) );
	m_String2Code.insert( PairString2Code("KEY_END",								KEY_END) );
	m_String2Code.insert( PairString2Code("KEY_PGDOWN",                          KEY_PGDOWN) );
	m_String2Code.insert( PairString2Code("KEY_PGUP",                            KEY_PGUP) );
	m_String2Code.insert( PairString2Code("KEY_PAUSE",                           KEY_PAUSE) );
	m_String2Code.insert( PairString2Code("KEY_SCROLL",                          KEY_SCROLL) );  
}

bool CActionToInput::GetGamepadState( void ) const
{
	return m_pInputManager->HasGamePad();
}