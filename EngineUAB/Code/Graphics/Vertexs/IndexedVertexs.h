#pragma once

#ifndef _INDEXEDVERTEXS_H
#define _INDEXEDVERTEXS_H

#include "RenderableVertexs.h"
#include "RenderManager.h"
#include "VertexType.h"
#include "Effects\EffectTechnique.h"
#include "Effects\Effect.h"

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

		return true;
	}

	bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique) const
	{
		LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
		UINT l_NumPasses;
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

	virtual inline unsigned short GetVertexType() const { return T::GetVertexType(); }

protected:
	inline size_t GetVertexSize() const	{ return sizeof(T); }
	inline size_t GetIndexSize() const	{ return sizeof(unsigned short); }
};

#endif