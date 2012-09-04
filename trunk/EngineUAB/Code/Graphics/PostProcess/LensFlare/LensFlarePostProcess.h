#pragma once

#ifndef _LENS_FLARE_POST_PROCESS_H
#define _LENS_FLARE_POST_PROCESS_H

#include <string>
#include <d3dx9.h>
#include <d3d9.h>

#include "Base.h"
#include "Math\Color.h"
#include "Math\Vector3.h"
#include "Math\Vector2.h"
#include "Math\Vector4.h"
#include "Math\Matrix44.h"
#include "Vertexs\VertexType.h"
#include "Graphic States\GraphicStates.h"

class CTexture;
class CEffectTechnique;
class CEffect;

struct SFlare
{
	float			m_Position;
	float			m_Scale;
	CColor			m_Color;
	std::string		m_TextureName;
	CTexture*		m_Texture;
	

	//Constructor
	SFlare(float position = 0, float scale = 0, CColor color = colBLACK, std::string textureName = "");
	~SFlare();
};

class CLensFlarePostProcess
{
	private:

		SFlare						m_Flares[10];
		const float					m_GlowSize; 
		const float					m_QuerySize;
		CColor						m_GlowColor;

		//Light
		bool						m_LightBehindCamera;
		Vect3f						m_LightDirection;
		Vect2f						m_LightPosition;

		//Graphics	
		CTexture*					m_GlowSprite;
		LPDIRECT3DVERTEXBUFFER9		m_VertexBuffer;
		TCOLORED_VERTEX				m_QueryVertices[4];
		const TGraphicBlendStates	m_LensFlarBS;
		CEffectTechnique*			m_BasicTechnique;
		CEffect*					m_Effect;
		D3DXHANDLE					m_WorldParam;
		D3DXHANDLE					m_ViewParam;
		D3DXHANDLE					m_ProjParam;
		
		//Occlusion Query
		bool						m_OcclusionQueryActive;
        float						m_OcclusionAlpha;
		IDirect3DQuery9*			m_OcclusionQuery;
		ID3DXSprite*				m_SpriteBatch;


		void	UpdateOcclusion();
		void	DrawGlows();
		void	DrawFlares();

		Vect3f	Get2DScreenSpace(const Vect3f& pos, const Mat44f& proj, const Mat44f& view);

	public:
		CLensFlarePostProcess();
		virtual ~CLensFlarePostProcess();

		inline void SetGlowColor(const CColor& color)
		{
			m_GlowColor = color;
		}

		void						Init();
		void						Render(const Vect3f& lightDirection);
};

#endif