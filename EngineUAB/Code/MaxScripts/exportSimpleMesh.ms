global VB = #()
global IB = #()

function CreateMesh Obj =
(
	numFaces = getNumFaces Obj
	for i = 1 to numFaces do
	(
		local IdxsFace = getFace Obj i
		local IdxsMap = meshop.getMapFace Obj 1 i
		
		local Vtx1 = getVert Obj IdxsFace.x
		local Vtx2 = getVert Obj IdxsFace.y
		local Vtx3 = getVert Obj IdxsFace.z
		
		local Normal1 = getNormal Obj IdxsFace.x
		local Normal2 = getNormal Obj IdxsFace.y
		local Normal3 = getNormal Obj IdxsFace.z
		
		local TUVMap1= meshop.getMapVert Obj 1 IdxsMap.x
		local TUVMap2= meshop.getMapVert Obj 1 IdxsMap.y
		local TUVMap3= meshop.getMapVert Obj 1 IdxsMap.z
		
		local V1 = CreateVtx Vtx1 Normal1 TUVMap1
		local V2 = CreateVtx Vtx2 Normal2 TUVMap2
		local V3 = CreateVtx Vtx3 Normal3 TUVMap3
		
		local idx = AddVtx VB V1
		AddIdx IB idx
		
		idx = AddVtx VB V2
		AddIdx IB idx
		
		idx = AddVtx VB V2
		AddIdx IB idx
	)
	
	format "%\n %\n %\n %\n" VB VB.count IB IB.count
)

function AddVtx Vtxs Vtx =
(
	local Pos = FindVtx Vtxs Vtx
	
	if Pos == -1 then
	(
		append VB Vtx
		Pos = VB.count
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

function CreateVtx Vtx Normal TUVMap =
(
	local Vertex = #()
		
	append Vertex Vtx.x
	append Vertex Vtx.y
	append Vertex Vtx.z
	
	append Vertex Normal.x
	append Vertex Normal.y
	append Vertex Normal.z
	
	append Vertex TUVMap.x	
	append Vertex TUVMap.y
	
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

function CreateFileMesh Obj FileName= 
(
	CreateMesh Obj --read mesh
	
	local file = fopen FileName "wb"
	
	WriteShort file 0xCACA #unsigned --header
	WriteShort file 1 #unsigned --1
	
	local matType = classof(Obj.material)
	if matType == Multimaterial then
	(
		local numMaterials = Obj.material.numsubs
		WriteShort file numMaterials --Num. Materials
		WriteString file "Multimaterial"
		for i = 1 to numMaterials do
		(
			local mat = (openBitMap Obj.material[i].diffuseMap.bitmap.filename).fileName
			WriteString file mat
		)
	)
	else
	(
		WriteShort file 1 #unsigned --Num. Materials
		WriteString file "Standardmaterial"
		local mat = (openBitMap Obj.material.diffuseMap.bitmap.filename).fileName
		WriteString file mat
	)
	
	WriteShort file VB.count #unsigned --#Vertexs
	for i = 1 to VB.count do
	(
		WriteFloat file VB[i][1] --Geometry
		WriteFloat file VB[i][2]
		WriteFloat file VB[i][3]
		
		WriteFloat file VB[i][4] --Normals
		WriteFloat file VB[i][5]
		WriteFloat file VB[i][6]
		
		WriteFloat file VB[i][7] --Texture Coords
		WriteFloat file VB[i][8]
	)
	
	WriteShort file IB.count #unsigned --#Indexs
	for i = 1 to IB.count do
	(
		WriteShort file IB[i] #unsigned 
	)
	WriteShort file 0xACAC #unsigned --footer
	
	fflush file
	fclose file
)
CreateFileMesh $ "C:\Users\Marc\Documents\UAB\Màster de Creació de Videojocs\EngineUAB\Run\Viewer\Data\box.m3d"