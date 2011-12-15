fileIn "dotNetFunctions.ms"

global VERTEX_TYPE_GEOMETRY = 0x0001
global VERTEX_TYPE_NORMAL = 0x0002
global VERTEX_TYPE_TANGENT = 0x0004
global VERTEX_TYPE_BINORMAL = 0x0008
global VERTEX_TYPE_TEXTURE1 = 0x0010
global VERTEX_TYPE_TEXTURE2 = 0x0020
global VERTEX_TYPE_DIFFUSE = 0x0040

function CreateMesh Obj Vtxs Idxs MinBox MaxBox=
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
		
		local l_Diffuse = 0
		
		local l_TUV11 = point3 0 0 0
		local l_TUV12 = point3 0 0 0
		local l_TUV13 = point3 0 0 0
		
		local l_TUV21 = point3 0 0 0
		local l_TUV22 = point3 0 0 0
		local l_TUV23 = point3 0 0 0
		
		local l_Material = undefined
		local l_MaterialID = 1
		if classof(Obj.material) == Multimaterial then
		(
			l_MaterialID = getFaceMatID Obj i
			l_Material = Obj.material[l_MaterialID]
		)
		else
		(
			l_MaterialID = 1
			l_Material = Obj.material
		)
		local l_VtxType = GetVertexType l_Material
		
		if(bit.and l_VtxType VERTEX_TYPE_TEXTURE1)==VERTEX_TYPE_TEXTURE1 then
		(
			local l_TUV11= meshop.getMapVert Obj 1 IdxsMap.x
			local l_TUV12= meshop.getMapVert Obj 1 IdxsMap.y
			local l_TUV13= meshop.getMapVert Obj 1 IdxsMap.z
			
			l_TUV11.y = 1 - l_TUV11.y
			l_TUV12.y = 1 - l_TUV12.y
			l_TUV13.y = 1 - l_TUV13.y
			
			if(bit.and l_VtxType VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
			(
				local l_TUV21= meshop.getMapVert Obj 1 IdxsMap.x
				local l_TUV22= meshop.getMapVert Obj 1 IdxsMap.y
				local l_TUV23= meshop.getMapVert Obj 1 IdxsMap.z
				
				l_TUV21.y = 1 - l_TUV21.y
				l_TUV22.y = 1 - l_TUV22.y
				l_TUV23.y = 1 - l_TUV23.y
			)
		)
		else
		(
			l_Diffuse = integer(Obj.material.diffuse.r * 0x01000000 + Obj.material.diffuse.g * 0x00010000 + Obj.material.diffuse.b * 0x00000100 + 0xff)
		)
	
		local l_V1 = CreateVtx l_Vtx1 l_Normal1 l_TUV11 l_TUV21 l_Diffuse l_VtxType 
		local l_V2 = CreateVtx l_Vtx2 l_Normal2 l_TUV12 l_TUV22 l_Diffuse l_VtxType 
		local l_V3 = CreateVtx l_Vtx3 l_Normal3 l_TUV13 l_TUV23 l_Diffuse l_VtxType 
		
		local VtxsDotNet = dotnetobject "System.Collections.Generic.Dictionary`2[System.String,System.Int16]"
		
		local l_Idx = AddVtx Vtxs VtxsDotNet l_V1 l_MaterialID
		AddIdx Idxs l_Idx l_MaterialID
		
		l_Idx = AddVtx Vtxs VtxsDotNet l_V3 l_MaterialID
		AddIdx Idxs l_Idx l_MaterialID
		
		l_Idx = AddVtx Vtxs VtxsDotNet l_V2 l_MaterialID
		AddIdx Idxs l_Idx l_MaterialID
		
		SearchMinBox l_V1 MinBox
		SearchMinBox l_V2 MinBox
		SearchMinBox l_V3 MinBox
		
		SearchMaxBox l_V1 MaxBox
		SearchMaxBox l_V2 MaxBox
		SearchMaxBox l_V3 MaxBox
	)
)

function AddVtx Vtxs VtxsDotNet Vtx MatID =
(
	while Vtxs.count < MatID do
	(
		local l_Vtx = #()
		append Vtxs l_Vtx
	)
	
	--local Pos = FindVtx Vtxs[MatID] Vtx
	local Pos = DotNetAddVtx Vtxs[MatID] VtxsDotNet Vtx
	format "%\n" Pos
	return Pos
)

