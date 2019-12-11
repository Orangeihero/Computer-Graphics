#ifndef	_STANFORD_BUNNY_H_
#define	_STANFORD_BUNNY_H_

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef MACOS
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif


void DrawBunny();


#endif	//_STANFORD_BUNNY_H_