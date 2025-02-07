/*******************************************************

Name: MeshObject.ms

Use: Structs to store the Mesh Object information
		- Material

Last Update: 12/15/2011

********************************************************/

--Includes------------------------------------
fileIn "HelperFunctions.ms"
------------------------------------------------

/**************************************
    Globals
**************************************/
global VERTEX_TYPE_GEOMETRY	= 0x0001;
global VERTEX_TYPE_NORMAL		= 0x0002;
global VERTEX_TYPE_TANGENT		= 0x0004;
global VERTEX_TYPE_BINORMAL		= 0x0008;
global VERTEX_TYPE_TEXTURE1	= 0x0010;
global VERTEX_TYPE_TEXTURE2	= 0x0020;
global VERTEX_TYPE_DIFFUSE		= 0x0040;
global VERTEX_TYPE_PARALLAX		= 0x0200;
global VERTEX_TYPE_RNM			= 0x0400;
global VERTEX_TYPE_TEXSPLAT	= 0x2000;

/**************************************
    Definitions
**************************************/

struct TextureSplattingObj
(
	TSEnable,
	TSBlend1,
	TSBlend2,
	TSBlend3,
	TSBlend4
)

struct MaterialObject
(
	CompletePath,
	RelativePath,
	Texture1,
	Texture2,
	Bump,
	RNMTextureX,
	RNMTextureY,
	RNMTextureZ,
	
	function WriteToFile vertexType textureSplatting file =
	(
		local finalName = ""
		
		if (bit.and vertexType VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 then
		(
			finalName = (GetRelativeFileNameWithParentDir Texture1 RelativePath)
			WriteStringToFile finalName file
		
			if (bit.and vertexType VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 then
			(
				finalName = (GetRelativeFileNameWithParentDir Texture2 RelativePath)
				WriteStringToFile finalName file
			)
			
			if (textureSplatting.TSEnable == true) then
			(
				finalName = (GetRelativeFileNameWithParentDir textureSplatting.TSBlend1 RelativePath)
				WriteStringToFile finalName file
				
				finalName = (GetRelativeFileNameWithParentDir textureSplatting.TSBlend2 RelativePath)
				WriteStringToFile finalName file
				
				finalName = (GetRelativeFileNameWithParentDir textureSplatting.TSBlend3 RelativePath)
				WriteStringToFile finalName file
				
				finalName = (GetRelativeFileNameWithParentDir textureSplatting.TSBlend4 RelativePath)
				WriteStringToFile finalName file
			)
		
			if (bit.and vertexType VERTEX_TYPE_RNM) == VERTEX_TYPE_RNM then
			(
				finalName = (GetRelativeFileNameWithParentDir RNMTextureX RelativePath)
				WriteStringToFile finalName file
				finalName = (GetRelativeFileNameWithParentDir RNMTextureY RelativePath)
				WriteStringToFile finalName file
				finalName = (GetRelativeFileNameWithParentDir RNMTextureZ RelativePath)
				WriteStringToFile finalName file
			)
		
			if (bit.and vertexType VERTEX_TYPE_TANGENT) == VERTEX_TYPE_TANGENT then
			(
				finalName = (GetRelativeFileNameWithParentDir Bump RelativePath)
				WriteStringToFile finalName file
			)
		)
	),
	
	function CopyTexturesToOutput =
	(
		--Final output directory
		local outputDir = (CompletePath + TEXTURE_SUBDIR)

		--Make sure output dir exists
		makeDir outputDir all:true
		
		--Texture FileNames
		local destFile = ""
				
		--Make sure texture1 is set
		if Texture1 != "" then
		(
			destFile = (GetCompleteFileName Texture1 CompletePath)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile Texture1 destFile
		)
		
		--Make sure texture2 is set
		if Texture2 != "" then
		(
			destFile = (GetCompleteFileName Texture2 CompletePath)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile Texture2 destFile
		)
		
		--Make sure RNMTextureX is set
		if RNMTextureX != "" then
		(
			destFile = (GetCompleteFileName RNMTextureX CompletePath)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile RNMTextureX destFile
		)
		
		--Make sure RNMTextureY is set
		if RNMTextureY != "" then
		(
			destFile = (GetCompleteFileName RNMTextureY CompletePath)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile RNMTextureY destFile
		)
		
		--Make sure RNMTextureZ is set
		if RNMTextureZ != "" then
		(
			destFile = (GetCompleteFileName RNMTextureZ CompletePath)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile RNMTextureZ destFile
		)
		
		--Make sure bump is set
		if Bump != "" then
		(
			destFile = (GetCompleteFileName Bump CompletePath)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile Bump destFile
		)
	),
	
	function PrintMaterialTest =
	(
		format "Complete Path: % \n" CompletePath
		format "Relative Path: % \n" RelativePath
		
		if Texture1 != "" then
		(
			format "Original Texture 1: % \n" Texture1
			format "Relative Texture 1: % \n" (GetRelativeFileName Texture1 RelativePath)
			format "Target Texture 1: % \n" (GetCompleteFileName Texture1 CompletePath)
		)
		else
		(
			format "Texture 1 is not set\n"
		)
		
		if Texture2 != "" then
		(
			format "Original Texture 2: % \n" Texture2
			format "Relative Texture 2: % \n" (GetRelativeFileName Texture2 RelativePath)
			format "Target Texture 2: % \n" (GetCompleteFileName Texture2 CompletePath)
		)
		else
		(
			format "Texture 2 is not set\n"
		)
		
		if RNMTextureX != "" then
		(
			format "Original RNM Texture X: % \n" RNMTextureX
			format "Relative RNM Texture X: % \n" (GetRelativeFileName RNMTextureX RelativePath)
			format "Target RNM Texture X: % \n" (GetCompleteFileName RNMTextureX CompletePath)
		)
		else
		(
			format "RNM Texture X is not set\n"
		)
		
		if RNMTextureY != "" then
		(
			format "Original RNM Texture Y: % \n" RNMTextureY
			format "Relative RNM Texture Y: % \n" (GetRelativeFileName RNMTextureY RelativePath)
			format "Target RNM Texture Y: % \n" (GetCompleteFileName RNMTextureY CompletePath)
		)
		else
		(
			format "RNM Texture Y is not set\n"
		)
		
		if RNMTextureZ != "" then
		(
			format "Original RNM Texture Z: % \n" RNMTextureZ
			format "Relative RNM Texture Z: % \n" (GetRelativeFileName RNMTextureZ RelativePath)
			format "Target RNM Texture Z: % \n" (GetCompleteFileName RNMTextureZ CompletePath)
		)
		else
		(
			format "RNM Texture Z is not set\n"
		)
		
		if Bump != "" then
		(
			format "Original Bump: % \n" Bump
			format "Relative Bump: % \n" (GetRelativeFileName Bump RelativePath)
			format "Target Bump: % \n" (GetCompleteFileName Bump CompletePath)
		)
		else
		(
			format "Bump is not set\n"
		)
	)
)

struct Vertex
(
	Pos,
	Nrm,
	Tex1,
	Tex2,
	Diff,

	function WriteToFile vertexType textureSplatting file =
	(
		-- Write Geometry
		WriteFloat file Pos.x
		WriteFloat file Pos.y
		WriteFloat file Pos.z
		
		--Normals
		WriteFloat file Nrm.x
		WriteFloat file Nrm.y
		WriteFloat file Nrm.z
		
		if(bit.and VertexType VERTEX_TYPE_TANGENT) == VERTEX_TYPE_TANGENT and
			(bit.and VertexType VERTEX_TYPE_BINORMAL) == VERTEX_TYPE_BINORMAL then
		(
			WriteFloat file 0.0 --Normal.w
			
			WriteFloat file 0.0  --Tangent
			WriteFloat file 0.0 
			WriteFloat file 0.0 
			WriteFloat file 0.0 
			
			WriteFloat file 0.0  --Binormal
			WriteFloat file 0.0 
			WriteFloat file 0.0 
			WriteFloat file 0.0 
		)
		
		if (bit.and VertexType VERTEX_TYPE_DIFFUSE) == VERTEX_TYPE_DIFFUSE then
		(
			--Write Diffuse Color Vertex to file
			WriteLong file Diff
		)
		
		if (bit.and VertexType VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 then
		(
			--Write Texture Coordinates for texture 1
			WriteFloat file Tex1.x
			WriteFloat file Tex1.y
		)
		
		if ( (bit.and VertexType VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 ) OR
           ( (bit.and VertexType VERTEX_TYPE_RNM) == VERTEX_TYPE_RNM ) OR
		   ( textureSplatting.TSEnable == true ) then
		(
			--Write Texture Coordinates for texture 2
			WriteFloat file Tex2.x
			WriteFloat file Tex2.y
		)
	),
	
	function ToString =
	(
		local stVtx = stringstream ""
		
		format "% % % % %" Pos Nrm Tex1 Tex2 Diff to:stVtx
		
		return (stVtx as string)
	)
)

struct VIBuffers
(
	MatID,
	VertexType,
	VertexBuffer,
	VertexMap,
	IndexBuffer,
	MeshMaterial,

	function AddVertexToBuffer vtx =
	(
		local stVtx = vtx.ToString()
		local idx = 0
	
		if ( VertexMap.ContainsKey(stVtx) ) == false then
		(
			append vertexBuffer vtx
			idx = (vertexBuffer.count)
		
			VertexMap.Add stVtx idx
		)
		else
		(
			idx = VertexMap.Item[stVtx]
		)
	
		return idx
	),

	function AddIndexToBuffer idx = 
	(
		append IndexBuffer idx
	),
	
	function AddVertexAndIndex Vtx1 Vtx2 Vtx3 CounterClockWise =
	(
		local idx1 = (AddVertexToBuffer Vtx1)
		local idx2 = (AddVertexToBuffer Vtx2)
		local idx3 = (AddVertexToBuffer Vtx3)
		
		if CounterClockWise == true then
		(   
			-- Insert Indexes in counterclockwise 
			AddIndexToBuffer idx1
			AddIndexToBuffer idx3
			AddIndexToBuffer idx2
		)
		else
		(   
			-- Insert Indexes in clockwise 
			AddIndexToBuffer idx1
			AddIndexToBuffer idx2
			AddIndexToBuffer idx3
		)
	),

	function SetVertexTypeAndMaterial objMaterial =
	(	
		--Initialize Vertex with default options
		VertexType = (bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL)
		
		if objMaterial != undefined then
		(
			if objMaterial.diffuseMapEnable == true AND classOf(objMaterial.diffuseMap) == BitmapTexture then
			(
				MeshMaterial = CreateMaterialObjectStruct()
				
				VertexType = (bit.or VertexType VERTEX_TYPE_TEXTURE1)
				MeshMaterial.Texture1 = (openBitMap objMaterial.diffuseMap.bitmap.filename).fileName
			
				if objMaterial.selfIllumMapEnable == true then
				(
					if classOf(objMaterial.selfIllumMap) == BitmapTexture then
					(
						VertexType = (bit.or VertexType VERTEX_TYPE_TEXTURE2)
						MeshMaterial.Texture2 = (openBitMap objMaterial.selfIllumMap.bitmap.filename).fileName
					)
					else if classOf(objMaterial.selfIllumMap) == Multi_Sub_Map then
					(
						VertexType = (bit.or VertexType VERTEX_TYPE_RNM)
						MeshMaterial.RNMTextureX = (openBitMap objMaterial.selfIllumMap.id_0_color_shader.filename).fileName
						MeshMaterial.RNMTextureY = (openBitMap objMaterial.selfIllumMap.id_1_color_shader.filename).fileName
						MeshMaterial.RNMTextureZ = (openBitMap objMaterial.selfIllumMap.id_2_color_shader.filename).fileName
					)
				)
				
				if objMaterial.bumpMapEnable == true AND classOf(objMaterial.bumpMap) == BitmapTexture then
				(
					VertexType = (bit.or VertexType (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) )
					MeshMaterial.Bump = (openBitMap objMaterial.bumpMap.bitmap.filename).fileName
					
					if objMaterial.DisplacementMapEnable == true  then
					(
						VertexType = (bit.or VertexType VERTEX_TYPE_PARALLAX )	
					)
				)
			)
			else
			(
				MeshMaterial = CreateMaterialObjectStruct()
				
				VertexType = (bit.or VertexType VERTEX_TYPE_DIFFUSE)
			)
		)
	),
	
	function SetCompleteAndRelativePath CompletePath RelativePath =
	(
		MeshMaterial.CompletePath = CompletePath
		MeshMaterial.RelativePath = RelativePath
	),
	
	function CopyTexturesToOutput =
	(
		MeshMaterial.CopyTexturesToOutput()
	),

	------------------------------------------------------------
	------------------ File Write Functions -----------------
	------------------------------------------------------------
	function WriteIndexBufferToFile file	=
	(
		--Write how many indexes we have
		WriteLong file IndexBuffer.count #unsigned
		
		-- Write the index buffer to file
		for i = 1 to IndexBuffer.count do
		(
			--Set indexes to start from 0 and write to file
			WriteShort file (IndexBuffer[i] - 1) #unsigned
		)
	),

	function WriteVertexBufferToFile textureSplatting file =
	(
		--Write how many vertexs we have
		WriteLong file VertexBuffer.count #unsigned
		
		--Write the vertex buffer to file
		for i = 1 to VertexBuffer.count do
		(
			VertexBuffer[i].WriteToFile VertexType textureSplatting file
		)
	),

	function WriteToFile textureSplatting file = 
	(
		local vtxType = VertexType
		
		if textureSplatting.TSEnable == true then
		(
			vtxType = (bit.or VertexType VERTEX_TYPE_TEXSPLAT)
		)
		
		--Write Vertex Type to file
		WriteShort file vtxType
		
		--Write Materials to File
		MeshMaterial.WriteToFile VertexType textureSplatting file
		
		--Write Index Buffer to File
		WriteIndexBufferToFile file
		
		--Write Vertex Buffer to File
		WriteVertexBufferToFile textureSplatting file
	),
	------------------------------------------------------------
	---------------End File Write Functions ----------------
	------------------------------------------------------------
	
	------------------------------------------------------------
	-------------------- Debug Functions -------------------- 
	------------------------------------------------------------
	function PrintVertexBufferTestXNA =
	(
		for i = 1 to VertexBuffer.count do
		(
			format "position = new Vector3(%f, %f, %f);\n" VertexBuffer[i].Pos.x VertexBuffer[i].Pos.y VertexBuffer[i].Pos.z
			format "normal = new Vector3(%f, %f, %f);\n" VertexBuffer[i].Nrm.x VertexBuffer[i].Nrm.y VertexBuffer[i].Nrm.z
			format "textureCoordinates = new Vector2(%f, %f);\n" VertexBuffer[i].Tex1.x VertexBuffer[i].Tex1.y
			format "textureCoordinates2 = new Vector2(%f, %f);\n" VertexBuffer[i].Tex2.x VertexBuffer[i].Tex2.y
			format "vertices[ % ] = new VertexPositionNormalTexture(position, normal, textureCoordinates);\n\n" (i - 1)
		)
	),

	function PrintIndexBufferTestXNA =
	(		
		for i = 1 to IndexBuffer.count do
		(
			format "indices[ % ] = % ;\n" (i - 1) (IndexBuffer[i] - 1)
		)
	),
	
	function PrintVertexBufferTestDX =
	(
		format "TVERTEX_TEXTURE1_NORMAL l_Vrt[%] = {" VertexBuffer.count
		
		for i = 1 to vertexBuffer.count do
		(
			format "{"
			format "%f, %f, %f, " VertexBuffer[i].Pos.x VertexBuffer[i].Pos.y VertexBuffer[i].Pos.z
			format "%f, %f, %f, " VertexBuffer[i].Nrm.x VertexBuffer[i].Nrm.y VertexBuffer[i].Nrm.z
			format "%f, %f" VertexBuffer[i].Tex1.x VertexBuffer[i].Tex1.y
			format "}"
			
			if i != VertexBuffer.count then
			(
				format ",\n"
			)
			else
			(
				format "\n"
			)
		)
		
		format "};\n"
	),

	function PrintIndexBufferTestDX =
	(
		format "unsigned short l_Idxs[%] = {" IndexBuffer.count
		
		for i = 1 to IndexBuffer.count do
		(
			format "%"  (IndexBuffer[i] - 1)
			
			if i != IndexBuffer.count then
			(
				format ", "
			)
		)
		
		format "};\n"
	),
	
	function PrintMeshBuffer opt =
	(
		format "Material ID: % \n"  MatID
		format "Vertex Type: % \n"  VertexType
	
		MeshMaterial.PrintMaterialTest()
	
		format "Index Buffer Size: %\n" IndexBuffer.count
		if opt == 1 then
		(
			--PrintIndexBufferTestXNA()
		)
		else
		(
			--PrintIndexBufferTestDX()
		)
		
		format "Vertex Buffer Size: %\n" VertexBuffer.count
		if opt == 1 then
		(
			--PrintVertexBufferTestXNA()
		)
		else
		(
			--PrintVertexBufferTestDX()
		)
	)
	------------------------------------------------------------
	------------------End Debug Functions ------------------
	------------------------------------------------------------
)

struct BoundingBox
(
	MinPos,
	MaxPos,
	
	function WriteToFile file =
	(
		--Write Min Position
		WriteFloat file MinPos.x
		WriteFloat file MinPos.y
		WriteFloat file MinPos.z
		
		--Write Max Position
		WriteFloat file MaxPos.x
		WriteFloat file MaxPos.y
		WriteFloat file MaxPos.z
	),
	
	function PrintBoundingBoxTest =
	(
		format "Bounding Box Max Pos: % \n" MaxPos
		format "Bounding Box Min Pos: % \n" MinPos
	)
)

struct BoundingSphere
(
	Center,
	Radius,

	function WriteToFile file =
	(
		--Write Center Position
		WriteFloat file Center.x
		WriteFloat file Center.y
		WriteFloat file Center.z
		
		--Write Radius
		WriteFloat file Radius
	),
	
	function PrintBoundingSphereTest =
	(
		format "Bounding Sphere Center: % \n" Center
		format "Bounding Sphere Radius: % \n" Radius
	)
)

struct MeshObj
(
	MeshName,
	MeshBuffers,
	BBox,
	BSphere,
	BocMeshAttributes,
	MeshPath,
	MeshFileName,
	YawAngle,
	PitchAngle,
	RollAngle,
	Translation,
	IsInstance,
	InstanceName,
	ParentInstance,
	RelativeMeshFileName,
	RenderExport,
	RenderVisible,
	RenderLayer,
	AnimModelCore,
	PhysicsType,
	VtxNormalBuffer,
	TextureSplatting,
	CompletePath,
	RelativePath,
	UsePXObj,
	PXObjName,
	UsePXObjPos,
	PhysicsGroup,
	MeshScale,
	
	function CalcVtxNormals obj =
	(
		------------------------------------------------
		
		--Make sure map is clear Map
		VtxNormalBuffer = #()
		
		------------------------------------------------
		
		local NumVtxs=getNumVerts obj

		--Initialize Map for future use
		for b=1 to NumVtxs do
		(
			VtxNormalBuffer[b] = #()
			
			--From 1 to Max Smoothing groups (32)
			for t=1 to 32 do
			(
				VtxNormalBuffer[b][t] = point3 0 0 0
			)
		)

		local NumFaces = getNumFaces obj
		local InvTransform = inverse obj.transform
		
		for IdFace = 1 to NumFaces do
		(
			local IdxsFace = getFace obj IdFace
			
			local Vtx1 = (getVert obj IdxsFace.x) * InvTransform
			local Vtx2 = (getVert obj IdxsFace.y) * InvTransform
			local Vtx3 = (getVert obj IdxsFace.z) * InvTransform
			
			local FaceNormal = getFaceNormal obj IdFace
			local SmoothValue = getFaceSmoothGroup obj IdFace
			
			for b=1 to 32 do
			(
				if((IsSmoothingGroupEnabled b SmoothValue)==true) then
				(
					VtxNormalBuffer[IdxsFace.x][b] = VtxNormalBuffer[IdxsFace.x][b] + FaceNormal
					VtxNormalBuffer[IdxsFace.y][b] = VtxNormalBuffer[IdxsFace.y][b] + FaceNormal
					VtxNormalBuffer[IdxsFace.z][b] = VtxNormalBuffer[IdxsFace.z][b] + FaceNormal
				)
			)
		)
	),
	
	function GetVertexNormal IdVertex SmoothValue FaceNormal obj =
	(
		local HasNormal = false
		local Normal = point3 0 0 0
		
		--From 1 to Max Smoothing Group (32)
		for b=1 to 32 do
		(
			if((IsSmoothingGroupEnabled b SmoothValue) == true) then
			(
				Normal = Normal + VtxNormalBuffer[IdVertex][b]
				HasNormal = true
			)
		)
		
		if HasNormal == false then
		(
			local inverseTransform = Inverse(Obj.transform)
			Normal = FaceNormal * inverseTransform
		)
		
		return Normal
	),
	
	function GetMeshBuffer MatID =
	(
		for i = 1 to MeshBuffers.count do
		(
			if MeshBuffers[i].MatID == MatID then
			(
				return MeshBuffers[i]
			)
		)
		
		return undefined
	),
	
	function BuildBoundingBox obj =
	(
		BBox = CreateBoundingBox()
		
		BBox.MinPos = ( ResetVertexToOriginal obj (obj.min) )
		BBox.MaxPos = ( ResetVertexToOriginal obj (obj.max ) )
	),

	function BuildBoundingSphere obj =
	(
		BSphere = CreateBoundingSphere()
		
		local center = point3 0 0 0
		local delta = point3 0 0 0
		local radius = 0
		local maxpos = ( ResetVertexToOriginal obj ( obj.max ) )
		
		center = ( ResetVertexToOriginal obj ( obj.center ) )
		delta = ( (maxpos - center) as point3)
		
		radius = ( sqrt(  ( (pow delta.x 2) + (pow delta.y 2) + (pow delta.z 2) ) ) )
		
		BSphere.Center = center
		BSphere.Radius = radius
	),

	function SetFileNameMesh Filepath RelativeFilePath =
	(		
		local FileName = MeshName
		
		if BocMeshAttributes != undefined then
		(		
			if BocMeshAttributes.UseObjName != true then
			(
				FileName = BocMeshAttributes.UserFileName
			)
		)
		
		--Set Mesh Path
		MeshPath = (FilePath + MESH_SUBDIR)
		
		--Set Mesh Name
		MeshFileName = (FileName + MESH_EXTENSION)
		
		--Set Relative FileName
		RelativeMeshFileName = (RelativeFilePath + MESH_SUBDIR + MeshFileName)
	),
	
	function RemoveEmptyBuffers =
	(
		if IsInstance == true then
		(
			format "% is an instance, no need to remove empty buffers\n" MeshName
		)
		else
		(
			local cont = true
			
			while cont == true do
			(
				cont = false
				
				for i = 1 to MeshBuffers.count do
				(
					if ( (MeshBuffers[i].IndexBuffer.count == 0) OR  (MeshBuffers[i].VertexBuffer.count == 0) )  then
					(
						format "Mesh % at Sub Material % has IB count of % and VB count of % removing from array\n" MeshName MeshBuffers[i].MatID MeshBuffers[i].IndexBuffer.count MeshBuffers[i].VertexBuffer.count
						deleteItem  MeshBuffers i
						cont = true
						exit
					)
				)
			)
		)
	),
	
	function WriteToFile =
	(
		if IsInstance == true then
		(
			format "% is an instance, not writing to file\n" MeshName
		)
		else
		(
			--Make Sure directories exist
			makeDir MeshPath all:true
			
			format "Writing Model to: % \n" (MeshPath + MeshFileName)
			
			local file = fopen (MeshPath + MeshFileName) "wb"
			
			--Write Header to file
			WriteShort file 0xCACA #unsigned 
			
			--Write Mesh Name
			WriteStringToFile MeshName file
			
			--Write how many buffers we have
			WriteShort file MeshBuffers.count #unsigned
			
			for i = 1 to MeshBuffers.count do
			(
				MeshBuffers[i].WriteToFile TextureSplatting file
			)
			
			--Write Bounding Box
			BBox.WriteToFile file
			
			--Write Bounding Sphere
			BSphere.WriteToFile file
			
			--Write Footer to file
			WriteShort file 0xACAC #unsigned 
			
			fflush file
			fclose file
		)
	),
	
	function CopyTSTexturesToOutput = 
	(
		if TextureSplatting.TSEnable == true then
		(
			--Final output directory
			local outputDir = (CompletePath + TEXTURE_SUBDIR)

			--Make sure output dir exists
			makeDir outputDir all:true
			
			--Texture FileNames
			local destFile = ""
			
			if TextureSplatting.TSBlend1 != "" AND TextureSplatting.TSBlend1 != "C:\\"  then
			(
				destFile = (GetCompleteFileName TextureSplatting.TSBlend1 CompletePath)
				
				--Delete any file that exists
				deleteFile destFile
					
				--Copy file to new location
				copyFile TextureSplatting.TSBlend1 destFile
			)
			
			if TextureSplatting.TSBlend2 != "" AND TextureSplatting.TSBlend2 != "C:\\"  then
			(
				destFile = (GetCompleteFileName TextureSplatting.TSBlend2 CompletePath)
			
				--Delete any file that exists
				deleteFile destFile
					
				--Copy file to new location
				copyFile TextureSplatting.TSBlend2 destFile
			)
			
			if TextureSplatting.TSBlend3 != "" AND TextureSplatting.TSBlend3 != "C:\\"  then
			(
				destFile = (GetCompleteFileName TextureSplatting.TSBlend3 CompletePath)
			
				--Delete any file that exists
				deleteFile destFile
					
				--Copy file to new location
				copyFile TextureSplatting.TSBlend3 destFile
			)
			
			if TextureSplatting.TSBlend4 != "" AND TextureSplatting.TSBlend4 != "C:\\"  then
			(
				destFile = (GetCompleteFileName TextureSplatting.TSBlend4 CompletePath)
			
				--Delete any file that exists
				deleteFile destFile
					
				--Copy file to new location
				copyFile TextureSplatting.TSBlend4 destFile
			)
		)
	),
	
	function CopyTexturesToOutput =
	(
		for i = 1 to MeshBuffers.count do
		(
			MeshBuffers[i].CopyTexturesToOutput()
		)
		
		CopyTSTexturesToOutput()
	),
	
	function SetCompleteAndRelativePathToMaterials pCompletePath pRelativePath =
	(
		CompletePath = pCompletePath
		RelativePath = pRelativePath
		
		for i = 1 to MeshBuffers.count do
		(
			MeshBuffers[i].SetCompleteAndRelativePath CompletePath RelativePath
		)
	),
	
	------------------------------------------------------------
	-------------------- Debug Functions -------------------- 
	------------------------------------------------------------
	function PrintMeshObjTest opt =
	(
		format "#########################Start Object#########################\n"
		format "Object Name: % \n"  MeshName
		
		if IsInstance == false then
		(
			format "Parent Instance: % \n" ParentInstance
			
			format "Num Materials: % \n" MeshBuffers.count
			
			format "Has Boc Attributes: % \n" (BocMeshAttributes != undefined)
			
			if BocMeshAttributes != undefined then
			(
				format "Boc Attributes: Visible In Game: % \n" BocMeshAttributes.VisibleInGame
				format "Boc Attributes: Use Object Name: % \n" BocMeshAttributes.UseObjName
				format "Boc Attributes: User File Name: % \n" BocMeshAttributes.UserFileName
			)
			
			format "User Property RenderExport: % \n" RenderExport
			format "User Property RenderVisible: % \n" RenderVisible
			format "User Property AnimModelCore: % \n" AnimModelCore
			
			format "PhysX Obj Use: %\n" UsePXObj
			
			if(UsePXObj == true)then
			(
				format "PhysX Obj Name: %\n" PXObjName
				
				format "Use PhysX Obj Pos: %\m" UsePXObjPos
			)
			
			format "Mesh Target Directory: % \n" MeshPath
			format "Mesh Relative Filename: % \n" RelativeMeshFileName
			format "Mesh output Filename: % \n"  MeshFileName
			
			format "Mesh Texture Splatting: % \n" TextureSplatting.TSEnable
			
			if TextureSplatting.TSEnable == true then
			(
				format "Texture Splatting Textures:\n\tBlend 1: %\n\tBlend 2: %\n\tBlend 3: %\n\tBlend 4: %\n" TextureSplatting.TSBlend1 TextureSplatting.TSBlend2 TextureSplatting.TSBlend3 TextureSplatting.TSBlend4
			)
			
			BBox.PrintBoundingBoxTest
			BSpherePrintBoundingSphereTest
			
			for i = 1 to MeshBuffers.count do
			(
				format "#########################Start Material#########################\n"
				
				MeshBuffers[i].PrintMeshBuffer opt
				
				format "#########################End Material#########################\n"
			)
		)
		else
		(
			format "Mesh is Instance\n"
			format "Mesh is instance of: % \n" InstanceName
		)
		
		format "Yaw: % \n" YawAngle
		format "Pitch % \n" PitchAngle
		format "Roll % \n" RollAngle
		format "Translation % \n" Translation
		format "Scale % \n" MeshScale
		
		format "#########################End Object#########################\n"
	)
	------------------------------------------------------------
	------------------End Debug Functions ------------------
	------------------------------------------------------------
)

struct SceneObj
(
	MeshObjs,
	OutPutDir,
	FilePath,
	SubLevelPath,
	RelativeFilePath,
	
	function SetFilePaths =
	(
		FilePath = (OutPutDir + SubLevelPath)
		makeDir FilePath
		
		RelativeFilePath = (".\\" + SubLevelPath)
	),
	
	function AddMeshObj meshObject =
	(
		append MeshObjs meshObject
	),
	
	function WriteToFile =
	(	
		for i = 1 to MeshObjs.count do
		(
			MeshObjs[i].WriteToFile()
		)
	),
	
	function CreateStaticMeshXML =
	(
		local stFile = FilePath + "\\static_meshes.xml"
		
		local file = createfile stFile

		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<static_meshes>\n" to:file
		
		for i = 1 to MeshObjs.count do
		(
			if MeshObjs[i].IsInstance == false then
			(
				format "\t<static_mesh name=\"%\" filename=\"%\" />\n" MeshObjs[i].MeshName ("." + PARENT_SUBDIR + MeshObjs[i].RelativeMeshFileName) to:file
			)
		)
		
		format "</static_meshes>\n" to:file
		
		flush file
		close file
		free file
	),
	
	function CreateRenderObjectXML =
	(
		local stFile = FilePath + "\\renderable_objects.xml"
		local file = createfile stFile

		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<renderable_objects>\n" to:file
		
		format "\t<layer name=\"solid\" default=\"true\" />\n" to:file
		format "\t<layer name=\"alpha_objects\" />\n" to:file
		format "\t<layer name=\"alpha_blend_objects\" />\n" to:file
		format "\t<layer name=\"particles\" />\n" to:file
		format "\t<layer name=\"vegetation\" />\n" to:file
		format "\t<layer name=\"water\" />\n" to:file
		format "\t<layer name=\"fish\" />\n" to:file 
		format "\t<layer name=\"birds\" />\n" to:file
		format "\t<layer name=\"terrain\" />\n" to:file
		
		for i = 1 to MeshObjs.count do
		(	
			if ( (MeshObjs[i].RenderExport != undefined) ) then
			(
				local pos =  stringstream ""
				format "% % %" MeshObjs[i].Translation.x MeshObjs[i].Translation.y MeshObjs[i].Translation.z to:pos
				
				local coreName = MeshObjs[i].InstanceName
				local nodeName = "mesh_instance_hw"
				
				if ( MeshObjs[i].IsInstance == false ) then
				(
					coreName = MeshObjs[i].MeshName
					
					if MeshObjs[i].ParentInstance == false then
					(
						nodeName = "mesh_instance"
					)
				)
				
				if ( MeshObjs[i].RenderExport == "static_mesh" ) then
				(
					format "\t<% name=\"%\" core=\"%\" position=\"%\" yaw=\"%\" pitch=\"%\" roll=\"%\" scale=\"% % %\" " nodeName MeshObjs[i].MeshName coreName (pos as string) MeshObjs[i].YawAngle MeshObjs[i].PitchAngle MeshObjs[i].RollAngle MeshObjs[i].MeshScale.x MeshObjs[i].MeshScale.y MeshObjs[i].MeshScale.z to:file
					format "visible=\"%\" " MeshObjs[i].RenderVisible to:file
					
					if MeshObjs[i].RenderLayer != "undefined" then
					(
						format "layer=\"%\" " MeshObjs[i].RenderLayer to:file
					)
					
					if MeshObjs[i].PhysicsType == "undefined" then
					(
						format "create_physics=\"%\" " false to:file
					)
					else
					(
												
						format "create_physics=\"%\" physics_type=\"%\" physics_group=\"%\" " true MeshObjs[i].PhysicsType MeshObjs[i].PhysicsGroup to:file
						
						if MeshObjs[i].UsePXObj == true then
						(
							format "use_physx_obj=\"%\" physx_obj_name=\"%\" use_physx_obj_pos=\"%\" " true MeshObjs[i].PXObjName MeshObjs[i].UsePXObjPos to:file
						)
					)
					
					format "/>\n" to:file
				)
				else
				(
					format "\t<animated_model name=\"%\" core=\"%\" position=\"%\" yaw=\"%\" pitch=\"%\" roll=\"%\" />\n" MeshObjs[i].MeshName MeshObjs[i].AnimCoreModel (pos as string) MeshObjs[i].YawAngle MeshObjs[i].PitchAngle MeshObjs[i].RollAngle to:file
				)
			)
		)
		
		format "</renderable_objects>\n" to:file
		
		flush file
		close file
		free file
	),
	
	function CopyTexturesToOutput = 
	(
		for i = 1 to MeshObjs.count do
		(
			MeshObjs[i].CopyTexturesToOutput()
		)
	),
	
	function PrintSceneObjectsTest opt =
	(
		format "############ Starting Print of Scene Objects ###############\n"
		
		format "Objects in Scene: % \n" MeshObjs.count
		
		for i = 1 to MeshObjs.count do
		(
			MeshObjs[i].PrintMeshObjTest opt
		)
		
		format "############ End Print of Scene Objects ###############\n"
	)
)

/**************************************
    Functions
**************************************/
function CreateMaterialObjectStruct = 
(
	local matObj = MaterialObject CompletePath:"" RelativePath:"" Texture1:"" Texture2:"" Bump:"" RNMTextureX:"" RNMTextureY:"" RNMTextureZ:""
	
	return matObj
)

function CreateVertex pos nrm tex tex2 diffCol =
(
	local vtx = Vertex Pos:0 Nrm:0 Tex1:(point3 0 0 0) Tex2:(point3 0 0 0) Diff:0
	
	--Add Vertex
	vtx.Pos = (pos as point3)
	
	--Add Normal
	vtx.Nrm = (nrm as point3)
	
	--Add Texture Coord 1
	vtx.Tex1 = (tex as point3)
	
	--Add Texture Coord 2
	vtx.Tex2 = (tex2 as point3)
	
	--Add Diffuse Color
	vtx.diff = (diffCol as integer) 
	
	return vtx
)

function CreateTextureSplattingObj = 
(
	local tsObj = TextureSplattingObj TSEnable:false TSBlend1:"" TSBlend2:"" TSBlend3:"" TSBlend4:""
	
	return tsObj
)

function CreateMeshStruct =
(
	local meshObject = MeshObj  MeshPath:"" MeshName:"" MeshBuffers:#() BBox:0 BSphere:0 BocMeshAttributes:0 MeshFileName:"" YawAngle:0 PitchAngle:0 RollAngle:0 Translation:0 IsInstance:False InstanceName:"" ParentInstance:false RelativeMeshFileName:"" RenderExport:"" RenderVisible:"" RenderLayer:"" AnimModelCore:"" PhysicsType:"" VtxNormalBuffer:#() TextureSplatting:"" CompletePath:"" RelativePath:"" UsePXObj:false PXObjName:"" UsePXObjPos:false PhysicsGroup:"" MeshScale:(point3 0 0 0)
	
	meshObject.TextureSplatting = CreateTextureSplattingObj()
	
	return meshObject
)

function CreateBoundingBox =
(
	local BBox = BoundingBox MinPos:(point3 0 0 0) MaxPos:(point3 0 0 0)
	
	return BBox
)

function CreateBoundingSphere =
(
	local BSphere = BoundingSphere Center:(point3 0 0 0) Radius:0
	
	return BSphere
)

function CreateMeshBuffer =
(
	local meshBuffer = VIBuffers  MatID:0 VertexType:0 VertexBuffer:#() VertexMap:0 IndexBuffer:#() MeshMaterial:0
	
	meshBuffer.VertexMap = dotnetobject "System.Collections.Generic.Dictionary`2[System.String, System.Int16]"
	
	return meshBuffer
)

function CreateSceneObject =
(
	local l_SceneObjs = SceneObj MeshObjs:#() OutPutDir:"" FilePath:"" SubLevelPath:"" RelativeFilePath:""
	
	return l_SceneObjs
)