function AddIdx Idxs Idx MatID =
(	
	while Idxs.count < MatID do
	(
		local l_Idx = #()
		append Idxs l_Idx
	)
	
	append Idxs[MatID] Idx
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

function SearchMinBox VtxModel VtxBox =
(
	if VtxModel[1] < VtxBox.x then
	(
		VtxBox.x = VtxModel[1]
	)
	
	if VtxModel[2] < VtxBox.y then
	(
		VtxBox.y = VtxModel[2]
	)
	
	if VtxModel[3] < VtxBox.z then
	(
		VtxBox.z = VtxModel[3]
	)
)

function SearchMaxBox VtxModel VtxBox =
(
	if VtxModel[1] > VtxBox.x then
	(
		VtxBox.x = VtxModel[1]
	)
	
	if VtxModel[2] > VtxBox.y then
	(
		VtxBox.y = VtxModel[2]
	)
	
	if VtxModel[3] > VtxBox.z then
	(
		VtxBox.z = VtxModel[3]
	)
)

function RHTranslationToLH translation =
(
	return point3 translation.x translation.z translation.y
)

function CalculateRadiusSphere Vtxs Center =
(
	local Radius = 0

	for i = 1 to Vtxs.count do
	(
		for j = 1 to Vtxs[i].count do
		(
			local Ax = pow (Vtxs[i][j][1] - Center.x) 2
			local Ay = pow (Vtxs[i][j][2] - Center.y) 2
			local Az = pow (Vtxs[i][j][3] - Center.z) 2
			local disRadius = sqrt (Ax + Ay + Az)

			if disRadius > Radius then
			(
				Radius = disRadius
			)
		)
	)
	
	return Radius
)

function CalculateCenterSphere MinBox MaxBox =
(
	local center = point3 0 0 0
	center.x = (MaxBox.x + MinBox.x) / 2
	center.y = (MaxBox.y + MinBox.y) / 2
	center.z = (MaxBox.z + MinBox.z) / 2
	
	return center
)

function GetVertexType mat = 
(
	local l_VertexType = bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL
	
	if mat.diffuseMapEnable and (classof mat.diffuseMap)==Bitmaptexture then
	(
		l_VertexType = bit.or l_VertexType VERTEX_TYPE_TEXTURE1
		
		if mat.selfIllumMapEnable and (classof mat.selfIllumMap)==Bitmaptexture then
		(
			l_VertexType = bit.or l_VertexType VERTEX_TYPE_TEXTURE2
		)
		
		if mat.bumpMapEnable and (classof mat.bumpMap)==Bitmaptexture  then
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

function WriteToFile Filename Vtxs Idxs VertexType Mat MinBox MaxBox Center Radius= 
(	
	local file = fopen Filename "wb"
	if file != undefined then
	(
		WriteShort file 0xCACA #unsigned --header
		
		WriteShort file 1 #unsigned
		--WriteShort file VertexType #unsigned 				--Vertexs Type
		
		WriteShort file Mat.numsubs #unsigned
		/*for m = 1 to Mat.numsubs do
		(
			local VtxType = GetVertexType Mat[m]			
		)*/
		--WriteMaterialToFile file  Mat VertexType
		
		--local VtxType = GetVertexType Obj.material[1]
		
		for m = 1 to Mat.numsubs do
		(
			local VtxType = GetVertexType Mat[m]
			WriteMaterialToFile file Mat[m] VtxType
			
			WriteShort file VtxType #unsigned
			
			WriteShort file Vtxs[m].count #unsigned --#Vertexs
			for i = 1 to Vtxs[m].count do
			(	
				WriteVertexToFile file Vtxs[m][i] VtxType
			)
			
			WriteShort file Idxs[m].count #unsigned --#Indexs
			for i = 1 to Idxs[m].count do
			(
				WriteShort file Idxs[m][i] #unsigned 
			)
		)
		
		for i = 1 to 3 do
		(
			WriteFloat file MinBox[i]
		)
		
		for i = 1 to 3 do
		(
			WriteFloat file MaxBox[i]
		)
		
		for i = 1 to 3 do
		(
			WriteFloat file Center[i]
		)
		
		WriteFloat file Radius
		
		WriteShort file 0xACAC #unsigned --footer
		
		fflush file
		fclose file
	)
)

function ExportMesh Obj Path=
(
	local VB = #(#())
	local IB = #(#())
	local MinBox = point3 0 0 0
	local MaxBox = point3 0 0 0
	local Center = point3 0 0 0
	local Radius
	CreateMesh Obj VB IB MinBox MaxBox --read mesh
	
	Center = CalculateCenterSphere MinBox MaxBox
	Radius = CalculateRadiusSphere VB Center
		
	WriteToFile Path VB IB VtxType Obj.material MinBox MaxBox Center Radius
)