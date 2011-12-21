/*******************************************************

Name: MeshObject.ms

Use: Structs to store the Mesh Object information
		- Material

Last Update: 12/15/2011

********************************************************/

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

global PARENT_SUBDIR		= "\\Data\\"
global MESH_SUBDIR		= "\\Models\\";
global TEXTURE_SUBDIR	= "\\Textures\\";
global MESH_EXTENSION	= ".m3d"

/**************************************
    Definitions
**************************************/
struct MaterialObject
(
	CompletePath,
	RelativePath,
	Texture1,
	Texture2,
	Bump,
	
	function GetRelativeFileName filename =
	(
		local fileString = (filenameFromPath filename)
		
		fileString = (RelativePath + TEXTURE_SUBDIR + fileString)
	),
	
	function GetCompleteFileName filename =
	(
		local fileString = (filenameFromPath filename)
		
		fileString = (CompletePath + TEXTURE_SUBDIR + fileString)
	),
	
	function WriteToFile vertexType file =
	(
		local finalName = ""
		
		if (bit.and VertexType VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 then
		(
			finalName = (GetRelativeFileName Texture1)
			WriteStringToFile finalName file
		
			if (bit.and VertexType VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 then
			(
				finalName = (GetRelativeFileName Texture2)
				WriteStringToFile finalName file
			)
		
			if (bit.and VertexType VERTEX_TYPE_TANGENT) == VERTEX_TYPE_TANGENT then
			(
				finalName = (GetRelativeFileName Bump)
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
			destFile = (GetCompleteFileName Texture1)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile Texture1 destFile
		)
		
		--Make sure texture2 is set
		if Texture2 != "" then
		(
			destFile = (GetCompleteFileName Texture2)
		
			--Delete any file that exists
			deleteFile destFile
				
			--Copy file to new location
			copyFile Texture2 destFile
		)
		
		--Make sure bump is set
		if Bump != "" then
		(
			destFile = (GetCompleteFileName Bump)
		
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
			format "Relative Texture 1: % \n" (GetRelativeFileName Texture1)
			format "Target Texture 1: % \n" (GetCompleteFileName Texture1)
		)
		else
		(
			format "Texture 1 is not set\n"
		)
		
		if Texture2 != "" then
		(
			format "Original Texture 2: % \n" Texture2
			format "Relative Texture 2: % \n" (GetRelativeFileName Texture2)
			format "Target Texture 2: % \n" (GetCompleteFileName Texture2)
		)
		else
		(
			format "Texture 2 is not set\n"
		)
		
		if Bump != "" then
		(
			format "Original Bump: % \n" Bump
			format "Relative Bump: % \n" (GetRelativeFileName Bump)
			format "Target Bump: % \n" (GetCompleteFileName Bump)
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

	function WriteToFile vertexType file =
	(
		-- Write Geometry
		WriteFloat file Pos.x
		WriteFloat file Pos.y
		WriteFloat file Pos.z
		
		--Normals
		WriteFloat file Nrm.x
		WriteFloat file Nrm.y
		WriteFloat file Nrm.z
		
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
		
		if (bit.and VertexType VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 then
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
		
		if objMaterial.diffuseMapEnable == true AND classOf(objMaterial.diffuseMap) == BitmapTexture then
		(
			MeshMaterial = CreateMaterialObjectStruct()
			
			VertexType = (bit.or VertexType VERTEX_TYPE_TEXTURE1)
			MeshMaterial.Texture1 = (openBitMap objMaterial.diffuseMap.bitmap.filename).fileName
		
			if objMaterial.selfIllumMapEnable == true AND classOf(objMaterial.selfIllumMap) == BitmapTexture then
			(		
				VertexType = (bit.or VertexType VERTEX_TYPE_TEXTURE2)
				MeshMaterial.Texture2 = (openBitMap objMaterial.selfIllumMap.bitmap.filename).fileName
			)
			
			if objMaterial.bumpMapEnable == true AND classOf(objMaterial.bumpMap) == BitmapTexture then
			(
				VertexType = (bit.or VertexType (bit.or VERTEX_TYPE_BINORMAL VERTEX_TYPE_TANGENT) )
				MeshMaterial.Bump = (openBitMap objMaterial.bumpMap.bitmap.filename).fileName
			)
		)
		else
		(
			VertexType = (bit.or VertexType VERTEX_TYPE_DIFFUSE)
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
		WriteShort file IndexBuffer.count #unsigned
		
		-- Write the index buffer to file
		for i = 1 to IndexBuffer.count do
		(
			--Set indexes to start from 0 and write to file
			WriteShort file (IndexBuffer[i] - 1) #unsigned
		)
	),

	function WriteVertexBufferToFile file =
	(
		--Write how many vertexs we have
		WriteShort file VertexBuffer.count #unsigned
		
		--Write the vertex buffer to file
		for i = 1 to VertexBuffer.count do
		(
			VertexBuffer[i].WriteToFile VertexType file
		)
	),

	function WriteToFile file = 
	(
		--Write Vertex Type to file
		WriteShort file VertexType
		
		--Write Materials to File
		MeshMaterial.WriteToFile VertexType file
		
		--Write Index Buffer to File
		WriteIndexBufferToFile file
		
		--Write Vertex Buffer to File
		WriteVertexBufferToFile file
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
			PrintIndexBufferTestXNA()
		)
		else
		(
			PrintIndexBufferTestDX()
		)
		
		format "Vertex Buffer Size: %\n" VertexBuffer.count
		if opt == 1 then
		(
			PrintVertexBufferTestXNA()
		)
		else
		(
			PrintVertexBufferTestDX()
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
	RelativeMeshFileName,
	
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
		
		BBox.MinPos = ( RHTranslationToLH (obj.min) )
		BBox.MaxPos = ( RHTranslationToLH (obj.max ) )
	),

	function BuildBoundingSphere obj =
	(
		BSphere = CreateBoundingSphere()
		
		local center = point3 0 0 0
		local delta = point3 0 0 0
		local radius = 0
		local maxpos = ( RHTranslationToLH ( obj.max ) )
		
		center = ( RHTranslationToLH ( obj.center ) )
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
		RelativeMeshFileName = (RelativeFilePath + MeshFileName)
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
				MeshBuffers[i].WriteToFile file
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
	
	function CopyTexturesToOutput =
	(
		for i = 1 to MeshBuffers.count do
		(
			MeshBuffers[i].CopyTexturesToOutput()
		)
	),
	
	function SetCompleteAndRelativePathToMaterials CompletePath RelativePath =
	(
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
			format "Num Materials: % \n" MeshBuffers.count
			
			format "Has Boc Attributes: % \n" (BocMeshAttributes != undefined)
			
			if BocMeshAttributes != undefined then
			(
				format "Boc Attributes: Visible In Game: % \n" BocMeshAttributes.VisibleInGame
				format "Boc Attributes: Use Object Name: % \n" BocMeshAttributes.UseObjName
				format "Boc Attributes: User File Name: % \n" BocMeshAttributes.UserFileName
			)
			
			format "Mesh Target Directory: % \n" MeshPath
			format "Mesh Relative Filename: % \n" RelativeMeshFileName
			format "Mesh output Filename: % \n"  MeshFileName
			
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
		
		close file
	),
	
	function CreateRenderObjectXML =
	(
		local stFile = FilePath + "\\renderable_objects.xml"
		
		local file = createfile stFile

		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<renderable_objects>\n" to:file
		
		for i = 1 to MeshObjs.count do
		(
			local pos =  stringstream ""
			format "% % %" MeshObjs[i].Translation.x MeshObjs[i].Translation.y MeshObjs[i].Translation.z to:pos
			
			local coreName = MeshObjs[i].InstanceName
			
			if MeshObjs[i].IsInstance == false then
			(
				coreName = MeshObjs[i].MeshName
			)
		
			format "\t<mesh_instance name=\"%\" core=\"%\" position=\"%\" yaw=\"%\" pitch=\"%\" roll=\"%\" />\n" MeshObjs[i].MeshName coreName (pos as string) MeshObjs[i].YawAngle MeshObjs[i].PitchAngle MeshObjs[i].RollAngle to:file
		)
		
		format "</renderable_objects>\n" to:file
		
		close file
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
	local matObj = MaterialObject CompletePath:"" RelativePath:"" Texture1:"" Texture2:"" Bump:""
	
	return matObj
)

function CreateVertex pos nrm tex tex2 diffCol VertexType =
(
	local vtx = Vertex Pos:0 Nrm:0 Tex1:0 Tex2:0 Diff:0
	
	--Add Vertex
	vtx.Pos = (pos as point3)
	
	--Add Normal
	vtx.Nrm = (nrm as point3)
	
	if (bit.and VertexType VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 then
	(
		vtx.Tex1 = (tex as point3)
		
		if (bit.and VertexType VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 then
		(
			vtx.Tex2 = (tex2 as point3)
		)
	)
	else
	(
		vtx.diff = (diffCol as integer) 
	)
	
	return vtx
)

function CreateMeshStruct =
(
	local meshObject = MeshObj  MeshPath:"" MeshName:"" MeshBuffers:#() BBox:0 BSphere:0 BocMeshAttributes:0 MeshFileName:"" YawAngle:0 PitchAngle:0 RollAngle:0 Translation:0 IsInstance:False InstanceName:"" RelativeMeshFileName:""

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
