/*******************************************************

Name: HelperFunctions.ms

Use: Helper Functions to use when exporting a mesh

Last Update: 12/15/2011

********************************************************/

/**************************************
    Functions
**************************************/
function RoundDownFloat num =
(
	local decimalPlaces = 4
	
	return floor(((pow 10 decimalPlaces) *  num))/(pow 10 decimalPlaces)
)

function RoundUpFloat num =
(
	local decimalPlaces = 4
	
	return ceil(((pow 10 decimalPlaces) * num))/(pow 10 decimalPlaces)
)

function RHTranslationToLH translation =
(
	return point3 translation.x translation.z translation.y
)

function GetYaw transform =
(
	return (transform as eulerangles).z
)

function GetPitch transform =
(
	return (transform as eulerangles).x
)

function GetRoll transform =
(
	return (transform as eulerangles).y
)

function ResetVertexToOriginal Obj Vtx=
(
	-- Set to local coordenates
	local inverseTransform = Inverse(Obj.transform)
	Vtx = Vtx * inverseTransform
	
	-- Set from RH to LH
	Vtx = RHTranslationToLH Vtx
)

function WriteStringToFile str file = 
(
	--Write String and String Length to file
	
	WriteShort file (str.count + 1) #unsigned
	WriteString file str
)
