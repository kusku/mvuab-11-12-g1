global VERTEX_TYPE_GEOMETRY = 0x0001
global VERTEX_TYPE_NORMAL = 0x0002
global VERTEX_TYPE_TANGENT = 0x0004
global VERTEX_TYPE_BINORMAL = 0x0008
global VERTEX_TYPE_TEXTURE1 = 0x0010
global VERTEX_TYPE_TEXTURE2 = 0x0020
global VERTEX_TYPE_DIFFUSE = 0x0040

function CreateMesh Obj Vtxs Idxs=
(	
	local numFaces = getNumFaces Obj
	for i = 1 to numFaces do
	(
		local IdxsFace = getFace Obj i
		local IdxsMap = meshop.getMapFace Obj 1 i
		
		local l_Vtx1 = getVert Obj IdxsFace.x
		local l_Vtx2 = getVert Obj IdxsFace.y
		local l_Vtx3 = getVert Obj IdxsFace.z
		
		local mat = inverse(Obj.transform)
		l_Vtx1 = l_Vtx1 * mat
		l_Vtx2 = l_Vtx2 * mat
		l_Vtx3 = l_Vtx3 * mat
		
		l_Vtx1 = RHTranslationToLH l_Vtx1
		l_Vtx2 = RHTranslationToLH l_Vtx2
		l_Vtx3 = RHTranslationToLH l_Vtx3
		
		local l_Normal1 = getNormal Obj IdxsFace.x
		local l_Normal2 = getNormal Obj IdxsFace.y
		local l_Normal3 = getNormal Obj IdxsFace.z
		
		local l_Diffuse0 = 0
		
		local l_TUV11 = point3 0 0 0
		local l_TUV12 = point3 0 0 0
		local l_TUV13 = point3 0 0 0
		
		local l_TUV21 = point3 0 0 0
		local l_TUV22 = point3 0 0 0
		local l_TUV23 = point3 0 0 0

		local l_VtxType = GetVertexType Obj.material
		if(bit.and l_VtxType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
		(
			local l_TUV11= meshop.getMapVert Obj 1 IdxsMap.x
			local l_TUV12= meshop.getMapVert Obj 1 IdxsMap.y
			local l_TUV13= meshop.getMapVert Obj 1 IdxsMap.z
			
			if(bit.and l_VtxType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
			(
				local l_TUV21= meshop.getMapVert Obj 1 IdxsMap.x
				local l_TUV22= meshop.getMapVert Obj 1 IdxsMap.y
				local l_TUV23= meshop.getMapVert Obj 1 IdxsMap.z
			)
		)
		else
		(
			l_Diffuse = integer(Obj.material.diffuse.r * 0x01000000 + Obj.material.diffuse.g * 0x00010000 + Obj.material.diffuse.b * 0x00000100 + 0xff)
		)
		
		local l_V1 = CreateVtx l_Vtx1 l_Normal1 l_TUV11 l_TUV21 l_Diffuse l_VtxType
		local l_V2 = CreateVtx l_Vtx2 l_Normal2 l_TUV12 l_TUV22 l_Diffuse l_VtxType
		local l_V3 = CreateVtx l_Vtx3 l_Normal3 l_TUV13 l_TUV23 l_Diffuse l_VtxType
		
		
		local l_Idx = AddVtx Vtxs l_V1
		AddIdx Idxs l_Idx
		
		l_Idx = AddVtx Vtxs l_V2
		AddIdx Idxs l_Idx
		
		l_Idx = AddVtx Vtxs l_V3
		AddIdx Idxs l_Idx
	)
)

function AddVtx Vtxs Vtx =
(
	local Pos = FindVtx Vtxs Vtx
	
	if Pos == -1 then
	(
		append Vtxs Vtx
		Pos = Vtxs.count
	)
	
	return Pos
)

function AddIdx Idxs Idx =
(
	append Idxs Idx
)

function FindVtx Vtxs Vtx =
(
	local PosVtx = -1
	for i = 1 to Vtxs.count do
	(
		local found = Compare Vtx Vtxs[i]
		if found == true then 
		(
			PosVtx = i
		)
	)
	
	return PosVtx
)

function CreateVtx Vtx Normal TUV TUV2 DiffuseColor VertexType =
(
	local Vertex = #()
		
	append Vertex Vtx.x
	append Vertex Vtx.y
	append Vertex Vtx.z
	
	append Vertex Normal.x
	append Vertex Normal.y
	append Vertex Normal.z
	
	if(bit.and VertexType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
	(
		append Vertex TUV.x	
		append Vertex TUV.y
		
		if(bit.and VertexType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
		(
			append Vertex TUV2.x	
			append Vertex TUV2.y
		)
	)
	else
	(
		append Vertex DiffuseColor
	)
	
	return Vertex
)

function Compare Vtx1 Vtx2 =
(
	if Vtx1.count == Vtx2.count then
	(
		for i = 1 to Vtx1.count do
		(
			if Vtx1[i] != Vtx2[i] then
			(
				return false
			)
		)
		
		return true
	)
	
	return false
)

function RHTranslationToLH translation =
(
	return point3 translation.x translation.z translation.y
)

function GetVertexType material = 
(
	local l_VertexType = bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL
	
	if material.diffuseMapEnable and (classof material.diffuseMap)==Bitmaptexture then
	(
		l_VertexType = bit.or l_VertexType VERTEX_TYPE_TEXTURE1
		
		if material.selfIllumMapEnable and (classof material.selfIllumMap)==Bitmaptexture then
		(
			l_VertexType = bit.or l_VertexType VERTEX_TYPE_TEXTURE2
		)
		
		if material.bumpMapEnable and (classof material.bumpMap)==Bitmaptexture  then
		(
			l_VertexType = bit.or l_VertexType VERTEX_TYPE_TANGENT
			l_VertexType = bit.or l_VertexType VERTEX_TYPE_BINORMAL
		)
	)
	else
	(	
		l_VertexType = bit.or l_VertexType VERTEX_TYPE_DIFFUSE
	)
	
	return l_VertexType
)

function WriteBitmapName File Filename =
(
	WriteShort file Filename.count #unsigned --Material Path Length
	WriteString file Filename--Material Path
)

function WriteMaterialToFile File Material VertexType=
(	
	if (bit.and VertexType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
	(
		WriteBitmapName File (openBitMap Material.diffuseMap.bitmap.filename).fileName 
	)
	
	if (bit.and VertexType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
	(
		WriteBitmapName File  (openBitMap Material.selfIllumMap.bitmap.filename).fileName 
	)
	
	if (bit.and VertexType VERTEX_TYPE_TANGENT)==VERTEX_TYPE_TANGENT then
	(
		WriteBitmapName File  (openBitMap Material.bumpMap.bitmap.filename).fileName 
	)
)

function WriteVertexToFile File Vtx VertexType =
(
	if (bit.and VertexType VERTEX_TYPE_DIFFUSE)==VERTEX_TYPE_DIFFUSE then
	(
		WriteFloat File Vtx[1] --Geometry
		WriteFloat File Vtx[2]
		WriteFloat File Vtx[3]
		
		WriteFloat File Vtx[4] --Normals
		WriteFloat File Vtx[5]
		WriteFloat File Vtx[6]
		WriteLong File Vtx[7] #unsigned
	)
	else
	(
		for i = 1 to Vtx.count do
		(
			WriteFloat File Vtx[i]
		)
	)
)

function WriteToFile Filename Vtxs Idxs VertexType Material = 
(	
	local file = fopen Filename "wb"
	if file != undefined then
	(
		WriteShort file 0xCACA #unsigned --header
		
		WriteShort file VertexType #unsigned 				--Vertexs Type
		
		WriteMaterialToFile file  Material VertexType

		WriteShort file Vtxs.count #unsigned --#Vertexs
		for i = 1 to Vtxs.count do
		(	
			WriteVertexToFile file Vtxs[i] VertexType
		)
		
		WriteShort file Idxs.count #unsigned --#Indexs
		for i = 1 to Idxs.count do
		(
			WriteShort file Idxs[i] #unsigned 
		)
		
		WriteShort file 0xACAC #unsigned --footer
		
		fflush file
		fclose file
	)
)

function main Obj =
(
	local VB = #()
	local IB = #()
	
	CreateMesh Obj VB IB --read mesh
	
	local VtxType = GetVertexType Obj.material
	WriteToFile "C:\Users\Marc\Documents\UAB\Màster de Creació de Videojocs\EngineUAB\Run\Viewer\Data\box.m3d" VB IB VtxType Obj.material
	
	format "%\n %\n %\n %\n" VB VB.count IB IB.count
)

main $