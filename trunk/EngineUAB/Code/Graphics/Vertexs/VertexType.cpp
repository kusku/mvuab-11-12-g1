

#include "Base.h"
#include "Core.h"
#include "RenderManager.h"
#include "VertexType.h"

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

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMALCOLORED_VERTEX::s_VertexDeclaration=NULL;

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
				
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_COLOR, 0 },
				
			{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
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

/////////////////////////////////////////////////////////////////////////////

LPDIRECT3DVERTEXDECLARATION9 TNORMALTEXTURE2_VERTEX::s_VertexDeclaration=NULL;

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