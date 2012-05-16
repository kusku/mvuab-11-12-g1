=====================================
What you've got
=====================================
You have a TEST version (completely unsupported by IMVU) of IMVU exporters for 3DS Max.
These were compiled by Dan Jones (aka nerseus on IMVU) based on a modified version of the 
Cal3D Open Source code.

The exporters included are for exporting IMVU objects from:
	* 3ds Max 9 (32bit)
	* 3ds Max 2008 (32bit)
	* 3ds Max 2011 (32bit)
	* 3ds Max 2011 (64bit)
	* 3ds Max 2012 (32bit)
	* 3ds Max 2012 (64bit)

=====================================
Installing - Quick instructions
=====================================
Find the DLL and DLE that match your version of max and copy:
* The DLL will go in the "root" max install folder
* The DLE will go in the "plugins" subfolder of your max install

The "max install" folder is usually under c:\Program Files\Autodesk\...


=====================================
Installing - With Explanation
=====================================
You MUST run as Administrator when using Windows Vista or Windows 7.
This is because Windows requires Administrator rights to copy files to c:\Program Files\

This zip includes the following:
	* For Max 9 and Max 2008:
		* ner_cal3d.dll
		* ner_cal3d_max_exporter.dle
	* For Max 11 (32bit)
		* nerseusReleaseIMVUExporterMax2011x86.dll
		* nerseusReleaseIMVUExporterMax2011x86.dle
	* For Max 11 (64bit)
		* nerseusReleaseIMVUExporterMax2011x64.dll
		* nerseusReleaseIMVUExporterMax2011x64.dle
	* For Max 12 (32bit)
		* nerseusReleaseIMVUExporterMax2012x86.dll
		* nerseusReleaseIMVUExporterMax2012x86.dle
	* For Max 12 (64bit)
		* nerseusReleaseIMVUExporterMax2012x64.dll
		* nerseusReleaseIMVUExporterMax2012x64.dle
	
These files need to be copied to 3ds Max:
* The DLL will go in the "root" max install folder
* The DLE will go in the "plugins" subfolder of your max install

For example, to copy to Max 9:
	C:\Program Files\Autodesk\3ds Max 9\ner_cal3d.dll
	C:\Program Files\Autodesk\3ds Max 9\plugins\ner_cal3d_max_exporter.dle

For Max 2012 64bit:
	C:\Program Files\Autodesk\3ds Max 2012\nerseusReleaseIMVUExporterMax2012x64.dll
	C:\Program Files\Autodesk\3ds Max 2012\plugins\nerseusReleaseIMVUExporterMax2012x64.dle

=====================================
Troubleshooting
=====================================
Make sure ALL previous versions of the Cal3D exporters are uninstalled/deleted.
This generally involves removing two files - the DLL and the DLE.

If you receive any errors when Max is starting up, verify the version of the files
you copied with your version of max.

For all other troubleshooting and tips, please see the following thread on the IMVU forums:
http://www.imvu.com/catalog/modules.php?op=modload&name=phpbb2&file=viewtopic.php&t=213459
	

=====================================
Bonus
=====================================
All exporters now DEFAULT to the X** version of the files.
That is, when you've selected your export type (skeleton, mesh, etc), simply
type the filename. The extension will default to .XSF, .XMF, etc. instead of
the CSF, CMF, etc. counterparts. 


=====================================
Known Issues
=====================================
None, but untested for Morph exports in Max 2011, Max 2012


=====================================
What's Next
=====================================
Nothing planned. I'm open to suggestions. Contact me on IMVU via my avatar nerseus
or shoot me an email: djatarchon@hotmail.com


=====================================
Update History
=====================================
04/30/2008 - DJ created initial version
05/01/2008 - DJ updated. Vertex colors now working.
05/06/2008 - DJ Bug fix. Exporting mesh was crashing on step 3 of export wizard if no vert colors were painted.
07/02/2008 - DJ Used new source code as base. Renamed DLLs.
06/21/2011 - DJ added files for Max 2011 (32bit and 64bit) and Max 2012 (64bit)
08/09/2011 - DJ added files fro Max 2012 (32bit) - considered final package until a new version of max comes out