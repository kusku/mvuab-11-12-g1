function AddAttributeToObject obj =
(
	BocModifier = EmptyModifier()
	BocModifier.Name = "Boc Game Properties"
	addModifier obj BocModifier
	
	BocAttributesCA = attributes BocAttribute
	(
		parameters PRM_BocAttributes rollout:RLT_BocAttributes
		(
			VisibleInGame type:#boolean ui:cbVisibleInGame default:true
			UseObjName type:#boolean ui:gbUseObjName default:true
			UserFileName type:#string ui:tFileName
		)
		
		rollout RLT_BocAttributes "Boc Object Game Properties" width:162 height:206
		(
			checkbox cbVisibleInGame "Visible in Game" pos:[13,7] width:118 height:18 checked:true
			GroupBox gbFileExport "File Export Name" pos:[2,115] width:154 height:78
			checkbox gbUseObjName "Use Object Name" pos:[10,132] width:117 height:19 checked:true
			edittext tFileName "" pos:[42,157] width:109 height:18 enabled:true
			label lFileName "Name:" pos:[8,159] width:29 height:17
		)
	)
	
	custattributes.add obj.modifiers["Boc Game Properties"] BocAttributesCA baseobject:true
)

MacroScript BocAddModifierAll category:"Boc Tools - Attributes" buttonText:"Set Attribute to All" tooltip:"Set the custom Boc Attribute to all objects on scene"
(
	select $*
	
	for obj in $ do -- Loop over currently selected objects
	(
		AddAttributeToObject obj
	)
)

MacroScript BocAddModifierSelected category:"Boc Tools - Attributes" buttonText:"Set Attribute Selected" tooltip:"Set the custom Boc Attribute to selected object"
(
	if(selection.count < 1) then
	(
		messagebox "You must have at least one object selected!"
	)
	else
	(
		AddAttributeToObject selection[1]
	)
)