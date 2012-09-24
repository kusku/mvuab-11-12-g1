#include <assert.h>
#include <string>

#include "SetDyingAmoungCommand.h"
#include "XML\XMLTreeNode.h"
#include "Base.h"
#include "Core.h"
#include "RenderManager.h"
#include "Effects\EffectManager.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"
#include "Utils\Timer.h"

#if defined(_DEBUG)
	#include "Memory\MemLeaks.h"
#endif


// --------------------------------------------
//			CONSTRUCTOR/DESTRUCTOR
// --------------------------------------------
CSetDyingAmountCommand::CSetDyingAmountCommand( CXMLTreeNode &_Node )
	: CSceneRendererCommand ( _Node )
{
	m_Effect = CORE->GetEffectManager()->GetEffect("DyingEffect");

	if(m_Effect == NULL)
	{
		assert(m_Effect);
		return;
	}

	m_Effect->GetParameterBySemantic("DYING_AMOUNT", m_Amount, false);

	if(m_Amount == NULL)
	{
		assert(m_Amount);
		return;
	}
}

CSetDyingAmountCommand::~CSetDyingAmountCommand()
{
}

// --------------------------------------------
//			   MÈTODES PRINCIPALS
// --------------------------------------------
void CSetDyingAmountCommand::Execute( CRenderManager &_RM )
{
	HRESULT hr = m_Effect->GetD3DEffect()->SetFloat(m_Amount, CORE->GetDyingAmount());

	assert(hr == D3D_OK);
}
