fileIn "HelperFunctions.ms"

rollout UABSetProperties "UABSetProperties" width:624 height:652
(
	GroupBox grp1 "Render" pos:[8,4] width:249 height:161
	checkbox ExportRenderCB "Export" pos:[14,25] width:53 height:15
	dropdownList ExportRenderList "" pos:[94,24] width:156 height:24 items:#("static_mesh", "animated_model")
	checkbox VisibleRenderCB "Visible" pos:[14,47] width:76 height:18
	checkbox LayerRenderCB "Layer" pos:[14,70] width:71 height:17
	dropdownList LayerRenderList "" pos:[94,68] width:156 height:24 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles", "vegetation", "water")
	label lbl1 "Anim. model" pos:[14,105] width:70 height:16
	edittext AnimModelRenderText "" pos:[93,101] width:157 height:21
	GroupBox grp2 "Physics" pos:[6,178] width:249 height:70
	checkbox GeneratePhysicsCB "Generate" pos:[12,193] width:71 height:18
	dropdownList GeneratePhysicsList "" pos:[92,190] width:156 height:24 items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
	checkbox layerPhysicsCB "Layer" pos:[12,220] width:72 height:16
	dropdownList LayerPhysicsList "" pos:[92,218] width:156 height:24
	GroupBox grp3 "Logic" pos:[7,255] width:244 height:48
	checkbox GenerateLogicCB "Generate" pos:[12,271] width:71 height:18
	dropdownList GenerateLogicList "" pos:[87,269] width:156 height:24
	button GetButton "Get" pos:[433,620] width:83 height:24
	button SetButton "Set" pos:[529,619] width:83 height:24
	checkbox cbGenShadowMap "Dynamic Shadow Map:" pos:[13,328] width:84 height:26 checked:false
	dropdownList ddlShMapFormat "" pos:[109,329] width:110 height:24 items:#("G32R32F", "R32F")
	dropdownList ddlShMapWidth "" pos:[109,357] width:108 height:24 items:#("512", "1024", "2048")
	label lbShMapWidth "Width:" pos:[65,360] width:35 height:18
	dropdownList ddlShMapHeigth "" pos:[109,385] width:108 height:24 items:#("512", "1024", "2048")
	label lbShMapHeigth "Heigth:" pos:[63,389] width:35 height:18
	checkbox cbGenSShMap "Static Shadow Map:" pos:[15,415] width:91 height:26 checked:false
	dropdownList ddlSShMapFormat "" pos:[109,419] width:110 height:24 items:#("G32R32F", "R32F")
	dropdownList ddlSShMapWidth "" pos:[109,448] width:108 height:24 items:#("512", "1024", "2048")
	label lbSShMapWidth "Width:" pos:[70,451] width:35 height:18
	dropdownList ddlSShMapHeigth "" pos:[109,476] width:108 height:24 items:#("512", "1024", "2048")
	label lbSShMapHeigth "Heigth:" pos:[69,479] width:35 height:18
	GroupBox grpLights "Lights" pos:[5,315] width:243 height:272
	button btnShadowMask "Shadow Mask" pos:[16,510] width:77 height:20
	label lblShadowMask "C:\\" pos:[122,510] width:119 height:19
	checkbox cbShadowMask "" pos:[98,508] width:24 height:25 witdh:25
	label lblStaticLayer "Static Layer:" pos:[38,536] width:70 height:14
	label lbl43 "Dynamic Layer:" pos:[22,561] width:77 height:14
	dropdownList ddlStaticLayer "" pos:[110,537] width:120 height:24 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	dropdownList ddlDynamicLayer "" pos:[110,561] width:120 height:24 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	GroupBox grp5 "AI Waypoints" pos:[278,11] width:326 height:290
	button RefreshBrotherNodes "Refresh WP" pos:[293,257] width:94 height:25
	checkbox IsWayPoint "Is Waypoint" pos:[291,36] width:119 height:19
	dropdownList BrotherNodesAI "Brother Nodes" pos:[292,149] width:173 height:43 items:#()
	button RemoveBrotherNode "Remove" pos:[479,164] width:96 height:25
	dropdownList PossibleBrotherNodesAI "Possible Brother Nodes" pos:[294,204] width:172 height:43 items:#()
	button AddBrotherNode "Add" pos:[482,222] width:90 height:24
	dropDownList WayPointGroup "Waypoint Group" pos:[291,76] width:175 height:43 items:#("Group 1", "Group 2", "Group 3", "Group 4", "Group 5")
	
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
		AnimModelRenderText.text = ""
	)
	
	function SaveObjectProps Obj =
	(
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
		)
		else
		(
			setUserProp Obj "physics_generate" "undefined"
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
)

