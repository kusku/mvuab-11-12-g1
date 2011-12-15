function ConvertToString Vtx =
(
	local l_Text = stringstream ""
	local l_Value=""
	for i=1 to Vtx.count do
	(
		format "%," Vtx[i] to:l_Text
	)
	
	return (l_Text as string)
)

function DotNetAddVtx Vtxs VtxsDotNet Vtx =
(
	local l_VtxStr = ConvertToString Vtx
	local l_Idx = -1
	
	if ( VtxsDotNet.ContainsKey l_VtxStr) then
	(
		l_Idx = VtxsDotNet.Item[ l_VtxStr ];
	)
	else
	(
		l_Idx = Vtxs.Count
		VtxsDotNet.Add l_VtxStr l_Idx
		
		append Vtxs Vtx
	)
	
	return l_Idx
)