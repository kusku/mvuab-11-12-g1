fileIn "ExportMesh.ms"
fileIn "ExportLights.ms"
fileIn "UABRolloutProperties.ms"

utility UABUtility "UAB Utility" width:165 height:364
(
	label lPath "C:\Users\Marc\Desktop\Proves" pos:[3,36] width:157 height:69
	button bSetPath "Set Path" pos:[6,6] width:148 height:25
	checkbox cbExportMesh "Export Mesh" pos:[10,254] width:80 height:15 checked:true
	checkbox cbCopyTextures "CopyTextures" pos:[10,271] width:100 height:15 checked:true
	dropdownList ddLocation "" pos:[6,113] width:152 height:21 items:#("General", "Chapter1", "Chapter2", "Chapter3") selection:1
	GroupBox gbExportmesh "Export Mesh Options" pos:[4,218] width:151 height:136
	button bMeshExport "Export" pos:[9,323] width:137 height:25
	checkbox cbCounterClockwise "Counter Clockwise" pos:[11,237] width:125 height:15 checked:true
	radiobuttons rdoExport "Export Objects" pos:[10,141] width:67 height:62 labels:#("Selection", "Scene", "Lights") default:1 columns:1
	checkbox cbCreateStaticMesh "Create Static Mesh" pos:[12,288] width:126 height:15 checked:true
	checkbox cbPrintDebugInfo "Print Debug Info" pos:[10,305] width:126 height:15 checked:false
	button bProperties "Properties" pos:[86,169] width:72 height:20

	----------------------------------------------------------------------------------------
	
	function GetPathDirectory =
	(
		local filepath = (lPath.text + "\\")
		
		return filepath
	)
	
	function GetSubPathDirectory =
	(
		local filepath = (ddLocation.selected + "\\")
		
		return filepath
	)
	
	
	on bProperties pressed do
	(
		createDialog UABSetProperties
	)
	
	on bSetPath pressed do
	(
		local data_path = getSavePath caption:"Data Path" initialDir=lPath.text
		
		if data_path != undefined then
		(
			lPath.text = data_path
		)
	)
	on bMeshExport pressed do
	(
		local filepath = GetPathDirectory()
		local subOutPutDir = GetSubPathDirectory()
		
		if rdoExport.state == 3 then
		(
			--Export Lights
			local l_Path = filepath + "XML\\" + subOutPutDir + "Lights\\"
			makeDir l_Path
			
			ExportLights $* l_Path false
			messageBox "Lights exported correctly."
		)
		else
		(
			if rdoExport.state == 2 then 
			(
				--Select all the objects to export all the scene
				select $*
			)
			
			--Export the selected objects
			ExportSelection $ filepath subOutPutDir cbCounterClockwise.checked cbExportMesh.checked cbCopyTextures.checked cbCreateStaticMesh.checked cbPrintDebugInfo.checked
		)
	)
)
