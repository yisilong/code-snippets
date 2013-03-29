#pragma once

#include <windows.h>
#include <tchar.h>
#include "resource.h"

#include "../DuiLib/UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "../bin/DuiLib_ud.lib")
#   else
#       pragma comment(lib, "../bin/DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "../bin/DuiLib_u.lib")
#   else
#       pragma comment(lib, "../bin/DuiLib.lib")
#   endif
#endif
