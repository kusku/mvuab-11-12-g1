function AddAttributeToObject obj =
(
	BocModifier = EmptyModifier()
	BocModifier.Name = "Boc Game Properties"
	addModifier obj BocModifier
	
	BocAttributesCA = attributes BocAttribute
	(
		parameters PRM_BocAttributes rollout:RLT_BocAttributes
		(
			VisibleInGame type:#boolean ui:CHK_VisibleInGame default:true
		)
		
		rollout RLT_BocAttributes "Boc Object Game Properties"
		(
			checkbox CHK_VisibleInGame "Visible in Game"
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