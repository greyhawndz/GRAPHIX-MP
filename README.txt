Hi Sir, We used an external library for loading Image files as textures in opengl
Inside the Project Folder, you will find the files "libSOIL.a" and "SOIL.h"
In order to install them in Dev-C, you must go to Tools->Compiler Options->Directories->Libraries-> <directory where libSOIL.a is found>
also, go to Tools->Compiler Options->Directories->Libraries-> <directory where SOIL.h is found>
After that, you must link the library by going to Project->Project Options ->Paramaeter->Linker -> <Directory where libSOIL.a is found>
(libSOIL.a MUST be linked first before linking the opengl library)