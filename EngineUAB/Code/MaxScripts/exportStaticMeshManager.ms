fileIn "exportSimpleMesh.ms"

function ExportStaticMeshXML Path ComboBox =
(
	local file = createfile Path
	
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<static_meshes>\n" to:file
	
	for i = 1 to $*.count do
	(
		format "\t<static_mesh name=\"%\" filename=\"%\" />\n" $*[i].name ("./Data/" + ComboBox.items[ComboBox.selection]  +"/Static_Meshes/" +$*[i].name+".m3d") to:file
	)
	format "</static_meshes>\n" to:file
	close file
)

function ExportStaticMeshes Path Level =
(	
	for i = 1 to $*.count do
	(
		local out_path = (Path + $*[i].name + ".m3d")
		ExportMesh $*[i] out_path Level
	)
)

function ExportTexturesMesh Path =
(	
	for i = 1 to $*.count do
	(
		if classof($*[i]) == Editable_Mesh then
		(
			local Material = $*[i].Material
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
)