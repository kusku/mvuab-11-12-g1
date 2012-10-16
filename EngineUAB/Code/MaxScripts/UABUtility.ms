fileIn "ExportMesh.ms"
fileIn "ExportLights.ms"
fileIn "UABRolloutProperties.ms"
fileIn "ExportWayPoints.ms"
fileIn "ExportRails.ms"
fileIn "ExportPhysXObjs.ms"
fileIn "ExportLimits.ms"
fileIn "HelperFunctions.ms"

utility UABUtility "UAB Utility" width:165 height:568
(
	label lPath "C:\\" pos:[3,36] width:157 height:69
	button bSetPath "Set Path" pos:[6,6] width:148 height:25
	checkbox cbExportMesh "Export Mesh" pos:[7,341] width:80 height:15 checked:true
	checkbox cbCopyTextures "CopyTextures" pos:[6,362] width:100 height:15 checked:true
	dropdownList ddLocation "" pos:[6,113] width:152 height:21 items:#("General", "Chapter1", "Chapter2", "Chapter3", "EntregaTest", "EntregaTestForward") selection:1
	GroupBox gbExportmesh "Export Mesh Options" pos:[2,299] width:146 height:140
	button bMeshExport "Export" pos:[5,486] width:137 height:25
	checkbox cbCounterClockwise "Counter Clockwise" pos:[9,323] width:125 height:15 checked:true
	radiobuttons rdoExport "Export Objects" pos:[10,161] width:77 height:126 labels:#("Selection", "Scene", "Lights", "WayPoints", "Rails", "PhysX Objs", "Limits") default:1 columns:1
	checkbox cbCreateStaticMesh "Create Static Mesh" pos:[8,379] width:126 height:15 checked:true
	checkbox cbPrintDebugInfo "Print Debug Info" pos:[6,397] width:126 height:15 checked:false
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
			local l_ShadowMaskPath = filepath + "General\\Textures\\"
			makeDir l_Path
			
			ExportLights $* l_Path false l_ShadowMaskPath
			messageBox "Lights exported correctly."
		)
		else if rdoExport.state == 4 then
		(
			local l_Path = filepath + subOutPutDir + "WayPoints\\"
			makeDir l_Path
			ExportWayPoints l_Path
		)
		else if rdoExport.state == 5 then
		(
			local l_Path = filepath + subOutPutDir + "Rails\\"
			makeDir l_Path
			ExportRails $* l_Path
		)
		else if rdoExport.state == 6 then
		(
			local l_Path = filepath + subOutPutDir + "PhysXObjs\\"
			makeDir l_Path
			ExportPhysXObjs $* l_Path
		)
		else if rdoExport.state == 7 then
		(
			local l_Path = filepath + subOutPutDir + "Limits\\"
			makeDir l_Path
			ExportLimitsObjs $* l_Path
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
			
			format "Export finished.\n"
		)
	)
	on bProperties pressed do
	(
		createDialog UABSetProperties
	)
)
