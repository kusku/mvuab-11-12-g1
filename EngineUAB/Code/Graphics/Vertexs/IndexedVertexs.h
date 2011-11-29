#pragma once

#ifndef _INDEXEDVERTEXS_H
#define _INDEXEDVERTEXS_H

#include "RenderableVertexs.h"
#include "..\RenderManager.h"

template<class T>
class CIndexedVertexs : public CRenderableVertexs
{
public:
	CIndexedVertexs(CRenderManager *RM, void *VertexAddress, void *IndexAddress, size_t VertexCount, size_t IndexCount)
		: m_VertexCount(VertexCount)
		, m_IndexCount(IndexCount)
	{
		LPDIRECT3DDEVICE9 Device = RM->GetDevice();		
		Device->CreateVertexBuffer( GetVertexSize()*m_VertexCount, 0, T::GetFVF(), D3DPOOL_DEFAULT, &m_VB, NULL);
	}

	~CIndexedVertexs();

	virtual bool Render(CRenderManager *RM);
	virtual inline unsigned short GetVertexType() const { return T::GetVertexType(); }

protected:
	inline size_t GetVertexSize() { return sizeof(T); }
	inline size_t GetIndezSize() { return sizeof(unsigned short); }
};

#endif