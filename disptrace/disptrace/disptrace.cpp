// disptrace.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "..\..\jytrace.h"

void __stdcall ETWTrace(wchar_t *szLine)
{
    EventWriteSingle(szLine);
}