#include "Toolkit.h"

//------------------------------------------------------------

void ReadStr( FILE* f, char* string )
{
	do
	{
		fgets( string, 255, f );
	}
	while ( ( string[0] == '/' ) || ( string[0] == '\n' ) );
}