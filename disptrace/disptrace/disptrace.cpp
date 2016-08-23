// disptrace.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "..\..\jytrace.h"

void __stdcall ETWTrace(wchar_t *szMod, wchar_t *szFile, wchar_t *szFn, wchar_t *szKey, wchar_t *szVal)
{
    EventWriteSimple(szMod, szFile, szFn, szKey, szVal);
}