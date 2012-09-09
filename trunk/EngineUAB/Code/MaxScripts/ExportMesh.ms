
--Includes------------------------------------
fileIn "HelperFunctions.ms"
fileIn "MeshObject.ms"
------------------------------------------------

-----------------------------------------------------------------
-- Purpose: Extract Material Properties from an Editable Mesh object to a "MeshObj" Struct
--
-- Parameters
--		(1) obj: Editable Mesh from where to get the properties
--		(2) t_mesh: "MeshObj" Struct where to save the information to (Must have been initialize previously with CreateMeshStruct Function)
--
-- Returns
--		 1 : Material information extracted and assigned to t_mesh
--		-1 : obj is not an "editable_mesh" object
--		-2 : t_mesh is not a "MeshObj" object
--		-3 : t_mesh does not have a material assigned to it
--		-4 : t_mesh has a material that is not supported
--		-5 : t_mesh has a material that has an undefined item assigned to it
--
-----------------------------------------------------------------
function ExtractMaterialProperties obj t_mesh =
(
	-----------------------------------------------------------------
	
	--Verify that "obj" is an editable Mesh
	if ( classOf(obj) ) != Editable_mesh then
	(
		return -1
	)
	
	-----------------------------------------------------------------
	
	--Verify that "t_mesh" is a MeshObj 
	if ( classOf(t_mesh) ) != MeshObj  then
	(
		return -2
	)
	
	-----------------------------------------------------------------
	
	format "Extracting Material Properties from object % \n" t_mesh.MeshName
	
	-----------------------------------------------------------------
	
	--Verify that a material exists on the object
	if obj.material != null then
	(
		local matType = classof(obj.material) 	
	
		if matType == Multimaterial then
		(			
			local matIDs = (obj.material.materialIDList as array)
			
			for i = 1 to matIDs.count do
			(
				local id = matIDs[i]
				
				if ( (obj.material[id] != undefined) AND ( classOf(obj.material[id]) ) == Standardmaterial ) then
				(					
					local meshBuffer = CreateMeshBuffer()
					
					meshBuffer.MatID = id
					
					--Get Vertex Type of the mesh and Materials
					meshBuffer.SetVertexTypeAndMaterial obj.material[id]
					
					format "Adding Material ID: % \n" meshBuffer.MatID
					
					append t_mesh.MeshBuffers meshBuffer
				)
				else if obj.material[id] == undefined then
				(
					format "Undefined Material % count %\n" id matIDs.count
					-- Undefined Material 
					continue
				)
				else
				(
					-- Unknown Material 
					return -4
				)
			)
		)
		else if matType == Standardmaterial then 
		(	
			local meshBuffer = CreateMeshBuffer()
			
			meshBuffer.MatID = 1
			
			--Get Vertex Type of the mesh and Materials
			meshBuffer.SetVertexTypeAndMaterial obj.material
			
			format "Adding Material ID: % \n" meshBuffer.MatID
			
			append t_mesh.MeshBuffers meshBuffer
		)
		else
		(
			--Unknown Material
			return -4
		)
	)
	else
	(
		return -3
	)
	
	-----------------------------------------------------------------
	
	format "Finish extracting Material Properties from object % \n" t_mesh.MeshName
	
	-----------------------------------------------------------------
	
	return 1
)

