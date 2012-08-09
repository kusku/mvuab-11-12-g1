fileIn "HelperFunctions.ms"

function SearchRails Objs =
(
	local rails = #()
	
	for i=1 to Objs.count do
	(
		local obj = Objs[i]
		if (getUserProp obj "logic_generate") == "rails" then
		(
			append rails obj
		)
	)
	
	return rails
)

function WriteRail Rail File =
(
	local waypoints = numKnots Rail
	
	format "\t<rail name=\"%\">\n" Rail.name to:File
	
	for i=1 to waypoints do
	(
		local position = RHTranslationToLH (getKnotPoint Rail 1 i)
		format "\t\t<point value=\"% % %\" />\n" position[1] position[2] position[3] to:File
	)
	
	format "\t<rail>\n" to:File
)

function ExportRails Objs FilePath =
(
	local stFile 	=	FilePath
	local rails		=	#()
	
	stFile 			= 	FilePath + "\\rails.xml"
	rails				=	SearchRails Objs
	
	format "Start Exporting Rails\n"
	
	local file = createfile stFile
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<rails>\n" to:file
	
	for i=1 to rails.count do
	(
		WriteRail rails[i] file
	)
	
	format "</rails>\n" to:file
	close file
	
	format "Finish Exporting Rails\n"
)