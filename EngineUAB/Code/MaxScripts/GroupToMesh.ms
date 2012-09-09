function grouptomeshC =
(
	for objPM in $ do
	(
			max create mode
			disableSceneRedraw()
			with undo off
			(
		 
				local sel = for o in objPM where superclassOf o == geometryClass collect o
				local meshO = sel[1]
				deleteItem sel (findItem sel meshO)
		 
				convertToMesh meshO
		 
				for o in sel do attach meshO o
		 
			)
			enableSceneRedraw()
		
	)
)

--grouptomeshC()

function ccungroup =
(
	for objPM in $ do
	(
		ungroup objPM 
	)
)

ccungroup()
