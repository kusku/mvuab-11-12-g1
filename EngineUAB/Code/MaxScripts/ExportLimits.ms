fileIn "HelperFunctions.ms"

struct LimitsHolder
(
	LimitsObjs,
	
	function LimitsObjExists lim = 
	(
		for i = 1 to LimitsObjs.count do
		(
			if LimitsObjs[i].LimitName == lim.LimitName then
			(
				return true
			)
		)
		
		return false
	),
	
	function AddLimitObj lim =
	(
		local exists = (LimitsObjExists lim)
		
		if exists == true then
		(
			return -1
		)
		
		append LimitsObjs lim
		
		return 0
	)
)

struct Limits
(
	LimitName,
	LimitGroup,
	LimitPosition,
	LimitYaw,
	LimitPitch,
	LimitRoll,
	LimitDimension
)

function CreateLimitsHolder = 
(
	local l_Holder = LimitsHolder LimitsObjs:#()
	
	return l_Holder
)

function CreateLimit = 
(
	local l_Limits = Limits LimitName:"" LimitGroup:"" LimitPosition:(point3 0 0 0) LimitDimension:(point3 0 0 0) LimitYaw:0 LimitPitch:0 LimitRoll:0
	
	return l_Limits
)

function ExportLimitsObjs sel FilePath =
(
	local stFile = FilePath
	
	stFile = FilePath + "\\limits.xml"
	
	format "Start Exporting Lmits Objects\n"
	
	local l_holder = CreateLimitsHolder()
	
	for obj in sel do
	(
		local l_LimitObj = (getUserProp obj "is_limit")
		
		if ( classof(obj) == Box AND l_LimitObj == true ) then
		(				
			local l_LimObj = CreateLimit()
			
			l_LimObj.LimitName = obj.name as string
			
			local limPos = obj.position
			limPos = RHTranslationToLH limPos
			l_LimObj.LimitPosition = limPos

			l_LimObj.LimitYaw = GetYaw obj.transform
			l_LimObj.LimitPitch = GetPitch obj.transform
			l_LimObj.LimitRoll = GetRoll obj.transform
			
			l_LimObj.LimitDimension = (point3 obj.width obj.height obj.length )
			
			l_LimObj.LimitGroup = (getUserProp obj "limit_group")
						
			l_holder.AddLimitObj l_LimObj
		)
	)
		
	local file = createfile stFile
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<limits_objs>\n" to:file
	
	local l_objs = l_holder.LimitsObjs
	
	format "Limit Count: %\n"  l_objs.count
	
	for i = 1 to l_objs.count do
	(
		format "\t<limit name=\"%\" dimension=\"% % %\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" group=\"%\" />\n" l_objs[i].LimitName l_objs[i].LimitDimension.x l_objs[i].LimitDimension.y l_objs[i].LimitDimension.z l_objs[i].LimitPosition.x l_objs[i].LimitPosition.y l_objs[i].LimitPosition.z l_objs[i].LimitYaw l_objs[i].LimitPitch l_objs[i].LimitRoll l_objs[i].LimitGroup to:file
	)
	
	format "</limits_objs>\n" to:file
	close file
	
	format "Finish Exporting Limits Objects\n"
)