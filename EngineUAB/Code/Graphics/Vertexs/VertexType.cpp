#include "Base.h"
#include "Core.h"
#include "RenderManager.h"
#include "VertexType.h"

#if defined(_DEBUG)
#include "Memory\MemLeaks.h"
#endif

LPDIRECT3DVERTEXDECLARATION9 TGEOMETRY_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TGEOMETRY_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TCOLORED_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TCOLORED_VERTEX::s_VertexDeclarationInstance=NULL;
	
LPDIRECT3DVERTEXDECLARATION9& TCOLORED_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
			
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TCOLORED_VERTEX::GetVertexDeclarationInstance()
{
	if(s_VertexDeclarationInstance==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
			
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },

			{ 1, 0  , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 0 },

			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclarationInstance);
	}

	return s_VertexDeclarationInstance;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMALCOLORED_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMALCOLORED_VERTEX::s_VertexDeclarationInstance=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMALCOLORED_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
			
			{ 0, 20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALCOLORED_VERTEX::GetVertexDeclarationInstance()
{
	if(s_VertexDeclarationInstance==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
			
			{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },

			{ 1, 0  , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 0 },

			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclarationInstance);
	}

	return s_VertexDeclarationInstance;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TTEXTURE1_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TTEXTURE1_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////
	
LPDIRECT3DVERTEXDECLARATION9 TTEXTURE2_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TTEXTURE2_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
				
			{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 1 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TCOLOREDTEXTURE1_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TCOLOREDTEXTURE1_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },
				
			{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////
	
LPDIRECT3DVERTEXDECLARATION9 TCOLOREDTEXTURE2_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TCOLOREDTEXTURE2_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
				
			{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 1 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////
	
LPDIRECT3DVERTEXDECLARATION9 TNORMALCOLOREDTEXTURE1_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMALCOLOREDTEXTURE1_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },
				
			{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMALCOLOREDTEXTURE2_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMALCOLOREDTEXTURE2_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },
				
			{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
				
			{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 1 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMALTEXTURE1_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMALTEXTURE1_VERTEX::s_VertexDeclarationInstance=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMALTEXTURE1_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
			
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
			
			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTEXTURE1_VERTEX::GetVertexDeclarationInstance()
{
	if(s_VertexDeclarationInstance==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION, 0 },

			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL, 0 },

			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 0 },

			{ 1, 0  , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 4 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclarationInstance);
	}

	return s_VertexDeclarationInstance;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMALTEXTURE2_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMALTEXTURE2_VERTEX::s_VertexDeclarationInstance=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMALTEXTURE2_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
				
			{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 1 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTEXTURE2_VERTEX::GetVertexDeclarationInstance()
{
	if(s_VertexDeclarationInstance==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION, 0 },

			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL, 0 },

			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 0 },

			{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 40 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 56, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			{ 1, 72, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 4 },

			{ 1, 88, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 5 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclarationInstance);
	}

	return s_VertexDeclarationInstance;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMALBINORMAL_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMALBINORMAL_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 CAL3D_HW_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& CAL3D_HW_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BLENDWEIGHT, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BLENDINDICES, 0 },
				
			{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 CAL3D_HW_VERTEX_BT::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 CAL3D_HW_VERTEX_BT::s_VertexDeclarationInstance=NULL;

LPDIRECT3DVERTEXDECLARATION9& CAL3D_HW_VERTEX_BT::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BLENDWEIGHT, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BLENDINDICES, 0 },
				
			{ 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 60, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TANGENT, 0 },
				
			{ 0, 76, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BINORMAL, 0 },
				
			{ 0, 92, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& CAL3D_HW_VERTEX_BT::GetVertexDeclarationInstance()
{
	if(s_VertexDeclarationInstance==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BLENDWEIGHT, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BLENDINDICES, 0 },
				
			{ 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 60, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TANGENT, 0 },
				
			{ 0, 76, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BINORMAL, 0 },
				
			{ 0, 92, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			{ 1, 0 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 4 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclarationInstance);
	}

	return s_VertexDeclarationInstance;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::s_VertexDeclarationInstance=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TANGENT, 0 },
				
			{ 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BINORMAL, 0 },
							
			{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexDeclarationInstance()
{
	if(s_VertexDeclarationInstance==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TANGENT, 0 },
				
			{ 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BINORMAL, 0 },
							
			{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },

			{ 1, 0 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 4 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclarationInstance);
	}

	return s_VertexDeclarationInstance;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX::s_VertexDeclaration=NULL;
LPDIRECT3DVERTEXDECLARATION9 TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX::s_VertexDeclarationInstance=NULL;

LPDIRECT3DVERTEXDECLARATION9& TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TANGENT, 0 },
				
			{ 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BINORMAL, 0 },
							
			{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
							
			{ 0, 68, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 1 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
																		&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMAL_TANGENT_BINORMAL_TEXTURED2_VERTEX::GetVertexDeclarationInstance()
{
	if(s_VertexDeclarationInstance==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION, 0 },
				
			{ 0, 12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TANGENT, 0 },
				
			{ 0, 44, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BINORMAL, 0 },
							
			{ 0, 60, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 0 },
							
			{ 0, 68, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 0 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 4 },

			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 5 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclarationInstance);
	}

	return s_VertexDeclarationInstance;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TPARTICLE_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TPARTICLE_VERTEX::GetVertexDeclaration()
{

	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 0, 0 , D3DDECLTYPE_SHORT2, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION, 0 },

			{ 0, 4, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_POSITION, 1 },

			{ 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_NORMAL, 0 },

			{ 0, 28, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 0 },

			{ 0, 32, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_COLOR, 0 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TINSTANCE_VERTEX::s_VertexDeclaration=NULL;

LPDIRECT3DVERTEXDECLARATION9& TINSTANCE_VERTEX::GetVertexDeclaration()
{
	if(s_VertexDeclaration==NULL)
	{
		D3DVERTEXELEMENT9 l_VertexDeclaration[] =
		{
			{ 1, 0 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 0 },

			{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 1 },

			{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 2 },

			{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
			D3DDECLUSAGE_TEXCOORD, 3 },

			D3DDECL_END()
		};

		CORE->GetRenderManager()->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration,
			&s_VertexDeclaration);
	}

	return s_VertexDeclaration;
}

/////////////////////////////////////////////////////////////////////////////

void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount, 
								size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
								size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
{
	D3DXVECTOR3 *tan1 = new D3DXVECTOR3[VtxCount * 2];
	D3DXVECTOR3 *tan2 = tan1 + VtxCount;
	ZeroMemory(tan1, VtxCount * sizeof(D3DXVECTOR3) * 2);
	unsigned char *l_VtxAddress=(unsigned char *)VtxsData;

	for(size_t b=0;b<IdxCount;b+=3)
	{
		unsigned short i1=IdxsData[b];
		unsigned short i2=IdxsData[b+1];
		unsigned short i3=IdxsData[b+2];
		
		D3DXVECTOR3 *v1=(D3DXVECTOR3 *)
		&l_VtxAddress[i1*VertexStride+GeometryStride];
		D3DXVECTOR3 *v2=(D3DXVECTOR3 *)
		&l_VtxAddress[i2*VertexStride+GeometryStride];
		D3DXVECTOR3 *v3=(D3DXVECTOR3 *)
		&l_VtxAddress[i3*VertexStride+GeometryStride];

		D3DXVECTOR2 *w1=(D3DXVECTOR2 *)
		&l_VtxAddress[i1*VertexStride+TextureCoordsStride];
		D3DXVECTOR2 *w2=(D3DXVECTOR2 *)
		&l_VtxAddress[i2*VertexStride+TextureCoordsStride];
		D3DXVECTOR2 *w3=(D3DXVECTOR2 *)
		&l_VtxAddress[i3*VertexStride+TextureCoordsStride];

		float x1=v2->x-v1->x;
		float x2=v3->x-v1->x;

		float y1=v2->y-v1->y;
		float y2=v3->y-v1->y;
		
		float z1=v2->z-v1->z;
		float z2=v3->z-v1->z;
		
		float s1=w2->x-w1->x;
		float s2=w3->x-w1->x;
		
		float t1=w2->y-w1->y;
		float t2=w3->y-w1->y;
		
		float r = 1.0F / (s1 * t2 - s2 * t1);

		D3DXVECTOR3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		D3DXVECTOR3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
		
		assert(i1<VtxCount);
		assert(i2<VtxCount);
		assert(i3<VtxCount);
		
		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (size_t b=0;b<VtxCount;++b)
	{
		D3DXVECTOR3 *l_NormalVtx=(D3DXVECTOR3 *)
		&l_VtxAddress[b*VertexStride+NormalStride];
		
		D3DXVECTOR3 *l_TangentVtx=(D3DXVECTOR3 *)
		&l_VtxAddress[b*VertexStride+TangentStride];
		
		D3DXVECTOR4 *l_TangentVtx4=(D3DXVECTOR4 *)
		&l_VtxAddress[b*VertexStride+TangentStride];
		
		D3DXVECTOR3 *l_BiNormalVtx=(D3DXVECTOR3 *)
		&l_VtxAddress[b*VertexStride+BiNormalStride];

		const D3DXVECTOR3& t=tan1[b];
		// Gram-Schmidt orthogonalize
		D3DXVECTOR3 l_VAl=t-(*l_NormalVtx)*D3DXVec3Dot(l_NormalVtx, &t);
		D3DXVec3Normalize(l_TangentVtx,&l_VAl);
		//tangent[a] = (t - n * Dot(n, t)).Normalize();
		// Calculate handedness
		D3DXVECTOR3 l_Cross;
		D3DXVec3Cross(&l_Cross,l_NormalVtx,l_TangentVtx);
		l_TangentVtx4->w=(D3DXVec3Dot(&l_Cross,&tan2[b])< 0.0f ) ? -1.0f : 1.0f;
		//tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		D3DXVec3Cross(l_BiNormalVtx,l_NormalVtx,l_TangentVtx);
	}

	delete[] tan1;
}


/////////////////////////////////////////////////////////////////////////////