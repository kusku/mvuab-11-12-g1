fileIn "HelperFunctions.ms"

function ExportLights Objs FilePath IsPathCompleted=
(
	local stFile = FilePath
	if IsPathCompleted == false then
	(
		stFile = FilePath + "\\lights.xml"
	)
	
	local file = createfile stFile
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<lights>\n" to:file
	
	for i=1 to Objs.count do
	(
		if (classof(Objs[i]) == Omnilight) then
		(
			local l_Name = Objs[i].name
			local l_Pos = Objs[i].pos
			local l_Color = Objs[i].rgb
			local l_StartAtt = Objs[i].farAttenStart
			local l_EndAtt = Objs[i].farAttenEnd
			local l_Shadows = Objs[i].atmosShadows
			
			l_Pos = RHTranslationToLH l_Pos
			
			format "\t<omni name=\"%\" pos=\"% % %\" color=\"% % %\" render_shadows=\"%\" att_start_range=\"%\" att_end_range=\"%\" />\n" l_Name l_Pos[1] l_Pos[2] l_Pos[3] l_Color.r l_Color.g l_Color.b l_Shadows l_StartAtt l_EndAtt to:file
		)
		else if (classof(Objs[i]) == TargetDirectionallight) then
		(
			local l_Name = Objs[i].name
			local l_Pos = Objs[i].pos
			local l_Dir=(Objs[i].target.pos-Objs[i].pos)
			local l_Color = Objs[i].rgb
			local l_StartAtt = Objs[i].farAttenStart
			local l_EndAtt = Objs[i].farAttenEnd
			local l_Shadows = Objs[i].atmosShadows
			
			l_Pos = RHTranslationToLH l_Pos
			l_Dir = RHTranslationToLH l_Dir
			
			format "\t<directional name=\"%\" pos=\"% % %\" dir=\"% % %\" color=\"% % %\" render_shadows=\"%\" att_start_range=\"%\" att_end_range=\"%\" />\n" l_Name l_Pos[1] l_Pos[2] l_Pos[3] l_Dir[1] l_Dir[2] l_Dir[3] l_Color.r l_Color.g l_Color.b l_Shadows l_StartAtt l_EndAtt to:file
		)
		else if (classof(Objs[i]) == targetSpot) then
		(
			local l_Name = Objs[i].name
			local l_Pos = Objs[i].pos
			local l_Dir=(Objs[i].target.pos-Objs[i].pos)
			local l_Color = Objs[i].rgb
			local l_StartAtt = Objs[i].farAttenStart
			local l_EndAtt = Objs[i].farAttenEnd
			local l_Shadows = Objs[i].atmosShadows
			local l_FallOff = Objs[i].falloff
			local l_Angle = Objs[i].hotspot 
			
			l_Pos = RHTranslationToLH l_Pos
			l_Dir = RHTranslationToLH l_Dir
			
			format "\t<spot name=\"%\" pos=\"% % %\" dir=\"% % %\" color=\"% % %\" angle=\"%\" fall_off=\"%\" render_shadows=\"%\" att_start_range=\"%\" att_end_range=\"%\" />\n" l_Name l_Pos[1] l_Pos[2] l_Pos[3] l_Dir[1] l_Dir[2] l_Dir[3] l_Color.r l_Color.g l_Color.b l_Angle l_FallOff l_Shadows l_StartAtt l_EndAtt to:file
		)
	)
	
	format "</lights>\n" to:file
	close file
)