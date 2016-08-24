// disptrace-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>

typedef void(__stdcall *FnTrace)(wchar_t*);

int main()
{
    HMODULE hLib = LoadLibrary(L"disptrace.dll");

    if (hLib) {
        FnTrace pfnTrace = (FnTrace)GetProcAddress(hLib, "ETWTrace");

        if (pfnTrace) {
            pfnTrace(L"Hello world!");
        }

        FreeLibrary(hLib);
    }

    return 0;
}

