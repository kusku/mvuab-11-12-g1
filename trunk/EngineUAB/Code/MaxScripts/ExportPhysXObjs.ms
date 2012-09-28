fileIn "HelperFunctions.ms"

global PHYSX_OBJ_BOX	= 0x0001;

struct PhyxObjsHolder
(
	PhyxObjsGroup,
	
	function PhysXObjExists pxObj = 
	(
		for i = 1 to PhyxObjsGroup.count do
		(
			if PhyxObjsGroup[i].PhysXObjName == pxObj.PhysXObjName then
			(
				return true
			)
		)
		
		return false
	),
	
	function AddPhysXObj pxObj =
	(
		local exists = (PhysXObjExists pxObj)
		
		if exists == true then
		(
			return -1
		)
		
		append PhyxObjsGroup pxObj
		
		return 0
	)
)

struct PhysXObjBox
(
	PhysXObjName,
	PhysXObjPosition,
	PhysXObjYaw,
	PhysXObjPitch,
	PhysXObjRoll,
	PhysXObjType,
	PhysXObjDimension,
	PhysXObjGroup
)

function CreatePhyxObjsHolder = 
(
	local l_Holder = PhyxObjsHolder PhyxObjsGroup:#()
	
	return l_Holder
)

function CreatePhysXObjBox = 
(
	local l_PXBox = PhysXObjBox PhysXObjName:"" PhysXObjPosition:(point3 0 0 0) PhysXObjDimension:(point3 0 0 0) PhysXObjType:PHYSX_OBJ_BOX
	
	return l_PXBox
)

function ExportPhysXObjs sel FilePath =
(
	local stFile = FilePath
	
	stFile = FilePath + "\\physxobjs.xml"
	
	format "Start Exporting PhysX Objects\n"
	
	local l_holder = CreatePhyxObjsHolder()
	
	for obj in sel do
	(
		local l_IsPXObj = (getUserProp obj "is_physx_obj")
		
		if ( classof(obj) == Box AND l_IsPXObj == true ) then
		(				
			local l_pxObj = CreatePhysXObjBox()
			
			l_pxObj.PhysXObjName = obj.name as string
			
			local pxObjPos = obj.position
			pxObjPos = RHTranslationToLH pxObjPos
			l_pxObj.PhysXObjPosition = pxObjPos

			l_pxObj.PhysXObjYaw = GetYaw obj.transform
			l_pxObj.PhysXObjPitch = GetPitch obj.transform
			l_pxObj.PhysXObjRoll = GetRoll obj.transform
			
			l_pxObj.PhysXObjDimension = (point3 obj.width obj.length obj.height)
			
			l_pxObj.PhysXObjGroup = (getUserProp obj "physics_layer")
			
			l_holder.AddPhysXObj l_pxObj
		)
	)
		
	local file = createfile stFile
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<physx_objs>\n" to:file
	
	local l_objs = l_holder.PhyxObjsGroup;
	
	for i = 1 to l_objs.count do
	(
		format "\t<physx_obj name=\"%\" type=\"%\" dimension=\"% % %\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" group=\"%\" />\n" l_objs[i].PhysXObjName l_objs[i].PhysXObjType l_objs[i].PhysXObjDimension.x l_objs[i].PhysXObjDimension.y l_objs[i].PhysXObjDimension.z l_objs[i].PhysXObjPosition.x l_objs[i].PhysXObjPosition.y l_objs[i].PhysXObjPosition.z l_objs[i].PhysXObjYaw l_objs[i].PhysXObjPitch l_objs[i].PhysXObjRoll l_objs[i].PhysXObjGroup to:file
	)
	
	format "</physx_objs>\n" to:file
	close file
	
	format "Finish Exporting PhysX Objects\n"
)