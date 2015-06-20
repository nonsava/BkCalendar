#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN // Windows ヘッダーから使用されていない部分を除外します。
#include <windows.h>
#include <windowsx.h>

#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <list>
#include <map>
#include <sstream>

using namespace std;

typedef std::basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR>> tstring;
