# Protocolo de Programación #

## Global ##

**1.** Cualquier nombre asignado a una variable, clase, método, etc. el inicio de la segunda palabra en adelante tiene que ser mayúscula.

**Ejemplo**

```
void SetMetodo()
{
...
}

int miContador;

class CMiClase
{
...
};
```

**2.** Si se está asignando un numero **float** o se va a pasar por parámetro, siempre poner **f** al final del número.

**Ejemplo**

```
float pi = 3.14f;

CVector3* vect3 = new CVector3(10.0f, 0.0f, 1.5f);
```

**3.** Declarar las variables en líneas individuales. (Es mas facil para leer y encontrar)

**Ejemplo Correcto**

```
int x;
int y;
int z;
```

**Incorrecto**

```
int x, y, z;
```

**4.** Todas las funciones deben llevar una pequeña descripción donde se definen (Normalmente en el .h) Tienen que utilizar la siguiente estructura:
  * 

&lt;summary&gt;

 

&lt;/summary&gt;

**Inicio y fin de la descripción del método o clase
  ***

&lt;param name=”x”&gt;

 y 

&lt;/param&gt;

**Inicio de descripción de los parámetros del método. X define el nombre del parámetro
  ***

&lt;returns&gt;

 y 

&lt;/returns&gt;

**Inicio y fin de la descripción de que devuelve el método
  * Se pueden conseguir más tags para Visual C++ en este link: http://msdn.microsoft.com/en-us/library/ms177227.aspx**


**Ejemplo**

```
/*<summary>
 * Calcula la suma de dos números.
 * </summary>
 *<param name=”a”>Primer número a sumar</param>
 *<param name=”b”>Segundo número a sumar</param>
 *<returns>Retorna un int con la suma de los números</returns>
 *</summary> */
int Suma(int a, int b);
```

**Excepciones:**

  * Constructor sin parámetros
  * Get
  * Set
  * Update
  * Render

**5.** Para todas las variables que se modifican dentro de un método (Parámetros de salida) se les agrega un _al final._

**Ejemplo**

```
void SetNewTime(const CTime& oldTime, CTime& newTime_)
{
…
}
```

**6.** Dejar un espacio después de **(** y antes de **)**

**Ejemplo**

```
if ( i == 0 )
{
…
}

for ( int i = 0; i < 10; i++ )
{
…
}
```


## Nombres de Objetos ##

**1. Nombre de Clases:** CNombreClase _(Se agrega una **C** al principio del nombre)_

**Ejemplo**

```
class CProtocolo
{
}
```

**2. Nombre de Estructura:** SNombreEstructura _(Se agrega una **S** al principio del nombre)_

**Ejemplo**

```
struct SVector3
{
}
```

**3. Nombre de Enumerador:** ENombreEnumerador _(Se agrega una **E** al principio del nombre)_

**Ejemplo**

```
enum ETipo
{
};
```

## Nombres de Variables ##

**1. Variable miembra de clase:** m\_MiVariable **(Se agrega un prefijo _m_** para indicar que es miembra de la clase)

**Ejemplo**

```
class CMiClase
{
private:
   int m_Contador;
public:
...
};
```

**2. Variables Globales:** g\_VariableGlobal **(Se agrega un prefijo _g_** para indicar que es una variable global)

**Ejemplo**

```
#include <vector>

vector<int> g_MiVector;

void main()
{
...
}
```

**3. Variables Constantes:** MI\_CONSTANTE _(Todo en mayúscula y se separan las palabras con guion bajo)_

**Ejemplo**

```

const float MI_PI = 3.14f;

```

## Estructura de Código ##

**1.** Se tiene que usar **tab** para indentar _(Por Default Visual Studio tiene esta opción)_

**2.** Todos los if y else tiene que tener **{}**

**Ejemplo Correcto**

```
if(1)
{
...
}
else
{
...
}
```

**Incorrecto**
```
if(1)
 x++
else
 y--
```

**3.** Todos los corchetes **{** tienen que ir abajo de la declaración

**Ejemplo Correcto**

```
if(1)
{
}

void MiFuncion()
{
}

class CMiClase
{
};
```

**Incorrecto**

```
if(1) {
}

void MiFuncion(){
}
```