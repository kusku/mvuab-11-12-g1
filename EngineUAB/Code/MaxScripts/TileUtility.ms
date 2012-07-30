
function TileUVMap =
(
	local numFaces = getNumFaces $
	
	format "Faces: %\n" numFaces
	
	$.Unwrap_UVW.setTVSubObjectMode 3
	
	for i=1 to numFaces do
	(
		local bitarr = #(i)
		
		--polyop.setFaceSelection $ i
		$.Unwrap_UVW.selectFaces (bitarr as BitArray)
		--$.Unwrap_UVW.setSelectedGeomEdges i
		
		--$.Unwrap_UVW.LSCMReset()
		--$.Unwrap_UVW.peltDialogRelaxHeavy()
		--$.Unwrap_UVW.mappingMode 5
		$.Unwrap_UVW.mappingMode 5
		--$.Unwrap_UVW.peltDialogRun()
		
		
		storeCmd = getCommandPanelTaskMode()
		 max modify mode
		 
		dialogHWND = windows.getChildHWND 0 "Pelt Map"
		dialogChildren = windows.getChildrenhwnd dialogHWND[1]
		 
		for child in dialogChildren where child[4] == "Button" and child[5] == "Commit" do 
		(
		 UIAccessor.pressButton child[1]
		 )
		 
		 setCommandPanelTaskMode storeCmd
	)
)

function SetUVTiles obj =
(
	local UVArray = #()
	
	append UVArray (point3 0 0 0)
	append UVArray (point3 1 0 0)
	append UVArray (point3 1 1 0)
	append UVArray (point3 0 1 0)
	
	--format "UVArray 1: %\n" UVArray[1]
	--format "UVArray 2: %\n" UVArray[2]
	--format "UVArray 3: %\n" UVArray[3]
	--format "UVArray 4: %\n" UVArray[4]

	local numFaces = getNumFaces obj
	
	--format "Faces: %\n" numFaces

	for i=1 to numFaces do
	(
		local IdxsMap = polyop.getMapFace obj 2 i
		
		--format "IdxMap: %\n" IdxsMap
		
		for j=1 to 4 do
		(
			local TUV2Map = polyop.getMapVert obj 2 (IdxsMap[j] as integer)
			--format "TUV2Map: %\n" TUV2Map
			
			polyop.setMapVert $ 2 (IdxsMap[j] as integer) (UVArray[j] as point3)
			
			TUV2Map = polyop.getMapVert obj 2 (IdxsMap[j] as integer)
			--format "New TUV2Map: %\n" TUV2Map
		)
		
		--format"\n"
	)
)

function CreateTiles =
(
	if($ == $selection) then
	(
		Format "out\n"
		return 0
	)
	
	TileUVMap()
	
	collapseStack $
	
	SetUVTiles $
)

CreateTiles()