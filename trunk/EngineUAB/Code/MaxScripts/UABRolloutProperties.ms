fileIn "HelperFunctions.ms"

rollout UABSetProperties "UABSetProperties" width:624 height:652
(
	GroupBox grp1 "Render" pos:[8,4] width:249 height:182
	checkbox ExportRenderCB "Export" pos:[14,25] width:53 height:15
	dropdownList ExportRenderList "" pos:[94,24] width:156 height:21 items:#("static_mesh", "animated_model")
	checkbox VisibleRenderCB "Visible" pos:[14,47] width:76 height:18
	checkbox LayerRenderCB "Layer" pos:[14,70] width:71 height:17
	dropdownList LayerRenderList "" pos:[94,68] width:156 height:21 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles", "vegetation", "water", "fish", "birds", "terrain")
	label lbl1 "Anim. model" pos:[12,112] width:70 height:16
	edittext AnimModelRenderText "" pos:[92,108] width:157 height:21
	GroupBox grp2 "Physics" pos:[4,195] width:249 height:147
	checkbox GeneratePhysicsCB "Generate" pos:[11,240] width:71 height:18
	dropdownList GeneratePhysicsList "" pos:[90,236] width:156 height:21 items:#("triangle_mesh", "convex_shape", "bounding_box", "sphere_shape", "plane_shape", "capsule_shape")
	checkbox layerPhysicsCB "Layer" pos:[11,267] width:72 height:16
	dropdownList LayerPhysicsList "" pos:[90,263] width:156 height:21
	GroupBox grp3 "Logic" pos:[278,13] width:244 height:48
	checkbox GenerateLogicCB "Generate" pos:[283,29] width:71 height:18
	dropdownList GenerateLogicList "" pos:[358,27] width:156 height:21 items:#("rails")
	button GetButton "Get" pos:[433,620] width:83 height:24
	button SetButton "Set" pos:[529,619] width:83 height:24
	checkbox cbGenShadowMap "Dynamic Shadow Map:" pos:[15,365] width:84 height:26 checked:false
	dropdownList ddlShMapFormat "" pos:[111,366] width:110 height:21 items:#("G32R32F", "R32F")
	dropdownList ddlShMapWidth "" pos:[111,394] width:108 height:21 items:#("512", "1024", "2048")
	label lbShMapWidth "Width:" pos:[67,397] width:35 height:18
	dropdownList ddlShMapHeigth "" pos:[111,422] width:108 height:21 items:#("512", "1024", "2048")
	label lbShMapHeigth "Heigth:" pos:[65,426] width:35 height:18
	checkbox cbGenSShMap "Static Shadow Map:" pos:[17,452] width:91 height:26 checked:false
	dropdownList ddlSShMapFormat "" pos:[111,456] width:110 height:21 items:#("G32R32F", "R32F")
	dropdownList ddlSShMapWidth "" pos:[111,485] width:108 height:21 items:#("512", "1024", "2048")
	label lbSShMapWidth "Width:" pos:[72,488] width:35 height:18
	dropdownList ddlSShMapHeigth "" pos:[111,513] width:108 height:21 items:#("512", "1024", "2048")
	label lbSShMapHeigth "Heigth:" pos:[71,516] width:35 height:18
	GroupBox grpLights "Lights" pos:[6,345] width:243 height:290
	button btnShadowMask "Shadow Mask" pos:[18,547] width:77 height:20
	label lblShadowMask "C:\\" pos:[124,547] width:119 height:19
	checkbox cbShadowMask "" pos:[100,545] width:24 height:25 witdh:25
	label lblStaticLayer "Static Layer:" pos:[40,573] width:70 height:14
	label lbl43 "Dynamic Layer:" pos:[24,598] width:77 height:14
	dropdownList ddlStaticLayer "" pos:[112,574] width:120 height:21 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles", "vegetation", "water", "fish", "birds", "terrain")
	dropdownList ddlDynamicLayer "" pos:[112,598] width:120 height:21 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles", "vegetation", "water", "fish", "birds", "terrain")
	GroupBox grp5 "AI Waypoints" pos:[279,98] width:326 height:290
	button RefreshBrotherNodes "Refresh WP" pos:[294,344] width:94 height:25
	checkbox IsWayPoint "Is Waypoint" pos:[292,123] width:119 height:19
	dropdownList BrotherNodesAI "Brother Nodes" pos:[293,236] width:173 height:40
	button RemoveBrotherNode "Remove" pos:[480,251] width:96 height:25
	dropdownList PossibleBrotherNodesAI "Possible Brother Nodes" pos:[295,291] width:172 height:40
	button AddBrotherNode "Add" pos:[483,309] width:90 height:24
	dropdownList WayPointGroup "Waypoint Group" pos:[292,163] width:175 height:40 items:#("Group 1", "Group 2", "Group 3", "Group 4", "Group 5", "Fish Group", "Bird Group")
	checkbox cbIsInstance "Is Instance:" pos:[10,160] width:80 height:16
	edittext InstanceName "" pos:[98,158] width:145 height:18
	checkbox cbParentInstance "Is Parent Instance" pos:[10,140] width:127 height:14
	checkbox cbTextureSplatting "Texture Splatting" pos:[290,421] width:106 height:16
	GroupBox gbTextureSplatting "Texture Splatting" pos:[279,399] width:327 height:209
	label lbBlend3 "C:\\" pos:[344,520] width:235 height:39
	button bnBlend3 "Blend 3" pos:[287,521] width:49 height:20
	label lbBlend2 "C:\\" pos:[342,481] width:235 height:36
	button bnBlend2 "Blend 2" pos:[286,482] width:49 height:20
	label lbBlend1 "C:\\" pos:[341,446] width:235 height:35
	button bnBlend1 "Blend 1" pos:[287,443] width:49 height:20
	label lbBlend4 "C:\\" pos:[344,563] width:235 height:36
	button bnBlend4 "Blend 4" pos:[286,562] width:49 height:20
	checkbox cbUsePXObj "Use PhysX Obj:" pos:[9,292] width:97 height:18
	edittext tbPXObj "" pos:[106,291] width:136 height:19
	checkbox cbIsPXObj "Is PhysX Obj" pos:[12,219] width:116 height:18
	checkbox cbUsePxPos "Use PhysX Obj Position" pos:[10,316] width:135 height:20
	
	local m_ObjSelected = undefined
	
	function ResetProps =
	(
		ExportRenderCB.checked = false
		VisibleRenderCB.checked = false
		LayerRenderCB.checked = false
		GeneratePhysicsCB.checked = false
		layerPhysicsCB.checked = false
		GenerateLogicCB.checked = false
		IsWayPoint.checked = false
		cbIsInstance.checked = false
		cbParentInstance.checked = false
		AnimModelRenderText.text = ""
		cbTextureSplatting.checked = false
		cbUsePXObj.checked = false
		tbPXObj.text = ""
		cbIsPXObj.checked = false
		cbUsePxPos.checked = false
		
		lbBlend1.text = "C:\\"
		lbBlend2.text = "C:\\"
		lbBlend3.text = "C:\\"
		lbBlend4.text = "C:\\"
	)
	
	function SaveObjectProps Obj =
	(
		--Set Texture Splatting 
		if cbTextureSplatting.checked then
		(
			setUserProp Obj "texture_splatting" true
		
			setUserProp Obj "texture_splatting_blend1" lbBlend1.text
			setUserProp Obj "texture_splatting_blend2" lbBlend2.text
			setUserProp Obj "texture_splatting_blend3" lbBlend3.text
			setUserProp Obj "texture_splatting_blend4" lbBlend4.text
		)
		else
		(
			setUserProp Obj "texture_splatting" false
			
			setUserProp Obj "texture_splatting_blend1" ""
			setUserProp Obj "texture_splatting_blend2" ""
			setUserProp Obj "texture_splatting_blend3" ""
			setUserProp Obj "texture_splatting_blend4" ""
		)
		
		if cbIsPXObj.checked then
		(
			setUserProp Obj "is_physx_obj" true
		)
		else
		(
			setUserProp Obj "is_physx_obj" false
		)
		
		--Set Instance
		if cbIsInstance.checked then
		(
			setUserProp Obj "is_instance" true
			setUserProp Obj "instance_name" InstanceName.text
		)
		else
		(
			setUserProp Obj "is_instance" false
		)
		
		if cbParentInstance.checked then
		(
			setUserProp Obj "parent_instance" true
		)
		else
		(
			setUserProp Obj "parent_instance" false
		)
		
		--Set Export Render Prop to Object
		if ExportRenderCB.checked then
		(
			setUserProp Obj "render_export" ExportRenderList.selected
		)
		else
		(
			setUserProp Obj "render_export" "undefined"
		)
		
		--Set Visible Render Prop to Object
		setUserProp Obj "render_visible" VisibleRenderCB.checked
		
		--Set Layer Render prop to Object
		if LayerRenderCB.checked then
		(
			setUserProp Obj "render_layer" LayerRenderList.selected
		)
		else
		(
			setUserProp Obj "render_layer" "undefined"
		)
		
		--Set Anim Model Core Prop to Object
		setUserProp Obj "anim_model_core" AnimModelRenderText.text
		
		--Set Generate Physics prop to Object
		if GeneratePhysicsCB.checked then
		(
			setUserProp Obj "physics_generate" GeneratePhysicsList.selected
			
			setUserProp Obj "physics_use_px_obj" cbUsePXObj.checked
			setUserProp Obj "physics_px_obj" tbPXObj.text
			setUserProp Obj "physics_use_px_obj_pos" cbUsePxPos.checked
		)
		else
		(
			setUserProp Obj "physics_generate" "undefined"
			
			setUserProp Obj "physics_use_px_obj" false
			setUserProp Obj "physics_use_px_obj_pos" false
		)
		
		--Set Layer Physics prop to Object
		if layerPhysicsCB.checked then
		(
			setUserProp Obj "physics_layer" layerPhysicsList.selected
		)
		else
		(
			setUserProp Obj "physics_layer" "undefined"
		)
		
		--Set Generate Logic prop to Object
		if GenerateLogicCB.checked then
		(
			setUserProp Obj "logic_generate" GenerateLogicList.selected
		)
		else
		(
			setUserProp Obj "logic_generate" "undefined"
		)
		
		if (cbGenShadowMap.checked) then
		(	
			setUserProp Obj "generate_shadow_map" cbGenShadowMap.checked
			setUserProp Obj "shadow_map_format_type" ddlShMapFormat.selected
			setUserProp Obj "shadow_map_width" ddlShMapWidth.selected
			setUserProp Obj "shadow_map_height" ddlShMapHeigth.selected
			setUserProp Obj "shadow_mask_dynamic_layer" ddlDynamicLayer.selected
			
			if cbShadowMask.checked then
			(
				setUserProp Obj "shadow_mask_path" lblShadowMask.text
			)
			else
			(
				setUserProp Obj "shadow_mask_path" "undefined"
			)
			
			if(cbGenSShMap.checked) then
			(
				setUserProp Obj "generate_static_shadow_map" cbGenSShMap.checked
				setUserProp Obj "static_shadow_map_format_type" ddlSShMapFormat.selected
				setUserProp Obj "static_shadow_map_width" ddlSShMapWidth.selected
				setUserProp Obj "static_shadow_map_height" ddlSShMapHeigth.selected
				setUserProp Obj "shadow_mask_static_layer" ddlStaticLayer.selected
			)
			else
			(
				setUserProp Obj "generate_static_shadow_map" "false"
				setUserProp Obj "static_shadow_map_format_type" "undefined" 
				setUserProp Obj "static_shadow_map_width" "undefined"
				setUserProp Obj "static_shadow_map_height" "undefined"
				setUserProp Obj "shadow_mask_static_layer" "undefined"
			)
		)
		else
		(
			cbGenSShMap.checked = false
			
			setUserProp Obj "generate_shadow_map" "false"
			setUserProp Obj "shadow_map_format_type" "undefined" 
			setUserProp Obj "shadow_map_width" "undefined"
			setUserProp Obj "shadow_map_height" "undefined"
			
			setUserProp Obj "generate_static_shadow_map" "false"
			setUserProp Obj "static_shadow_map_format_type" "undefined" 
			setUserProp Obj "static_shadow_map_width" "undefined"
			setUserProp Obj "static_shadow_map_height" "undefined"
			
			setUserProp Obj "shadow_mask_path" "undefined"
			setUserProp Obj "shadow_mask_dynamic_layer" "undefined"
			setUserProp Obj "shadow_mask_static_layer" "undefined"
		)
		
		if IsWayPoint.checked then
		(
			if classof(Obj) != Point then
			(
				messageBox "Only Points can be WayPoints" title:"ATTENTION"
			)
			else
			(
				setUserProp Obj "is_waypoint" "true"
				setUserProp Obj "waypoint_group" WayPointGroup.selected
				setUserProp Obj "waypoint_brothers" BrotherNodesAI.items
			)
		)
		else
		(
			setUserProp Obj "is_waypoint" false
		)
	)
	
	on GetButton pressed do
	(
		if $ == undefined then
		(
			messageBox "No has seleccionado ningún objeto."
		)
		else if $ == $selection then
		(
			messageBox "Sólo se puede obtener las propiedades de un objeto."
		)
		else
		(
			m_ObjSelected = $
			
			local l_ExportRender = getUserProp $ "render_export"
			local l_VisibleRender = getUserProp $ "render_visible"
			local l_LayerRender = getUserProp $ "render_layer"
			local l_AnimModel = getUserProp $ "anim_model_core"
			local l_GeneratePhysics = getUserProp $ "physics_generate"
			local l_LayerPhysics = getUserProp $ "physics_layer"
			local l_GenerateLogic = getUserProp $ "logic_generate"
			local l_GenShadowMap = getUserProp $ "generate_shadow_map"
			local l_GenStaticShadowMap = getUserProp $ "generate_static_shadow_map"
			local l_ShadowMaskPath = getUserProp $ "shadow_mask_path"
			local l_SMStaticLayer = getUserProp $ "shadow_mask_static_layer"
			local l_SMDynamicLayer = getUserProp $ "shadow_mask_dynamic_layer"
			local l_IsWP = getUserProp $ "is_waypoint"
			local l_IsInstace = getUserProp $ "is_instance"
			local l_ParentInstance = getUserProp $ "parent_instance"
			local l_TextureSplatting = getUserProp $ "texture_splatting"
			local l_UsePXObj = getUserProp $ "physics_use_px_obj"
			local l_IsPXObj = getUserProp $ "is_physx_obj"
			
			--Unchecked all check boxes
			ResetProps()
			
			if l_IsWP != undefined AND l_IsWP == true then
			(
				IsWayPoint.checked = true
				local bros = getUserProp $ "waypoint_brothers" 
				
				if(bros != undefined) then
				(
					BrotherNodesAI.items = FromStringToArray bros
				)
			)
			
			if(l_IsPXObj == true)then
			(
				cbIsPXObj.checked = true
			)
			else
			(
				cbIsPXObj.checked = false
			)
			
			
			if l_TextureSplatting == true then
			(
				cbTextureSplatting.checked = true
				
				lbBlend1.text = (getUserProp $ "texture_splatting_blend1" as string)
				lbBlend2.text = (getUserProp $ "texture_splatting_blend2" as string)
				lbBlend3.text = (getUserProp $ "texture_splatting_blend3" as string)
				lbBlend4.text = (getUserProp $ "texture_splatting_blend4" as string)
			)   
			else
			(
				cbTextureSplatting.checked = false
				
				lbBlend1.text = "C:\\"
				lbBlend2.text = "C:\\"
				lbBlend3.text = "C:\\"
				lbBlend4.text = "C:\\"
			)
			
			--Set Instance
			if l_ParentInstance == true then
			(
				cbParentInstance.checked = true
			)
			else
			(
				cbParentInstance.checked = false
			)
			
			if l_IsInstace == true then
			(
				cbIsInstance.checked = true
				
				local instanceNameProp = getUserProp $ "instance_name" as string
				InstanceName.text = instanceNameProp
			)
			else
			(
				cbIsInstance.checked = false
				InstanceName.text = ""
			)
			
			--Set Export Render Prop to Rollout
			if l_ExportRender != "undefined" then
			(
				for i = 1 to ExportRenderList.items.count do
				(
					if ExportRenderList.items[i] == l_ExportRender then
					(
						ExportRenderList.selection = i
						ExportRenderCB.checked = true
					)
				)
			)
			
			--Set Visible Render Prop to Rollout
			if l_VisibleRender != undefined then
			(
				VisibleRenderCB.checked = l_VisibleRender
			)
			
			--Set Layer Render Prop to Rollout
			if l_LayerRender != undefined then
			(
				for i = 1 to LayerRenderList.items.count do
				(
					if LayerRenderList.items[i] == l_LayerRender then
					(
						LayerRenderList.selection = i
						LayerRenderCB.checked = true
					)
				)
			)
			
			--Set Animated Core Model Name to Rollout
			if l_AnimModel != undefined then
			(
				AnimModelRenderText.text = l_AnimModel
			)
			
			--Set Generate Physics Prop to Rollout
			if l_GeneratePhysics != undefined then
			(
				for i = 1 to GeneratePhysicsList.items.count do
				(
					if GeneratePhysicsList.items[i] == l_GeneratePhysics then
					(
						GeneratePhysicsList.selection = i
						GeneratePhysicsCB.checked = true
					)
				)
				
				if(l_UsePXObj == true) then
				(
					cbUsePXObj.checked = true
					tbPXObj.text = getUserProp $ "physics_px_obj" as string
					
					local usePxPos = getUserProp $ "physics_use_px_obj_pos"
					
					if(usePxPos == true)then
					(
						cbUsePxPos.checked = true
					)
					else
					(
						cbUsePxPos.checked = false
					)
				)
			)
			
			--Set Layer Physics Prop to Rollout
			if l_LayerPhysics != undefined then
			(
				for i = 1 to layerPhysicsList.items.count do
				(
					if layerPhysicsList.items[i] == l_LayerPhysics then
					(
						layerPhysicsList.selection = i
						layerPhysicsCB.checked = true
					)
				)
			)
			
			--Set Generate Logic Prop to Rollout
			if l_GenerateLogic != undefined then
			(
				for i = 1 to GenerateLogicList.items.count do
				(
					if GenerateLogicList.items[i] == l_GenerateLogic then
					(
						GenerateLogicList.selection = i
						GenerateLogicCB.checked  =true
					)
				)
			)
			
			--Shadow Map Params
			if l_SMStaticLayer != undefined then
			(
				for i = 1 to ddlStaticLayer.items.count do
				(
					if ddlStaticLayer.items[i] == l_SMStaticLayer then
					(
						ddlStaticLayer.selection = i
					)
				)
			)
			
			if l_SMDynamicLayer != "undefined" then
			(
				for i = 1 to ddlDynamicLayer.items.count do
				(
					if ddlDynamicLayer.items[i] == l_SMDynamicLayer then
					(
						ddlDynamicLayer.selection = i
					)
				)
			)
			
			if l_ShadowMaskPath != undefined then
			(
				lblShadowMask.text = l_ShadowMaskPath
				cbShadowMask.checked = true
			)
			else
			(
				cbShadowMask.checked = false
			)
			
			if (l_GenShadowMap == true) then
			(	
				cbGenShadowMap.checked = true
				
				local l_ShMapFormat = getUserProp $ "shadow_map_format_type"
				for i = 1 to ddlShMapFormat.items.count do
				(
					if ddlShMapFormat.items[i] == l_ShMapFormat then
					(
						ddlShMapFormat.selection = i
					)
				)
				
				local l_ShMapWidth = getUserProp $ "shadow_map_width"
				for i = 1 to ddlShMapWidth.items.count do
				(
					if ddlShMapWidth.items[i] == l_ShMapWidth then
					(
						ddlShMapWidth.selection = i
					)
				)
				
				local l_ShMapHeigth = getUserProp $ "shadow_map_height"
				for i = 1 to ddlShMapHeigth.items.count do
				(
					if ddlShMapHeigth.items[i] == l_ShMapHeigth then
					(
						ddlShMapHeigth.selection = i
					)
				)
				
				if(l_GenStaticShadowMap == true) then
				(
					cbGenSShMap.checked = true
				
					local l_SShMapFormat = getUserProp $ "static_shadow_map_format_type"
					for i = 1 to ddlSShMapFormat.items.count do
					(
						if ddlSShMapFormat.items[i] == l_SShMapFormat then
						(
							ddlSShMapFormat.selection = i
						)
					)
					
					local l_SShMapWidth = getUserProp $ "static_shadow_map_width"
					for i = 1 to ddlSShMapWidth.items.count do
					(
						if ddlSShMapWidth.items[i] == l_SShMapWidth then
						(
							ddlSShMapWidth.selection = i
						)
					)
					
					local l_SShMapHeigth = getUserProp $ "static_shadow_map_height"
					for i = 1 to ddlSShMapHeigth.items.count do
					(
						if ddlSShMapHeigth.items[i] == l_SShMapHeigth then
						(
							ddlSShMapHeigth.selection = i
						)
					)
				)
			)			
		)
	)
	
	on SetButton pressed do
	(	
		if $ == undefined then
		(
			messageBox "No has seleccionado ningún objeto."
		)
		else if $ == $selection then
		(
			local l_Option = queryBox "Hay más de un objeto seleccionado para guardar las propiedades.\n ¿Seguro que quieres guardar estas propiedades a los objetos?"
			if l_Option then
			(
				for obj in $ do
				(
					SaveObjectProps obj
				)
			)
		)
		else
		(
			if m_ObjSelected == $ then
			(
				SaveObjectProps $
			)
			else 
			(
				local l_Option  = queryBox "El objeto seleccionado no es el mismo del que se obtenieron las propiedades.\n ¿Asignar las propiedades al objeto?"
				if l_Option then
				(
					SaveObjectProps $
				)
			)
		)
	)
	
	on btnShadowMask pressed do
	(
		local data_path = getOpenFileName caption:"Shadow Mask Path" types:"TGA(*.tga)|*.tga|BMP(*.bmp)|*.bmp|JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|DDS(*.dds)|*.dds|All|*.*|"initialDir=lblShadowMask.text
		
		if data_path != undefined then
		(
			lblShadowMask.text = data_path
		)
	)
	
	on RefreshBrotherNodes pressed do
	(
		if classof($) != Point then
		(
			messageBox "Only Points can be WayPoints" title:"ATTENTION"
			return 0
		)
		
		local wps = #()
		local mainGroup = (getUserProp $ "waypoint_group")
		
		if mainGroup == undefined then
		(
			messageBox "No Waypoint group defined" title:"ATTENTION"
			return 0
		)
		
		for pnt in Helpers do
		(
			if pnt == $ then
			(
				continue
			)
			
			if (classof(pnt) == Point)  then
			(
				local l_IsWP = (getUserProp pnt "is_waypoint")
				local l_WPGroup = (getUserProp pnt "waypoint_group")
				
				if l_IsWP == true AND l_WPGroup ==  mainGroup then
				(
					append wps pnt.name
				)
			)
		)
		
		PossibleBrotherNodesAI.items = wps;
	)
	
	on AddBrotherNode pressed do
	(
		local pnt = PossibleBrotherNodesAI.selected
		
		if(pnt == undefined OR pnt == "") then
		(
			return 0
		)
		
		local broPnts = BrotherNodesAI.items
		
		for i = 1 to broPnts.count do
		(
			if broPnts[i] == pnt then
			(
				return 0
			)
		)
		
		append broPnts pnt
		
		BrotherNodesAI.items = broPnts
	)
	
	function GetBlendTexPath =
	(
		local data_path = getOpenFileName caption:"Blend TexturePath" types:"TGA(*.tga)|*.tga|BMP(*.bmp)|*.bmp|JPG(*.jpg)|*.jpg|PNG(*.png)|*.png|DDS(*.dds)|*.dds|All|*.*|"initialDir="C:\\"
			
		return data_path
	)
	
	on bnBlend1 pressed do
	(
		local data_path = GetBlendTexPath()
		
		if data_path != undefined then
		(
			lbBlend1.text = data_path
		)		
	)
	
	on bnBlend2 pressed do
	(
		local data_path = GetBlendTexPath()
		
		if data_path != undefined then
		(
			lbBlend2.text = data_path
		)		
	)
	
	on bnBlend3 pressed do
	(
		local data_path = GetBlendTexPath()
		
		if data_path != undefined then
		(
			lbBlend3.text = data_path
		)		
	)
	
	on bnBlend4 pressed do
	(
		local data_path = GetBlendTexPath()
		
		if data_path != undefined then
		(
			lbBlend4.text = data_path
		)		
	)
	
)