-----------------------------------------------------------------
-- Purpose: Extract the Geometry/Normals Properties from an Editable Mesh object to a "MeshObj" Struct
--
-- Parameters
--		(1) obj: Editable Mesh from where to get the properties
--		(2) counterClockwise: TRUE is object is going to be exported in a counter clockwise fashion, false otherwise
--		(3) t_mesh: "MeshObj" Struct where to save the information to (Must have been initialize previously with CreateMeshStruct Function)
--
-- Returns
--		 1 : Geometry information extracted and assigned to t_mesh
--		-1 : obj is not an "editable_mesh" object
--		-2 : t_mesh is not a "MeshObj" object
--		-3 : obj has a face with a material that is not present in t_mesh material collection
--
-----------------------------------------------------------------
function ExtractMeshGeometry obj counterClockwise t_mesh =
(
	-----------------------------------------------------------------
	
	--Verify that "obj" is an editable Mesh
	if classOf(obj) != Editable_mesh then
	(
		return -1
	)
	
	-----------------------------------------------------------------
	
	--Verify that "t_mesh" is a MeshObj 
	if ( classOf(t_mesh) ) != MeshObj  then
	(
		return -2
	)
	
	-----------------------------------------------------------------
	
	format "Extracting Geometry from object % \n" t_mesh.MeshName

	-----------------------------------------------------------------

	t_mesh.YawAngle = GetYaw obj.transform
	t_mesh.PitchAngle = GetPitch obj.transform
	t_mesh.RollAngle = GetRoll obj.transform

	-----------------------------------------------------------------

	t_mesh.Translation = RHTranslationToLH obj.transform.translation

	-----------------------------------------------------------------

	--Extract Geometry information if t_mesh is not an instance
	if t_mesh.IsInstance == false then
	(
		-----------------------------------------------------------------
		
		--Create VtxNormalMap
		t_mesh.CalcVtxNormals obj
		
		-----------------------------------------------------------------
		
		local numFaces = getNumFaces obj

		for i=1 to numFaces do
		(
			local MeshBuffer = undefined
			
			--Get Mesh Buffer
			if t_mesh.MeshBuffers.Count > 1 then
			(
				--Get Material ID assign to the face
				local MatID = (getFaceMatID obj i)
				
				MeshBuffer = (t_mesh.GetMeshBuffer MatID)
				
				if MeshBuffer == undefined then
				(
					Format "MatID MeshBuffer not found % \n" MatID
					
					for x = 1 to t_mesh.MeshBuffers.count do
					(
						Format "Available MatID Mesh Buffers: % \n" t_mesh.MeshBuffers[x].MatID
					)
					
					continue
				)
			)
			else
			(
				MeshBuffer = t_mesh.MeshBuffers[1]
			)
			
			local IdxsFace = getFace obj i
			
			local Vtx1 = getVert obj IdxsFace.x
			local Vtx2 = getVert obj IdxsFace.y
			local Vtx3 = getVert obj IdxsFace.z
			
			
			local Normal1 = getNormal obj IdxsFace.x
			local Normal2 = getNormal obj IdxsFace.y
			local Normal3 = getNormal obj IdxsFace.z
						
			local FaceNormal=getFaceNormal obj i
			local SmoothValue=getFaceSmoothGroup obj i
			local Normal1 = t_mesh.GetVertexNormal IdxsFace.x SmoothValue FaceNormal obj
			local Normal2 = t_mesh.GetVertexNormal IdxsFace.y SmoothValue FaceNormal obj
			local Normal3 = t_mesh.GetVertexNormal IdxsFace.z SmoothValue FaceNormal obj
			Normal1 = RHTranslationToLH Normal1
			Normal2 = RHTranslationToLH Normal2
			Normal3 = RHTranslationToLH Normal3
						
			local Diffuse1 = 0
			local Diffuse2 = 0
			local Diffuse3 = 0
			
			local TUVMap1 = point3 0 0 0
			local TUVMap2 = point3 0 0 0
			local TUVMap3 = point3 0 0 0
			
			local TUV2Map1 = point3 0 0 0
			local TUV2Map2 = point3 0 0 0
			local TUV2Map3 = point3 0 0 0
			
			if (bit.and MeshBuffer.VertexType VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 then
			(
				local IdxsMap = meshop.getMapFace obj 1 i
				
				TUVMap1 = meshop.getMapVert obj 1 (IdxsMap.x as integer)
				TUVMap1.y = 1 - TUVMap1.y
				TUVMap2 = meshop.getMapVert obj 1 (IdxsMap.y as integer)
				TUVMap2.y = 1 - TUVMap2.y
				TUVMap3 = meshop.getMapVert obj 1 (IdxsMap.z as integer)
				TUVMap3.y = 1 - TUVMap3.y
			
				if ( (bit.and MeshBuffer.VertexType VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 ) OR
				   ( (bit.and MeshBuffer.VertexType VERTEX_TYPE_RNM) == VERTEX_TYPE_RNM ) OR
				   ( t_mesh.TextureSplatting.TSEnable == true ) then
				(
					IdxsMap = meshop.getMapFace obj 2 i
					
					TUV2Map1 = meshop.getMapVert obj 2 (IdxsMap.x as integer)
					TUV2Map1.y = 1 - TUV2Map1.y
					TUV2Map2 = meshop.getMapVert obj 2 (IdxsMap.y as integer)
					TUV2Map2.y = 1 - TUV2Map2.y
					TUV2Map3 = meshop.getMapVert obj 2 (IdxsMap.z as integer)
					TUV2Map3.y = 1 - TUV2Map3.y					
				)
			)
			else
			(
				Diffuse1 = bit.shift obj.material.diffuse.r 24 + bit.shift obj.material.diffuse.g 16 + bit.shift obj.material.diffuse.b 8 + obj.material.diffuse.a
				Diffuse2 = bit.shift obj.material.diffuse.r 24 + bit.shift obj.material.diffuse.g 16 + bit.shift obj.material.diffuse.b 8 + obj.material.diffuse.a
				Diffuse3 = bit.shift obj.material.diffuse.r 24 + bit.shift obj.material.diffuse.g 16 + bit.shift obj.material.diffuse.b 8 + obj.material.diffuse.a
			)
			
			--Set Coordenates to local and from RH to LH
			Vtx1 = ResetVertexToOriginal obj Vtx1
			Vtx2 = ResetVertexToOriginal obj Vtx2
			Vtx3 = ResetVertexToOriginal obj Vtx3
			--Vtx1 = RHTranslationToLH Vtx1
			--Vtx2 = RHTranslationToLH Vtx2
			--Vtx3 = RHTranslationToLH Vtx3
			
			--Create Vertexs
			local Vertex1 = CreateVertex Vtx1 Normal1 TUVMap1 TUV2Map1 Diffuse1
			local Vertex2 = CreateVertex Vtx2 Normal2 TUVMap2 TUV2Map2 Diffuse2 
			local Vertex3 = CreateVertex Vtx3 Normal3 TUVMap3 TUV2Map3 Diffuse3 
			
			--Add to Buffer
			MeshBuffer.AddVertexAndIndex Vertex1 Vertex2 Vertex3 counterClockwise
		)
	)
	
	-----------------------------------------------------------------
	
	format "Finish extracting Geometry from object % \n" t_mesh.MeshName

	-----------------------------------------------------------------
	
	return 1
)

-----------------------------------------------------------------
-- Purpose: Export the selected objects
--
-- Parameters
--		(1) selectedObjs: Selected objects to export
--		(2) outputDir: Output directory to save the files
--		(3) subOutPutDir: Sub Directory (Level) where the files are going to be
--		(4) counterClockwise: TRUE is object is going to be exported in a counter clockwise fashion, false otherwise
--		(5) extractMesh: TRUE if mesh properties want to be extracted from the objects and save to file, false otherwise
--		(6) copyTexture: TRUE if mesh material textures are going to be copy to output directories, false otherwise
--		(7) createST: TRUE to create static_mesh.xml
--		(8) displayDebugInfo: TRUE if debug information wants to be printed, false to not print
--
-- Returns
--		 1 : Objects information extracted correctly
--		-1 : No objects on selection to extract
--
-----------------------------------------------------------------
function ExportSelection selectedObjs outputDir subOutPutDir counterClockwise extractMesh copyTexture createST displayDebugInfo =
(
	local objsNum = 0
	local warningMeshes = ""
	local sceneObjects = CreateSceneObject()
	local copiedObjects = #()
	local processedObjects = #()
	
	if selectedObjs != null then
	(
		if classOf(selectedObjs) == ObjectSet then
		(
			objsNum = selectedObjs.count
		)
		else
		(
			objsNum = 1
		)
	)
	
	--If no objects on the scene to export
	if  objsNum == 0 then
	(
		messageBox "No objects to export" title:"ATTENTION"
		return -1
	)
	
	--Set File Output Directories
	sceneObjects.OutPutDir = outputDir
	sceneObjects.SubLevelPath = subOutPutDir
	sceneObjects.SetFilePaths()
	
	for objPM in selectedObjs do
	(
		local obj = (snapshot objPM)
		local classObj = ( classOf(obj) )
		local ret = 0
		
		--Make sure we have an Editable Mesh
		if classObj != Editable_Mesh then
		(
			convertToMesh obj
		)
		
		--Collapse the stack
		collapseStack obj
		
		--Append copied objs to be deleted at the end
		append copiedObjects obj
		
		--Create mesh object
		local t_mesh = CreateMeshStruct()
	
		--Name of the mesh
		t_mesh.MeshName = objPM.name
		
		--------------------------------------
		
		format "Starting export of object: % \n" t_mesh.MeshName 
		
		--------------------------------------
		
		t_mesh.TextureSplatting.TSEnable = getUserProp objPM "texture_splatting"
		
		if t_mesh.TextureSplatting.TSEnable == true then
		(
			t_mesh.TextureSplatting.TSBlend1 = getUserProp $ "texture_splatting_blend1" 
			t_mesh.TextureSplatting.TSBlend2 = getUserProp $ "texture_splatting_blend2" 
			t_mesh.TextureSplatting.TSBlend3 = getUserProp $ "texture_splatting_blend3" 
			t_mesh.TextureSplatting.TSBlend4 = getUserProp $ "texture_splatting_blend4" 
		)
		
		--------------------------------------
		
		local l_IsInstance = getUserProp objPM "is_instance"
		local l_InstanceName = getUserProp objPM "instance_name"
		
		if ( (l_IsInstance == true) AND (l_InstanceName != undefined) ) then
		(
			format "Verifying Instance of object. Object is instace \n"
			
			t_mesh.IsInstance = true
			t_mesh.InstanceName = l_InstanceName as string
		)
		else
		(
			--------------------------------------
			
			format "Verifying Instance of object. Current number in array % \n" processedObjects.count
			
			--------------------------------------
			
			--Verify if it is an instance
			for i = 1 to processedObjects.count do
			(
				if (areNodesInstances objPM processedObjects[i]) == true then
				(
					t_mesh.IsInstance = true
					t_mesh.InstanceName = processedObjects[i].Name
				)
			)
		)
	
		--Add Boc Attributes 
		t_mesh.BocMeshAttributes = obj.baseobject.custAttributes["BocAttribute"]
		
		--Add User Define Properties
		t_mesh.ParentInstance = getUserProp objPM "parent_instance"
		t_mesh.RenderExport = getUserProp objPM "render_export"
		t_mesh.RenderVisible = getUserProp objPM "render_visible"
		t_mesh.AnimModelCore = getUserProp objPM "anim_model_core"
		t_mesh.RenderLayer = getUserProp objPM "render_layer"
		t_mesh.PhysicsType = getUserProp objPM "physics_generate"
		
		--Set Filename
		t_mesh.SetFileNameMesh sceneObjects.FilePath sceneObjects.RelativeFilePath
		
		if t_mesh.IsInstance == true then
		(
			format "% is an instance, no need to extract materials\n" t_mesh.MeshName
		)
		else
		(
			--Extract Material Properties from object
			ret = ExtractMaterialProperties obj t_mesh
		
			if ret != 1 then 
			(
				if ret == -1 then
				(
					warningMeshes = warningMeshes + (obj.name + " is not an editable mesh. Object will not be exported.\n")
				)
				else if ret == -3 then
				(
					warningMeshes = warningMeshes + (obj.name + " has not materials assigned to it. Object will not be exported.\n")
				)
				else if ret == -4 then
				(
					warningMeshes = warningMeshes + (obj.name + " has a material not supported by this exporter (Supported Materials: Standard and Multi). Object will not be exported\n")
				)
				else if ret == -5 then
				(
					warningMeshes = warningMeshes + (obj.name + " has a material that is not initialize. Object will not be exported.\n")
				)
				else
				(
					warningMeshes = warningMeshes + (obj.name + " unexpected error in \"ExtractMaterialProperties\" ret: " + (ret as string) + "\n")
				)
				
				--Do not continue with this object and discard it
				continue
			)
		)
	
		--Set Relative Path to Materials
		t_mesh.SetCompleteAndRelativePathToMaterials sceneObjects.FilePath sceneObjects.RelativeFilePath
		
		if extractMesh == true then
		(
			--Extract Object information
			ret = (ExtractMeshGeometry obj counterClockwise t_mesh)
			
			if ret != 1 then
			(
				warningMeshes = warningMeshes + (obj.name + " unexpected error in \"ExtractMeshGeometry\" ret: " + (ret as string) + "\n")
				
				continue
			)

			--Create Bounding Spheres and Bounding Boxes
			t_mesh.BuildBoundingBox obj
			t_mesh.BuildBoundingSphere obj
			
			--Remove Emty Buffers
			t_mesh.RemoveEmptyBuffers()
		)
	
		--Append new Mesh to Scene Objects
		sceneObjects.AddMeshObj t_mesh
		
		if t_mesh.IsInstance == false then
		(
			--Append processed objects
			append processedObjects objPM
		)
	)
	
	for i = 1 to copiedObjects.count do
	(
		local delObj = copiedObjects[i]
		
		copiedObjects[i] = 0
		
		delete delObj
	)
	
	if displayDebugInfo == true then
	(
		format "Exporting Meshes: % \n" extractMesh
		format "Copying Textures: % \n" copyTexture
		sceneObjects.PrintSceneObjectsTest 1
	)
	
	if extractMesh == true then 
	(
		sceneObjects.WriteToFile()
		sceneObjects.CreateRenderObjectXML()
	)
	
	if copyTexture == true then 
	(
		sceneObjects.CopyTexturesToOutput()
	)
	
	if createST == true then
	(
		sceneObjects.CreateStaticMeshXML()
	)
	
	if  warningMeshes != "" then
	(
		messageBox warningMeshes title:"ATTENTION"
	)
	
	return 1
)

--Exports the entire scene to a file
function ExportScene outputDir subOutPutDir counterClockwise copyMesh copyTexture createST displayDebugInfo = 
(
	-- Select all objects on Scene
	select $*
	
	return (ExportSelection $ outputDir subOutPutDir counterClockwise copyMesh copyTexture createST displayDebugInfo)
)

--ExportScene "E:\\My Stuff\\UAB\\CaperucitaLoca\\Run\\Viewer\\Data\\" "Chapter4\\" true true true true true 
