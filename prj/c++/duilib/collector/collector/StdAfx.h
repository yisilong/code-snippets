#pragma once

#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "resource.h"

#include "../common/rapidxml/rapidxml.hpp"

#include "../common/sqlite3/sqlite3.h"
#pragma comment(lib,"../bin/sqlite3.lib")

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

#ifdef _UNICODE
	typedef std::wfstream tfstream;
	typedef std::wstring tstring;
#else
	typedef std::fstream tfstream;
	typedef std::string tstring;
#endif