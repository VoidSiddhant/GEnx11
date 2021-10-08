#ifndef UTIL_H
#define UTIL_H

#define WINDOWS_LEAN_AND_MEAN
#include<windows.h>
#include<atlbase.h>
#define SCREEN_HEIGHT	640
#define SCREEN_WIDTH	800
#define QUIT_ENGINE quit_engine




namespace CoreSystems
{
	#define ReleaseCOM(x) if(x) x->Release()
};


#endif