fileIn "exportSimpleMesh.ms"

function ExportStaticMeshXML Path =
(
	local file = createfile Path
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	
	for i = 1 to $selection.count do
	(
		format "\t<static_mesh name=\"%\" filename=\"%\" />\n" $selection[i].name ("./Data/Static_Meshes/"+$selection[i].name+".m3d") to:file
	)
	format "</static_meshes>\n" to:file
	close file
)

function ExportStaticMeshes Path =
(
	makeDir MeshesPath all:true
	
	for i = 1 to $selection.count do
	(
		local out_path = (Path + $selection[i].name + ".m3d")
		ExportMesh $selection[i] out_path
	)
)

function ExportTexturesMesh Path =
(
	makeDir TexturesPath all:true
	
	for i = 1 to $selection.count do
	(
		local Material = $selection[i].Material
		for j = 1 to Material.count do
		(
			local texture_path = (openBitMap Material[j].diffuseMap.bitmap.filename).fileName
			local texture_name = filenameFromPath texture_path
			local out_path = (Path + texture_name)
		
			deleteFile out_path
			copyFile texture_path out_path
		)
	)
)