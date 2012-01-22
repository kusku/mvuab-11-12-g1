/*******************************************************

Name: HelperFunctions.ms

Use: Helper Functions to use when exporting a mesh

Last Update: 12/15/2011

----------------------------------------------------------------

Notes:

The EulerAngles class provides a representation for orientation in 3D space using rotation angles in degrees about each axis.

Angles can be greater than 360 and so specify multiple revolutions.

Rotations follow the right-hand-rule. 
********************************************************/

/**************************************
    Functions
**************************************/

/* ----------------------------------------------------------------------------------- 
	RoundDownFloat:
	Return : Returns the nearest whole number less than or equal to number 
			with 4 decimals
----------------------------------------------------------------------------------- */
function RoundDownFloat num =
(
	local decimalPlaces = 4
	
	return floor(((pow 10 decimalPlaces) *  num))/(pow 10 decimalPlaces)
)

/* ----------------------------------------------------------------------------------- 
	RoundUpFloat:
	Return : Returns the nearest whole number greater than or equal to number
			with 4 decimals
----------------------------------------------------------------------------------- */
function RoundUpFloat num =
(
	local decimalPlaces = 4
	
	return ceil(((pow 10 decimalPlaces) * num))/(pow 10 decimalPlaces)
)

/* ----------------------------------------------------------------------------------- 
	RHTranslationToLH:
	Return : Returns a point3 with the translation from Right Hand to Left Hand
----------------------------------------------------------------------------------- */
function RHTranslationToLH translation =
(
	return point3 translation.x translation.z translation.y
)

/* ----------------------------------------------------------------------------------- 
	GetYaw:
	Return : Returns the Yaw angle
----------------------------------------------------------------------------------- */
function GetYaw transform =
(
	return (-1 * (transform as eulerangles).z)
)

/* ----------------------------------------------------------------------------------- 
	GetPitch:
	Return : Returns the Pitch angle
----------------------------------------------------------------------------------- */
function GetPitch transform =
(
	return (-1 * (transform as eulerangles).x)
)


/* ----------------------------------------------------------------------------------- 
	GetRoll:
	Return : Returns the Roll angle
----------------------------------------------------------------------------------- */
function GetRoll transform =
(
	return (-1 * (transform as eulerangles).y)
)

/* ----------------------------------------------------------------------------------- 
	ResetVertexToOriginal
	Return : Returns the transformation of a vertex form RH to LH of an object 
----------------------------------------------------------------------------------- */
function ResetVertexToOriginal Obj Vtx=
(
	-- Set to local coordenates
	local inverseTransform = Inverse(Obj.transform)
	Vtx = Vtx * inverseTransform
	
	-- Set from RH to LH
	Vtx = RHTranslationToLH Vtx
)

/* ----------------------------------------------------------------------------------- 
	ResetVertexToOriginal
	Return : Write String and String Length to file
----------------------------------------------------------------------------------- */
function WriteStringToFile str file = 
(
	WriteShort file (str.count + 1) #unsigned
	WriteString file str
)

/**************************************************/
global UABVtxsNormals=#()

function UABClearNormalsArray =
(
	while UABVtxsNormals.count>0 do
	(
		deleteItem UABVtxsNormals 1
	)
)

function IsSmoothingGroupEnabled IdGroup Value =
(
	local ValueMask=2^(IdGroup-1)
	return (bit.and Value ValueMask)==ValueMask
)

function UABGetVertexNormal obj IdVertex SmoothValue FaceNormal =
(
	local HasNormal=false
	local Normal=point3 0 0 0
	for b=1 to 32 do
	(
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then
		(
			Normal=Normal+UABVtxsNormals[IdVertex][b]
			HasNormal=true
		)
	)
	if HasNormal==false then
	(
		Normal=FaceNormal
	)
	
	return Normal
)

function UABCalcVertexsNormals obj =
(
	UABClearNormalsArray()
	local NumVtxs=getNumVerts obj

	for b=1 to NumVtxs do
	(
		UABVtxsNormals[b]=#()
		
		for t=1 to 32 do
		(
			UABVtxsNormals[b][t]=point3 0 0 0
		)
	)

	local NumFaces=getNumFaces obj
	local InvTransform=inverse obj.transform
	
	for IdFace=1 to NumFaces do
	(
		local IdxsFace=getFace obj IdFace
		local Vtx1=(getVert obj IdxsFace.x)*InvTransform
		
		local Vtx2=(getVert obj IdxsFace.y)*InvTransform
		local Vtx3=(getVert obj IdxsFace.z)*InvTransform
		
		local FaceNormal=getFaceNormal obj IdFace
		local SmoothValue=getFaceSmoothGroup obj IdFace
		
		for b=1 to 32 do
		(
			if((IsSmoothingGroupEnabled b SmoothValue)==true) then
			(
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] +
				FaceNormal
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] +
				FaceNormal
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] +
				FaceNormal
			)
		)
	)
)