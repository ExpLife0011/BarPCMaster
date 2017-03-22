// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#include <atlbase.h>
#include <atlstr.h>

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <vector>
#include <list>
#include <map>

// DuiLib
#include "UIlib.h"
#include "Utils/Utils.h"
using namespace DuiLib;

// EasyLogging++
#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_UNICODE
#define ELPP_DEFAULT_LOG_FILE "Logs\\BarPCMaster%datetime{%Y%M%d}.log"
#include "easylogging++.h"
