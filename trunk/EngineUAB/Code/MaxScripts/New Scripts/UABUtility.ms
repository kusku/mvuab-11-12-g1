fileIn "ExportMesh.ms"

utility UABUtility "UAB Utility" width:178 height:385
(
	label lPath "C:\\Users\Marc\Desktop\Proves" pos:[1,40] width:193 height:56
	button bSetPath "Set Path" pos:[69,11] width:60 height:25
	checkbox cbExportMesh "Export Mesh" pos:[9,265] width:80 height:15 checked:true
	checkbox cbCopyTextures "CopyTextures" pos:[9,282] width:100 height:15 checked:true
	dropdownList ddLocation "Sub Level:" pos:[6,113] width:159 height:40 items:#("General", "Chapter1", "Chapter2", "Chapter3") selection:1
	GroupBox gbExportmesh "Export Mesh Options" pos:[3,220] width:160 height:145
	button bMeshExport "Export" pos:[78,334] width:50 height:25
	checkbox cbCounterClockwise "Counter Clockwise" pos:[10,248] width:125 height:15 checked:true
	radiobuttons rdoExport "Export Objects" pos:[8,173] width:67 height:46 labels:#("Selection", "Scene") default:1 columns:1
	checkbox cbCreateStaticMesh "Create Static Mesh" pos:[11,299] width:126 height:15 checked:true
	checkbox cbPrintDebugInfo "Print Debug Info" pos:[9,316] width:126 height:15 checked:false

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
		
		if rdoExport.state == 2 then 
		(
			select $*
		)
		
		ExportSelection $ filepath subOutPutDir cbCounterClockwise.checked cbExportMesh.checked cbCopyTextures.checked cbCreateStaticMesh.checked cbPrintDebugInfo.checked
	)
)
