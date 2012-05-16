fileIn "HelperFunctions.ms"

struct WayPointHolder
(
	WayPointGroups,
	
	function GetGroup groupName =
	(
		for i = 1 to WayPointGroups.count do
		(
			if WayPointGroups[i].GroupName == groupName then
			(
				return WayPointGroups[i]
			)
		)
		
		return undefined
	),
	
	function AddWayPoint groupName wp =
	(
		local l_group = GetGroup groupName
		
		if(l_group == undefined) then
		(
			l_group = CreateWayPointGroup()
			
			append WayPointGroups l_group

			l_group.GroupName = groupName
		)
		
		l_group.AddWayPoint wp
		
		return 0
	),
	
	function AddGroup wpGroup =
	(
		local exists = (GroupExists wpGroup.GroupName)
		
		if exists == true then
		(
			return -1
		)
		
		append WayPointGroups wpGroup
		
		return 0
	),
	
	function GroupExists gName = 
	(
		for i = 1 to WayPointGroups.count do
		(
			if WayPointGroups[i].GroupName == gName then
			(
				return true
			)
		)
		
		return false
	)
)

struct WayPointGroup
(
	GroupName,
	WayPoints,
	
	function AddWayPoint pnt = 
	(
		append WayPoints pnt
	)
)

struct WayPoint
(
	WPName,
	Brothers,
	Pos
)

function CreateWayPointHolder = 
(
	local l_Holder = WayPointHolder WayPointGroups:#()
	
	return l_Holder
)

function CreateWayPointGroup = 
(
	local l_Group = WayPointGroup GroupName:"" WayPoints:#()
	
	return l_Group
)

function CreateWayPoint = 
(
	local l_WayPoint = WayPoint WPName:"" Brothers:#() Pos:(point3 0 0 0)
	
	return l_WayPoint
)

function ExportWayPoints FilePath =
(
	local stFile = FilePath
	
	stFile = FilePath + "\\waypoints.xml"
	
	format "Start Exporting Waypoints"
	
	local l_holder = CreateWayPointHolder()
	
	for pnt in Helpers do
	(
		local l_IsWP = (getUserProp pnt "is_waypoint")
		
		if ( classof(pnt) == Point AND l_IsWP == true ) then
		(
			
			local l_WPGroup = (getUserProp pnt "waypoint_group")
			
			if(l_WPGroup == undefined) then
			(
				continue
			)
			
			local l_wp = CreateWayPoint()
			local l_bros = getUserProp pnt "waypoint_brothers"
			
			l_wp.WPName = pnt.Name
			l_wp.Brothers = FromStringToArray l_bros
			l_wp.Pos = RHTranslationToLH pnt.Position
			
			l_holder.AddWayPoint l_WPGroup l_wp
		)
	)
		
	local file = createfile stFile
	format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
	format "<waypoints>\n" to:file
	
	local l_groups = l_holder.WayPointGroups;
	
	for i = 1 to l_groups.count do
	(
		format "\t<group name=\"%\">\n" l_groups[i].GroupName to:file
		
		local l_points = l_groups[i].WayPoints
		
		for j = 1 to l_points.count do
		(
			format "\t\t<waypoint name=\"%\" position=\"% % %\">\n" l_points[j].WPName l_points[j].Pos.x l_points[j].Pos.y l_points[j].Pos.z to:file
			
			local l_bros = l_points[j].Brothers
			
			for x = 1 to l_Bros.count do
			(
				format "\t\t\t<Brother name=\"%\" />\n" l_Bros[x] to:file
			)
			
			format "\t\t</waypoint>\n" to:file
		)
		
		format "\t</group>\n" to:file
	)
	
	format "</waypoints>\n" to:file
	close file
	
	format "Finish Exporting Waypoints"
)