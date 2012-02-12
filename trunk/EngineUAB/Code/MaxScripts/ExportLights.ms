fileIn "HelperFunctions.ms"

function ExportLights Objs FilePath IsPathCompleted RelativeShadowMaskPath =
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
		if ( ( (classof(Objs[i]) == Omnilight) OR (classof(Objs[i]) == TargetDirectionallight) ) OR (classof(Objs[i]) == targetSpot) ) then
		(
			local l_XML_Line = stringstream ""
			local l_XML_Layers = stringstream ""
			local l_Print_Layers = false
			
			local l_Name = Objs[i].name
			local l_Pos = Objs[i].pos
			local l_Color = Objs[i].rgb
			local l_StartAtt = Objs[i].farAttenStart
			local l_EndAtt = Objs[i].farAttenEnd
			
			l_Pos = RHTranslationToLH l_Pos
			
			format "name=\"%\" pos=\"% % %\" color=\"% % %\" att_start_range=\"%\" att_end_range=\"%\"" l_Name l_Pos[1] l_Pos[2] l_Pos[3] l_Color.r l_Color.g l_Color.b l_StartAtt l_EndAtt to:l_XML_Line
			
			local l_GenShadowMap = getUserProp Objs[i] "generate_shadow_map"
			local l_GenStaticShadowMap = getUserProp Objs[i] "generate_static_shadow_map"
			local l_ShadowMaskPath = getUserProp Objs[i] "shadow_mask_path"
			
			if (l_GenShadowMap == true) then
			(
				local l_XML_Shadow_Params = stringstream ""
				
				local l_ShMapFormat = getUserProp Objs[i] "shadow_map_format_type"				
				local l_ShMapWidth = getUserProp Objs[i] "shadow_map_width"				
				local l_ShMapHeigth = getUserProp Objs[i] "shadow_map_height"
				local l_DynamicLayer = getUserProp Objs[i] "shadow_mask_dynamic_layer"
				
				format "generate_shadow_map=\"true\" shadow_map_format_type=\"%\" shadow_map_width=\"%\" shadow_map_height=\"%\"" l_ShMapFormat l_ShMapWidth l_ShMapHeigth to:l_XML_Shadow_Params
				format "\t\t<dynamic renderable_objects_layer=\"%\" />\n" l_DynamicLayer to:l_XML_Layers
				l_Print_Layers = true
				
				if(l_GenStaticShadowMap == true) then
				(
					local l_XML_Static_Shadow_Params = stringstream ""
					
					local l_SShMapFormat = getUserProp Objs[i] "static_shadow_map_format_type"
					local l_SShMapWidth = getUserProp Objs[i] "static_shadow_map_width"					
					local l_SShMapHeigth = getUserProp Objs[i] "static_shadow_map_height"
					local l_Static_Layer = getUserProp Objs[i] "shadow_mask_static_layer"
					
					format "generate_static_shadow_map=\"true\" static_shadow_map_format_type=\"%\" static_shadow_map_width=\"%\" static_shadow_map_height=\"%\"" l_SShMapFormat l_SShMapWidth l_SShMapHeigth to:l_XML_Static_Shadow_Params
					l_XML_Shadow_Params  = (l_XML_Shadow_Params as string) + " " + (l_XML_Static_Shadow_Params as string)
					
					format "\t\t<static renderable_objects_layer=\"%\" />\n" l_Static_Layer to:l_XML_Layers
					l_Print_Layers = true
				)
				
				if( l_ShadowMaskPath != "undefined" ) then
				(
					local l_XML_Shadow_Mask_Path = stringstream ""
					local fileString = (filenameFromPath l_ShadowMaskPath)
					local l_Path = RelativeShadowMaskPath + fileString
					
					makeDir RelativeShadowMaskPath all:true
					copyFile l_ShadowMaskPath l_Path
					
					format "shadow_texture_mask=\"./Data/General/Textures/%\"" fileString to:l_XML_Shadow_Mask_Path
					l_XML_Shadow_Params  = (l_XML_Shadow_Params as string) + " " + (l_XML_Shadow_Mask_Path as string)
				)
				
				l_XML_Line = (l_XML_Line as string) + " " + (l_XML_Shadow_Params as string)
			)
			
			local l_LightType = ""
		
			if (classof(Objs[i]) == Omnilight) then
			(
				l_LightType = "omni"
			)
			else if (classof(Objs[i]) == TargetDirectionallight) then
			(
				l_LightType = "directional"
				
				local l_Dir=(Objs[i].target.pos-Objs[i].pos)
				l_Dir = RHTranslationToLH l_Dir
				
				local l_DirectionalS = stringstream ""
				
				format "dir=\"% % %\"" l_Dir[1] l_Dir[2] l_Dir[3] to:l_DirectionalS
				
				l_XML_Line = (l_XML_Line as string) + " " + (l_DirectionalS as string)
			)
			else if (classof(Objs[i]) == targetSpot) then
			(
				l_LightType = "spot"
				
				local l_Dir=(Objs[i].target.pos-Objs[i].pos)
				local l_FallOff = Objs[i].falloff
				local l_Angle = Objs[i].hotspot 
				l_Dir = RHTranslationToLH l_Dir
				
				local l_SpotS = stringstream ""
				
				format "dir=\"% % %\" angle=\"%\" fall_off=\"%\"" l_Dir[1] l_Dir[2] l_Dir[3] l_Angle l_FallOff to:l_SpotS
				
				l_XML_Line = (l_XML_Line as string) + " " + (l_SpotS as string)
			)

			l_XML_Line = "\t<" + l_LightType + " " + (l_XML_Line as string) + " >\n";
			
			if( l_Print_Layers ) then
			(
				l_XML_Line = (l_XML_Line as string) + (l_XML_Layers as string)
			)
			
			l_XML_Line = (l_XML_Line as string) + "\t</" + l_LightType +  ">\n";
			
			format "%" l_XML_Line to:file
		)
	)
	
	format "</lights>\n" to:file
	close file
)