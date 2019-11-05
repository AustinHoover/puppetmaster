

#ifndef PUPPETLIB_H
#define PUPPETLIB_H


#include <windows.h>

#ifdef BUILDING_PUPPETMASTERLIB_DLL
#define PUPPETMASTERLIB_DLL __declspec(dllexport)
#else
#define PUPPETMASTERLIB_DLL __declspec(dllimport)
#endif


extern void PUPPETMASTERLIB_DLL puppetmaster(int argc, TCHAR * argv[], int current_key);




#endif
