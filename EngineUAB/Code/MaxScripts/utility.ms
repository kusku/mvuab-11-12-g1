fileIn "exportSimpleMesh.ms"
fileIn "exportStaticMeshManager.ms"

utility UABUtility "UABUtility" width:160 height:296
(
	local levels =#("General", "Level1", "Level2")
	
	button PathButton "Set Path" pos:[8,8] width:144 height:16
	checkbox ExportMeshesCheckBox "Export Meshes" pos:[8,104] width:104 height:16
	label PathLabel "C:\Users\Marc\Documents\UAB\Màster de Creació de Videojocs\EngineUAB\Run\Viewer\Data" pos:[9,32] width:141 height:64
	checkbox ExportTexturesCheckBox "Export Textures" pos:[8,120] width:96 height:16
	dropdownList LevelComboBox "" pos:[8,144] width:144 height:21 items:#("General", "Level1", "Level2")
	GroupBox ExportBox "Export" pos:[8,176] width:144 height:112
	button LightsButton "Lights" pos:[16,192] width:56 height:16
	button MeshButton "Mesh" pos:[88,192] width:56 height:16
	button CameraButton "Camera" pos:[16,216] width:56 height:16
	button StaticMeshButton "Static Mesh Manager" pos:[16,240] width:128 height:16
	button SceneButton "Scene" pos:[16,264] width:128 height:16

	function GetMeshesPathExport =
	(
		return PathLabel.text + "\\"
	)
	
	on PathButton pressed do
	(
		local data_path = getSavePath caption:"Data Path" initialDir:PathLabel.text
		if data_path != undefined then
		(
			PathLabel.text = data_path
		)
	)
	
	on MeshButton pressed do
	(
		local out_name=GetSaveFileName filename:(GetMeshesPathExport()+$.name+".m3d") caption:"Select output file to export file" types:" m3d(*.m3d)|*.m3d|All Files(*.*)|*.*|"
		if out_name != undefined then
		(
			ExportMesh $ out_name
			messageBox "Mesh exported correctly." title:"INFORMATION"
		)
	)
	
	on StaticMeshButton pressed do
	(
		local XMLPath = (GetMeshesPathExport() + "XML\\Static_Meshes\\")
		local MeshesPath = (GetMeshesPathExport() + "Static_Meshes\\")
		local TexturesPath = (GetMeshesPathExport() + "Textures\\")
		
		makeDir XMLPath all:true
		
		ExportStaticMeshXML (XMLPath + "static_meshes.xml")	--Export the XML file
		
		if ExportMeshesCheckBox.checked then
		(
			makeDir MeshesPath all:true
			ExportStaticMeshes MeshesPath		--Export the meshes
		)
		
		if ExportTexturesCheckBox.checked then
		(
			makeDir TexturesPath all:true
			ExportTexturesMesh TexturesPath	--Export the textures
		)
		
		messageBox "Static Meshes exported correctly." title:"INFORMATION"
	)
)
