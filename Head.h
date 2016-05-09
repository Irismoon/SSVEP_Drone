#pragma once
#pragma comment(lib, "legacy_stdio_definitions.lib")

#include <queue>
#include <math.h>
#include <stdio.h>
#include <windows.h>
#include <gl\gl.h>			
#include <gl\glu.h>		
#include <glaux.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <conio.h>
#include <iomanip>		
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include "escapi.h"

#include "Config.h"
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")
#pragma comment(lib,"GLaux.lib")
#pragma comment(lib,"winmm.lib")

#if (_MSC_VER>=1600)
#define __STDC_UTF_16__
#endif
#include "engine.h"
#pragma comment(lib,"libeng.lib")
#pragma comment(lib,"libmat.lib")
#pragma comment(lib,"libmx.lib")

#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4305)
using namespace std;
