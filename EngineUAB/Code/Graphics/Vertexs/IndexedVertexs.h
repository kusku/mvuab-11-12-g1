#pragma once

#ifndef _INDEXEDVERTEXS_H
#define _INDEXEDVERTEXS_H

#include "RenderableVertexs.h"
#include "RenderManager.h"
#include "VertexType.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"
#include "Effects\EffectManager.h"
#include "Stadistics\Stadistics.h"
#include "Base.h"
#include "Core.h"

template<class T>
class CIndexedVertexs : public CRenderableVertexs
{
public:
	CIndexedVertexs(CRenderManager *RM, void *VertexAddress, void *IndexAddress, size_t VertexCount, size_t IndexCount)
	{
		m_VertexCount = VertexCount;
		m_IndexCount = IndexCount;

		void *l_Data = NULL;
		LPDIRECT3DDEVICE9 Device = RM->GetDevice();
		
		UINT l_Length = GetVertexSize() * m_VertexCount;
		Device->CreateVertexBuffer( l_Length, 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, NULL);
		m_VB->Lock(0, l_Length, &l_Data, 0);
		memcpy(l_Data, VertexAddress, l_Length);
		m_VB->Unlock();

		l_Length = GetIndexSize() * m_IndexCount;
		Device->CreateIndexBuffer( l_Length, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, NULL);
		m_IB->Lock(0, l_Length, &l_Data, 0);
		memcpy(l_Data, IndexAddress, l_Length);
		m_IB->Unlock();
	}

	~CIndexedVertexs()
	{
	}

	virtual bool Render(CRenderManager *RM) const
	{
		LPDIRECT3DDEVICE9 Device = RM->GetDevice();

		Device->SetStreamSource( 0, m_VB, 0, GetVertexSize() );
		Device->SetIndices( m_IB );
		Device->SetFVF( static_cast<DWORD>(T::GetFVF()) );
		Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, static_cast<UINT>(m_VertexCount), 0, static_cast<UINT>(m_IndexCount/3) );

#if defined(_DEBUG)
		//Capture Info for Stadistics
		CStadistics *l_pStadistics = CORE->GetStadistics();

		l_pStadistics->AddDrawCall();
		l_pStadistics->AddVerticesInFrustum(m_VertexCount);
		l_pStadistics->AddTriangles( m_IndexCount / 3 );
#endif

		return true;
	}

bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique, int baseVertexIndex, uint32 minVertexIndex, uint32 vertexCount, uint32 startIndex, uint32 faceCount) const
{
	LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
	UINT l_NumPasses;
	if( EffectTechnique == NULL )
	{
		//Coger una technique por defecto
		std::string l_TechName = CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault( T::GetVertexType() );
		EffectTechnique = CORE->GetEffectManager()->GetEffectTechnique(l_TechName);
	}

	if( EffectTechnique->BeginRender() )
	{
		LPD3DXEFFECT l_Effect=EffectTechnique->GetEffect()->GetD3DEffect();
		l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
		
		if(SUCCEEDED(l_Effect->Begin(&l_NumPasses,0)))
		{
			l_Device->SetVertexDeclaration(T::GetVertexDeclaration());
			l_Device->SetStreamSource(0,m_VB,0,sizeof(T));
			l_Device->SetIndices(m_IB);

			for (UINT b=0;b<l_NumPasses;++b)
			{
				l_Effect->BeginPass(b);
				l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, baseVertexIndex, minVertexIndex,
					vertexCount, startIndex, faceCount);
				l_Effect->EndPass();

#if defined(_DEBUG)
				//Capture Info for Stadistics
				CStadistics *l_pStadistics = CORE->GetStadistics();

				l_pStadistics->AddDrawCall();
				l_pStadistics->AddVerticesInFrustum(vertexCount);
				l_pStadistics->AddTriangles(faceCount);
#endif
			}
			l_Effect->End();
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique) const
{
	LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
	UINT l_NumPasses;

	if( EffectTechnique == NULL )
	{
		//Coger una technique por defecto
		std::string l_TechName = CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault( T::GetVertexType() );
		EffectTechnique = CORE->GetEffectManager()->GetEffectTechnique(l_TechName);
	}

	if( EffectTechnique->BeginRender() )
	{
		LPD3DXEFFECT l_Effect=EffectTechnique->GetEffect()->GetD3DEffect();
		l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
		
		if(SUCCEEDED(l_Effect->Begin(&l_NumPasses,0)))
		{
			l_Device->SetVertexDeclaration(T::GetVertexDeclaration());
			l_Device->SetStreamSource(0,m_VB,0,sizeof(T));
			l_Device->SetIndices(m_IB);

			for (UINT b=0;b<l_NumPasses;++b)
			{
				l_Effect->BeginPass(b);
				l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0,
					static_cast<UINT>(m_VertexCount), 0, static_cast<UINT>( m_IndexCount/3));
				l_Effect->EndPass();

#if defined(_DEBUG)
				//Capture Info for Stadistics
				CStadistics *l_pStadistics = CORE->GetStadistics();

				l_pStadistics->AddDrawCall();
				l_pStadistics->AddVerticesInFrustum(m_VertexCount);
				l_pStadistics->AddTriangles( m_IndexCount / 3 );
#endif

			}
			l_Effect->End();
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool RenderInstance(CRenderManager *RM, CEffectTechnique *EffectTechnique, LPDIRECT3DVERTEXBUFFER9 instanceBuffer, uint32 size) const
{
	LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
	UINT l_NumPasses;

	if( EffectTechnique == NULL )
	{
		//Coger una technique por defecto
		std::string l_TechName = CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault( T::GetVertexType() );
		EffectTechnique = CORE->GetEffectManager()->GetEffectTechnique(l_TechName);
	}

	if( EffectTechnique->BeginRender() )
	{
		LPD3DXEFFECT l_Effect = EffectTechnique->GetEffect()->GetD3DEffect();
		
		l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());

		//Set VB of Geo
		l_Device->SetStreamSource(0, m_VB, 0, sizeof(T));
		l_Device->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | size);

		//Set VB of Instance
		l_Device->SetStreamSource( 1, instanceBuffer, 0, sizeof(TINSTANCE_VERTEX) );
		l_Device->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1ul);

		l_Device->SetVertexDeclaration(T::GetVertexDeclarationInstance());

		l_Device->SetIndices(m_IB);

		if(SUCCEEDED(l_Effect->Begin(&l_NumPasses,0)))
		{
			for (UINT b=0;b<l_NumPasses;++b)
			{
				l_Effect->BeginPass(b);
				l_Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0,
					static_cast<UINT>(m_VertexCount), 0, static_cast<UINT>( m_IndexCount/3));
				l_Effect->EndPass();

#if defined(_DEBUG)
				//Capture Info for Stadistics
				CStadistics *l_pStadistics = CORE->GetStadistics();

				l_pStadistics->AddDrawCall();
				l_pStadistics->AddVerticesInFrustum(m_VertexCount);
				l_pStadistics->AddTriangles( m_IndexCount / 3 );
#endif

			}
			l_Effect->End();
		}

		//Set Steam Freq to Default
		l_Device->SetStreamSourceFreq( 0, 1 );
		l_Device->SetStreamSourceFreq( 1, 1 );

		return true;
	}
	else
	{
		return false;
	}
}

virtual inline unsigned short GetVertexType() const { return T::GetVertexType(); }

protected:
	inline size_t GetVertexSize() const	{ return sizeof(T); }
	inline size_t GetIndexSize() const	{ return sizeof(unsigned short); }
};

#endif