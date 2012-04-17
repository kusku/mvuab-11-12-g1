#include "stdafx.h"
#include "GUIEditorProcess.h"
#include "defines.h"

CGUIEditorProcess::CGUIEditorProcess(void)
{
}


CGUIEditorProcess::~CGUIEditorProcess(void)
{
}

void CGUIEditorProcess::Update(float elapsedTime)
{
	PostMessage( m_hWndOutput, WM_GETLOGGER, 0, 0);
}