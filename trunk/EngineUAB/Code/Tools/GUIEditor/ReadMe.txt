================================================================================
    BIBLIOTECA MICROSOFT FOUNDATION CLASS: Informaci�n general del 
    proyectoGUIEditor
================================================================================

El Asistente para aplicaciones ha creado esta aplicaci�n GUIEditor.
Esta aplicaci�n no s�lo muestra los fundamentos del uso de Microsoft Foundation 
Classes sino que tambi�n supone un punto de partida para el desarrollo de la 
propia aplicaci�n.

Este archivo incluye un resumen acerca del contenido de los archivos que
constituyen su aplicaci�n GUIEditor.

GUIEditor.vcxproj
    �ste es el archivo de proyecto principal para los proyectos de VC++ 
    generados mediante un asistente para aplicaciones. 
    Contiene informaci�n acerca de la versi�n de Visual C++ con la que se 
    gener� el archivo, as� como informaci�n acerca de las plataformas, 
    configuraciones y caracter�sticas del proyecto seleccionadas en el
    Asistente para aplicaciones.

GUIEditor.vcxproj.filters
    �ste es el archivo de filtros para los proyectos de VC++ generados mediante 
    un asistente para aplicaciones. 
    Contiene informaci�n acerca de la asociaci�n entre los archivos de un 
    proyecto 
y los filtros. Esta asociaci�n se usa en el IDE para mostrar la 
    agrupaci�n de archivos con extensiones similares bajo un nodo espec�fico 
    (por ejemplo, los archivos ".cpp" se asocian con el filtro "Archivos de 
    c�digo fuente").

GUIEditor.h
    �ste es el archivo de encabezado principal para la aplicaci�n. Incluye otros
    encabezados espec�ficos del proyecto y declara la clase de aplicaci�n
    CGUIEditorApp.

GUIEditor.cpp
    �ste es el archivo fuente principal de la aplicaci�n contenido en la clase 
    de la aplicaci�n CGUIEditorApp.

GUIEditor.rc
    �sta es una lista de todos los recursos de Microsoft Windows que utiliza el 
    programa. Incluye los iconos, mapas de bits y cursores almacenados en el 
    subdirectorio RES. Este archivo puede editarse directamente en Microsoft
    Visual C++. Los recursos de su proyecto se encuentran en 3082.

res\GUIEditor.ico
    �ste es un archivo de icono, que se utiliza como el icono de la aplicaci�n.
    Este icono est� incluido en el archivo principal de recursos 
    GUIEditor.rc.

res\GUIEditor.rc2
    Este archivo incluye recursos no editados por Microsoft 
    Visual C++. Debe colocar todos los recursos que no se pueden editar en 
    los editores de recursos de este archivo.

/////////////////////////////////////////////////////////////////////////////

Para la ventana de marco principal:
    El proyecto incluye una interfaz MFC est�ndar.

MainFrm.h, MainFrm.cpp
    Estos archivos contienen la clase de marco CMainFrame, que se 
    deriva de
    CMDIFrameWnd y que controla todas las caracter�sticas del marco MDI.

/////////////////////////////////////////////////////////////////////////////

Para la ventana de marco secundaria:

ChildFrm.h, ChildFrm.cpp
    Estos archivos definen e implementan la clase CChildFrame, 
    que permite ventanas secundarias en una aplicaci�n MDI.

/////////////////////////////////////////////////////////////////////////////

El Asistente para aplicaciones crea un tipo de documento y una sola vista:

GUIEditorDoc.h, GUIEditorDoc.cpp � el documento
    Estos archivos contienen su clase CGUIEditorDoc. Edite estos archivos 
    para agregar los datos del documento especial y para guardar y cargar el 
    archivo (mediante CGUIEditorDoc::Serialize).

GUIEditorView.h, GUIEditorView.cpp � la vista del documento
    Estos archivos contienen su clase CGUIEditorView.
    Los objetos CGUIEditorView se utilizan para ver objetos 
    CGUIEditorDoc.

res\GUIEditorDoc.ico
    �ste es un archivo de icono, que se utiliza como el icono para las ventanas 
    secundarias MDI para la clase CGUIEditorDoc. Este icono lo incluye el 
    archivo principal de recursos GUIEditor.rc.




/////////////////////////////////////////////////////////////////////////////

Otras caracter�sticas:

Controles ActiveX
    La aplicaci�n es compatible con el uso de los controles ActiveX.

Compatibilidad con la impresi�n y la vista preliminar
    Este Asistente para aplicaciones genera el c�digo necesario para controlar 
    los comandos de impresi�n, la configuraci�n de la impresora y la vista 
    preliminar mediante llamadas a funciones miembro de la clase CView de la 
    biblioteca MFC.

/////////////////////////////////////////////////////////////////////////////

Otros archivos est�ndar:

StdAfx.h, StdAfx.cpp
    Estos archivos se utilizan para generar un archivo de encabezado 
    precompilado (PCH) denominado GUIEditor.pch y un archivo de 
    tipos precompilado llamado StdAfx.obj.

Resource.h
    �ste es el archivo de encabezado est�ndar, que define nuevos identificadores 
    de recurso.
    Microsoft Visual C++ lee y actualiza este archivo.

GUIEditor.manifest
    Windows XP utiliza los archivos de manifiesto de la aplicaci�n para describir 
    la dependencia de una aplicaci�n en versiones espec�ficas de ensamblados 
    simult�neos. El cargador utiliza esta informaci�n para cargar el ensamblado 
    adecuado desde la cach� de ensamblados o desde el directorio privado de la 
    aplicaci�n. El manifiesto de la aplicaci�n puede incluirse para su 
    redistribuci�n como un archivo .manifest instalado en la misma carpeta que 
    el ejecutable de la aplicaci�n o puede incluirse en el ejecutable en forma 
    de recurso. 
/////////////////////////////////////////////////////////////////////////////

Otras notas:

El Asistente para aplicaciones utiliza "TODO:" para indicar partes del c�digo 
fuente que deber�a agregar o personalizar.

Si su aplicaci�n utiliza MFC en un archivo DLL compartido, tendr� que copiar
los archivos DLL MFC. Si el idioma de su aplicaci�n es distinto al idioma actual
del sistema operativo, tambi�n tendr� que copiar los recursos localizados 
correspondientes MFC100XXX.DLL. Para obtener mas informaci�n sobre ambos temas, 
consulte la secci�n que trata sobre la copia de aplicaciones de Visual C++ en la 
documentaci�n de MSDN.

/////////////////////////////////////////////////////////////////////////////
