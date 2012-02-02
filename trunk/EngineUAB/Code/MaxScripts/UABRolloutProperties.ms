rollout UABSetProperties "UABSetProperties" width:262 height:599
(
	GroupBox grp1 "Render" pos:[8,4] width:249 height:132
	checkbox ExportRenderCB "Export" pos:[14,25] width:53 height:15
	dropdownList ExportRenderList "" pos:[94,24] width:156 height:24 items:#("static_mesh", "animated_model")
	checkbox VisibleRenderCB "Visible" pos:[14,47] width:76 height:18
	checkbox LayerRenderCB "Layer" pos:[14,70] width:71 height:17
	dropdownList LayerRenderList "" pos:[94,68] width:156 height:24 items:#("solid", "alpha_objects", "alpha_blend_objects", "particles")
	label lbl1 "Anim. model" pos:[14,105] width:70 height:16
	edittext AnimModelRenderText "" pos:[93,101] width:157 height:21
	GroupBox grp2 "Physics" pos:[8,145] width:249 height:70
	checkbox GeneratePhysicsCB "Generate" pos:[14,160] width:71 height:18
	dropdownList GeneratePhysicsList "" pos:[94,157] width:156 height:24 items:#("triangle_mesh", "convex_shape", "box_shape", "sphere_shape", "plane_shape", "capsule_shape")
	checkbox layerPhysicsCB "Layer" pos:[14,187] width:72 height:16
	dropdownList LayerPhysicsList "" pos:[94,185] width:156 height:24
	GroupBox grp3 "Logic" pos:[9,222] width:244 height:48
	checkbox GenerateLogicCB "Generate" pos:[14,238] width:71 height:18
	dropdownList GenerateLogicList "" pos:[89,236] width:156 height:24
	button GetButton "Get" pos:[7,561] width:83 height:24
	button SetButton "Set" pos:[167,563] width:83 height:24
	checkbox cbGenShadowMap "Shadow Map:" pos:[15,300] width:84 height:18 checked:false
	dropdownList ddlShMapFormat "" pos:[106,296] width:110 height:24 items:#("R32F", "A8R8G8B8", "R8G8B8", "X8R8G8B8")
	dropdownList ddlShMapWidth "" pos:[107,324] width:108 height:24 items:#("512", "1024", "2048")
	label lbShMapWidth "Width:" pos:[67,327] width:35 height:18
	dropdownList ddlShMapHeigth "" pos:[107,352] width:108 height:24 items:#("512", "1024", "2048")
	label lbShMapHeigth "Heigth:" pos:[65,356] width:35 height:18
	checkbox cbGenSShMap "Static Sh Map:" pos:[14,390] width:91 height:18 checked:false
	dropdownList ddlSShMapFormat "" pos:[111,386] width:110 height:24 items:#("R32F", "A8R8G8B8", "R8G8B8", "X8R8G8B8")
	dropdownList ddlSShMapWidth "" pos:[112,415] width:108 height:24 items:#("512", "1024", "2048")
	label lbSShMapWidth "Width:" pos:[72,418] width:35 height:18
	dropdownList ddlSShMapHeigth "" pos:[111,443] width:108 height:24 items:#("512", "1024", "2048")
	label lbSShMapHeigth "Heigth:" pos:[71,446] width:35 height:18
	GroupBox grpLights "Lights" pos:[10,281] width:243 height:195
	
	
	
	local m_ObjSelected = undefined
	
	function ResetProps =
	(
		ExportRenderCB.checked = false
		VisibleRenderCB.checked = false
		LayerRenderCB.checked = false
		GeneratePhysicsCB.checked = false
		layerPhysicsCB.checked = false
		GenerateLogicCB.checked = false
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
				
				if(cbGenSShMap.checked) then
				(
					setUserProp Obj "generate_static_shadow_map" cbGenSShMap.checked
					setUserProp Obj "static_shadow_map_format_type" ddlSShMapFormat.selected
					setUserProp Obj "static_shadow_map_width" ddlSShMapWidth.selected
					setUserProp Obj "static_shadow_map_height" ddlSShMapHeigth.selected
				)
				else
				(
					setUserProp Obj "generate_static_shadow_map" "false"
					setUserProp Obj "static_shadow_map_format_type" "undefined" 
					setUserProp Obj "static_shadow_map_width" "undefined"
					setUserProp Obj "static_shadow_map_height" "undefined"
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
			
			--Unchecked all check boxs
			ResetProps()
			
			--Set Export Render Prop to Rollout
			if l_ExportRender != undefined then
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
)

