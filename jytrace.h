//**********************************************************************`
//* This is an include file generated by Message Compiler.             *`
//*                                                                    *`
//* Copyright (c) Microsoft Corporation. All Rights Reserved.          *`
//**********************************************************************`
#pragma once
#include <wmistr.h>
#include <evntrace.h>
#include "evntprov.h"
//
//  Initial Defs
//
#if !defined(ETW_INLINE)
#define ETW_INLINE DECLSPEC_NOINLINE __inline
#endif

#if defined(__cplusplus)
extern "C" {
#endif

//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION
#if  !defined(McGenDebug)
#define McGenDebug(a,b)
#endif 


#if !defined(MCGEN_TRACE_CONTEXT_DEF)
#define MCGEN_TRACE_CONTEXT_DEF
typedef struct _MCGEN_TRACE_CONTEXT
{
    TRACEHANDLE            RegistrationHandle;
    TRACEHANDLE            Logger;
    ULONGLONG              MatchAnyKeyword;
    ULONGLONG              MatchAllKeyword;
    ULONG                  Flags;
    ULONG                  IsEnabled;
    UCHAR                  Level; 
    UCHAR                  Reserve;
    USHORT                 EnableBitsCount;
    PULONG                 EnableBitMask;
    const ULONGLONG*       EnableKeyWords;
    const UCHAR*           EnableLevel;
} MCGEN_TRACE_CONTEXT, *PMCGEN_TRACE_CONTEXT;
#endif

#if !defined(MCGEN_LEVEL_KEYWORD_ENABLED_DEF)
#define MCGEN_LEVEL_KEYWORD_ENABLED_DEF
FORCEINLINE
BOOLEAN
McGenLevelKeywordEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ UCHAR Level,
    _In_ ULONGLONG Keyword
    )
{
    //
    // Check if the event Level is lower than the level at which
    // the channel is enabled.
    // If the event Level is 0 or the channel is enabled at level 0,
    // all levels are enabled.
    //

    if ((Level <= EnableInfo->Level) || // This also covers the case of Level == 0.
        (EnableInfo->Level == 0)) {

        //
        // Check if Keyword is enabled
        //

        if ((Keyword == (ULONGLONG)0) ||
            ((Keyword & EnableInfo->MatchAnyKeyword) &&
             ((Keyword & EnableInfo->MatchAllKeyword) == EnableInfo->MatchAllKeyword))) {
            return TRUE;
        }
    }

    return FALSE;

}
#endif

#if !defined(MCGEN_EVENT_ENABLED_DEF)
#define MCGEN_EVENT_ENABLED_DEF
FORCEINLINE
BOOLEAN
McGenEventEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{

    return McGenLevelKeywordEnabled(EnableInfo, EventDescriptor->Level, EventDescriptor->Keyword);

}
#endif


//
// EnableCheckMacro
//
#ifndef MCGEN_ENABLE_CHECK
#define MCGEN_ENABLE_CHECK(Context, Descriptor) (Context.IsEnabled &&  McGenEventEnabled(&Context, &Descriptor))
#endif

#if !defined(MCGEN_CONTROL_CALLBACK)
#define MCGEN_CONTROL_CALLBACK

DECLSPEC_NOINLINE __inline
VOID
__stdcall
McGenControlCallbackV2(
    _In_ LPCGUID SourceId,
    _In_ ULONG ControlCode,
    _In_ UCHAR Level,
    _In_ ULONGLONG MatchAnyKeyword,
    _In_ ULONGLONG MatchAllKeyword,
    _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData,
    _Inout_opt_ PVOID CallbackContext
    )
/*++

Routine Description:

    This is the notification callback for Vista.

Arguments:

    SourceId - The GUID that identifies the session that enabled the provider. 

    ControlCode - The parameter indicates whether the provider 
                  is being enabled or disabled.

    Level - The level at which the event is enabled.

    MatchAnyKeyword - The bitmask of keywords that the provider uses to 
                      determine the category of events that it writes.

    MatchAllKeyword - This bitmask additionally restricts the category 
                      of events that the provider writes. 

    FilterData - The provider-defined data.

    CallbackContext - The context of the callback that is defined when the provider 
                      called EtwRegister to register itself.

Remarks:

    ETW calls this function to notify provider of enable/disable

--*/
{
    PMCGEN_TRACE_CONTEXT Ctx = (PMCGEN_TRACE_CONTEXT)CallbackContext;
    ULONG Ix;
#ifndef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    UNREFERENCED_PARAMETER(SourceId);
    UNREFERENCED_PARAMETER(FilterData);
#endif

    if (Ctx == NULL) {
        return;
    }

    switch (ControlCode) {

        case EVENT_CONTROL_CODE_ENABLE_PROVIDER:
            Ctx->Level = Level;
            Ctx->MatchAnyKeyword = MatchAnyKeyword;
            Ctx->MatchAllKeyword = MatchAllKeyword;
            Ctx->IsEnabled = EVENT_CONTROL_CODE_ENABLE_PROVIDER;

            for (Ix = 0; Ix < Ctx->EnableBitsCount; Ix += 1) {
                if (McGenLevelKeywordEnabled(Ctx, Ctx->EnableLevel[Ix], Ctx->EnableKeyWords[Ix]) != FALSE) {
                    Ctx->EnableBitMask[Ix >> 5] |= (1 << (Ix % 32));
                } else {
                    Ctx->EnableBitMask[Ix >> 5] &= ~(1 << (Ix % 32));
                }
            }
            break;

        case EVENT_CONTROL_CODE_DISABLE_PROVIDER:
            Ctx->IsEnabled = EVENT_CONTROL_CODE_DISABLE_PROVIDER;
            Ctx->Level = 0;
            Ctx->MatchAnyKeyword = 0;
            Ctx->MatchAllKeyword = 0;
            if (Ctx->EnableBitsCount > 0) {
                RtlZeroMemory(Ctx->EnableBitMask, (((Ctx->EnableBitsCount - 1) / 32) + 1) * sizeof(ULONG));
            }
            break;
 
        default:
            break;
    }

#ifdef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    //
    // Call user defined callback
    //
    MCGEN_PRIVATE_ENABLE_CALLBACK_V2(
        SourceId,
        ControlCode,
        Level,
        MatchAnyKeyword,
        MatchAllKeyword,
        FilterData,
        CallbackContext
        );
#endif
   
    return;
}

#endif
#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION
//+
// Provider JyTrace Event Count 24
//+
EXTERN_C __declspec(selectany) const GUID ProviderJyTrace = {0x277c604b, 0x1962, 0x47fa, {0x93, 0x07, 0x7c, 0xe0, 0x85, 0x5d, 0xfe, 0xa6}};

//
// Channel
//
#define ProviderJyTrace_CHANNEL_Application 0x9
//
// Keyword
//
#define KeywordFunctionEntryExit 0x1
#define KeywordGeneric 0x2

//
// Event Descriptors
//
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR FunctionEntry = {0x65, 0x0, 0x0, 0x5, 0x0, 0x0, 0x1};
#define FunctionEntry_value 0x65
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR FunctionExit = {0x66, 0x0, 0x0, 0x5, 0x0, 0x0, 0x1};
#define FunctionExit_value 0x66
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR InfoW = {0x67, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define InfoW_value 0x67
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR ErrorW = {0x68, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define ErrorW_value 0x68
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR InfoA = {0x69, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define InfoA_value 0x69
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR ErrorA = {0x6a, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define ErrorA_value 0x6a
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WideStrInfo = {0x6b, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define WideStrInfo_value 0x6b
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR WideStrError = {0x6c, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define WideStrError_value 0x6c
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR AnsiStrInfo = {0x6d, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define AnsiStrInfo_value 0x6d
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR AnsiStrError = {0x6e, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define AnsiStrError_value 0x6e
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR HresultError = {0x6f, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define HresultError_value 0x6f
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR LastError = {0x70, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define LastError_value 0x70
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR PointerInfo = {0x71, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define PointerInfo_value 0x71
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR PointerError = {0x72, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define PointerError_value 0x72
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR NumberInfo = {0x73, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define NumberInfo_value 0x73
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR NumberError = {0x74, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define NumberError_value 0x74
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR HexInfo = {0x75, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define HexInfo_value 0x75
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR HexError = {0x76, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define HexError_value 0x76
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR BoolInfo = {0x77, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define BoolInfo_value 0x77
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR BoolError = {0x78, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define BoolError_value 0x78
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR GuidInfo = {0x79, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define GuidInfo_value 0x79
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR GuidError = {0x7a, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define GuidError_value 0x7a
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR FloatInfo = {0x7b, 0x0, 0x0, 0x5, 0x0, 0x0, 0x2};
#define FloatInfo_value 0x7b
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR FloatError = {0x7c, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define FloatError_value 0x7c

//
// Note on Generate Code from Manifest Windows Vista and above
//
//Structures :  are handled as a size and pointer pairs. The macro for the event will have an extra 
//parameter for the size in bytes of the structure. Make sure that your structures have no extra padding.
//
//Strings: There are several cases that can be described in the manifest. For array of variable length 
//strings, the generated code will take the count of characters for the whole array as an input parameter. 
//
//SID No support for array of SIDs, the macro will take a pointer to the SID and use appropriate 
//GetLengthSid function to get the length.
//

//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Globals 
//


//
// Event Enablement Bits
//

EXTERN_C __declspec(selectany) DECLSPEC_CACHEALIGN ULONG JyTraceEnableBits[1];
EXTERN_C __declspec(selectany) const ULONGLONG JyTraceKeywords[3] = {0x1, 0x2, 0x2};
EXTERN_C __declspec(selectany) const UCHAR JyTraceLevels[3] = {5, 5, 2};
EXTERN_C __declspec(selectany) MCGEN_TRACE_CONTEXT ProviderJyTrace_Context = {0, 0, 0, 0, 0, 0, 0, 0, 3, JyTraceEnableBits, JyTraceKeywords, JyTraceLevels};

EXTERN_C __declspec(selectany) REGHANDLE JyTraceHandle = (REGHANDLE)0;

#if !defined(McGenEventRegisterUnregister)
#define McGenEventRegisterUnregister
#pragma warning(push)
#pragma warning(disable:6103)
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventRegister(
    _In_ LPCGUID ProviderId,
    _In_opt_ PENABLECALLBACK EnableCallback,
    _In_opt_ PVOID CallbackContext,
    _Inout_ PREGHANDLE RegHandle
    )
/*++

Routine Description:

    This function register the provider with ETW USER mode.

Arguments:
    ProviderId - Provider Id to be register with ETW.

    EnableCallback - Callback to be used.

    CallbackContext - Context for this provider.

    RegHandle - Pointer to Registration handle.

Remarks:

    If the handle != NULL will return ERROR_SUCCESS

--*/
{
    ULONG Error;


    if (*RegHandle) {
        //
        // already registered
        //
        return ERROR_SUCCESS;
    }

    Error = EventRegister( ProviderId, EnableCallback, CallbackContext, RegHandle); 

    return Error;
}
#pragma warning(pop)


DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventUnregister(_Inout_ PREGHANDLE RegHandle)
/*++

Routine Description:

    Unregister from ETW USER mode

Arguments:
            RegHandle this is the pointer to the provider context
Remarks:
            If Provider has not register RegHandle = NULL,
            return ERROR_SUCCESS
--*/
{
    ULONG Error;


    if(!(*RegHandle)) {
        //
        // Provider has not registerd
        //
        return ERROR_SUCCESS;
    }

    Error = EventUnregister(*RegHandle); 
    *RegHandle = (REGHANDLE)0;
    
    return Error;
}
#endif
//
// Register with ETW Vista +
//
#ifndef EventRegisterJyTrace
#define EventRegisterJyTrace() McGenEventRegister(&ProviderJyTrace, McGenControlCallbackV2, &ProviderJyTrace_Context, &JyTraceHandle) 
#endif

//
// UnRegister with ETW
//
#ifndef EventUnregisterJyTrace
#define EventUnregisterJyTrace() McGenEventUnregister(&JyTraceHandle) 
#endif

//
// Enablement check macro for FunctionEntry
//

#define EventEnabledFunctionEntry() ((JyTraceEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for FunctionEntry
//
#define EventWriteFunctionEntry(Module, File, Function)\
        EventEnabledFunctionEntry() ?\
        Template_zss(JyTraceHandle, &FunctionEntry, Module, File, Function)\
        : ERROR_SUCCESS\

//
// Enablement check macro for FunctionExit
//

#define EventEnabledFunctionExit() ((JyTraceEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for FunctionExit
//
#define EventWriteFunctionExit(Module, File, Function)\
        EventEnabledFunctionExit() ?\
        Template_zss(JyTraceHandle, &FunctionExit, Module, File, Function)\
        : ERROR_SUCCESS\

//
// Enablement check macro for InfoW
//

#define EventEnabledInfoW() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for InfoW
//
#define EventWriteInfoW(Module, File, Function, Value)\
        EventEnabledInfoW() ?\
        Template_zssz(JyTraceHandle, &InfoW, Module, File, Function, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for ErrorW
//

#define EventEnabledErrorW() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for ErrorW
//
#define EventWriteErrorW(Module, File, Function, Value)\
        EventEnabledErrorW() ?\
        Template_zssz(JyTraceHandle, &ErrorW, Module, File, Function, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for InfoA
//

#define EventEnabledInfoA() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for InfoA
//
#define EventWriteInfoA(Module, File, Function, Value)\
        EventEnabledInfoA() ?\
        Template_zsss(JyTraceHandle, &InfoA, Module, File, Function, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for ErrorA
//

#define EventEnabledErrorA() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for ErrorA
//
#define EventWriteErrorA(Module, File, Function, Value)\
        EventEnabledErrorA() ?\
        Template_zsss(JyTraceHandle, &ErrorA, Module, File, Function, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for WideStrInfo
//

#define EventEnabledWideStrInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for WideStrInfo
//
#define EventWriteWideStrInfo(Module, File, Function, Key, Value)\
        EventEnabledWideStrInfo() ?\
        Template_zsszz(JyTraceHandle, &WideStrInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for WideStrError
//

#define EventEnabledWideStrError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for WideStrError
//
#define EventWriteWideStrError(Module, File, Function, Key, Value)\
        EventEnabledWideStrError() ?\
        Template_zsszz(JyTraceHandle, &WideStrError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for AnsiStrInfo
//

#define EventEnabledAnsiStrInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for AnsiStrInfo
//
#define EventWriteAnsiStrInfo(Module, File, Function, Key, Value)\
        EventEnabledAnsiStrInfo() ?\
        Template_zsszs(JyTraceHandle, &AnsiStrInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for AnsiStrError
//

#define EventEnabledAnsiStrError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for AnsiStrError
//
#define EventWriteAnsiStrError(Module, File, Function, Key, Value)\
        EventEnabledAnsiStrError() ?\
        Template_zsszs(JyTraceHandle, &AnsiStrError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for HresultError
//

#define EventEnabledHresultError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for HresultError
//
#define EventWriteHresultError(Module, File, Function, Key, Value)\
        EventEnabledHresultError() ?\
        Template_zsszd(JyTraceHandle, &HresultError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for LastError
//

#define EventEnabledLastError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for LastError
//
#define EventWriteLastError(Module, File, Function, Key, Value)\
        EventEnabledLastError() ?\
        Template_zsszq(JyTraceHandle, &LastError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for PointerInfo
//

#define EventEnabledPointerInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for PointerInfo
//
#define EventWritePointerInfo(Module, File, Function, Key, Value)\
        EventEnabledPointerInfo() ?\
        Template_zsszp(JyTraceHandle, &PointerInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for PointerError
//

#define EventEnabledPointerError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for PointerError
//
#define EventWritePointerError(Module, File, Function, Key, Value)\
        EventEnabledPointerError() ?\
        Template_zsszp(JyTraceHandle, &PointerError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for NumberInfo
//

#define EventEnabledNumberInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for NumberInfo
//
#define EventWriteNumberInfo(Module, File, Function, Key, Value)\
        EventEnabledNumberInfo() ?\
        Template_zsszd(JyTraceHandle, &NumberInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for NumberError
//

#define EventEnabledNumberError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for NumberError
//
#define EventWriteNumberError(Module, File, Function, Key, Value)\
        EventEnabledNumberError() ?\
        Template_zsszd(JyTraceHandle, &NumberError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for HexInfo
//

#define EventEnabledHexInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for HexInfo
//
#define EventWriteHexInfo(Module, File, Function, Key, Value)\
        EventEnabledHexInfo() ?\
        Template_zsszq(JyTraceHandle, &HexInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for HexError
//

#define EventEnabledHexError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for HexError
//
#define EventWriteHexError(Module, File, Function, Key, Value)\
        EventEnabledHexError() ?\
        Template_zsszq(JyTraceHandle, &HexError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for BoolInfo
//

#define EventEnabledBoolInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for BoolInfo
//
#define EventWriteBoolInfo(Module, File, Function, Key, Value)\
        EventEnabledBoolInfo() ?\
        Template_zsszt(JyTraceHandle, &BoolInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for BoolError
//

#define EventEnabledBoolError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for BoolError
//
#define EventWriteBoolError(Module, File, Function, Key, Value)\
        EventEnabledBoolError() ?\
        Template_zsszt(JyTraceHandle, &BoolError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for GuidInfo
//

#define EventEnabledGuidInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for GuidInfo
//
#define EventWriteGuidInfo(Module, File, Function, Key, Value)\
        EventEnabledGuidInfo() ?\
        Template_zsszj(JyTraceHandle, &GuidInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for GuidError
//

#define EventEnabledGuidError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for GuidError
//
#define EventWriteGuidError(Module, File, Function, Key, Value)\
        EventEnabledGuidError() ?\
        Template_zsszj(JyTraceHandle, &GuidError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for FloatInfo
//

#define EventEnabledFloatInfo() ((JyTraceEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for FloatInfo
//
#define EventWriteFloatInfo(Module, File, Function, Key, Value)\
        EventEnabledFloatInfo() ?\
        Template_zsszf(JyTraceHandle, &FloatInfo, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

//
// Enablement check macro for FloatError
//

#define EventEnabledFloatError() ((JyTraceEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for FloatError
//
#define EventWriteFloatError(Module, File, Function, Key, Value)\
        EventEnabledFloatError() ?\
        Template_zsszf(JyTraceHandle, &FloatError, Module, File, Function, Key, Value)\
        : ERROR_SUCCESS\

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION


//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Template Functions 
//
//
//Template from manifest : TemplateFunctionEntryExit
//
#ifndef Template_zss_def
#define Template_zss_def
ETW_INLINE
ULONG
Template_zss(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2
    )
{
#define ARGUMENT_COUNT_zss 3

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zss];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zss, EventData);
}
#endif

//
//Template from manifest : TemplateSingleWide
//
#ifndef Template_zssz_def
#define Template_zssz_def
ETW_INLINE
ULONG
Template_zssz(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3
    )
{
#define ARGUMENT_COUNT_zssz 4

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zssz];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zssz, EventData);
}
#endif

//
//Template from manifest : TemplateSingleAnsi
//
#ifndef Template_zsss_def
#define Template_zsss_def
ETW_INLINE
ULONG
Template_zsss(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ LPCSTR  _Arg3
    )
{
#define ARGUMENT_COUNT_zsss 4

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsss];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : "NULL",
                        (_Arg3 != NULL) ? (ULONG)((strlen(_Arg3) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsss, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValueWide
//
#ifndef Template_zsszz_def
#define Template_zsszz_def
ETW_INLINE
ULONG
Template_zsszz(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_opt_ PCWSTR  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszz 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszz];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], 
                        (_Arg4 != NULL) ? _Arg4 : L"NULL",
                        (_Arg4 != NULL) ? (ULONG)((wcslen(_Arg4) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszz, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValueAnsi
//
#ifndef Template_zsszs_def
#define Template_zsszs_def
ETW_INLINE
ULONG
Template_zsszs(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_opt_ LPCSTR  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszs 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszs];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], 
                        (_Arg4 != NULL) ? _Arg4 : "NULL",
                        (_Arg4 != NULL) ? (ULONG)((strlen(_Arg4) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszs, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValueHresult
//
#ifndef Template_zsszd_def
#define Template_zsszd_def
ETW_INLINE
ULONG
Template_zsszd(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_ const signed int  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszd 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszd];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], &_Arg4, sizeof(const signed int)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszd, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValueLastError
//
#ifndef Template_zsszq_def
#define Template_zsszq_def
ETW_INLINE
ULONG
Template_zsszq(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_ const unsigned int  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszq 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszq];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], &_Arg4, sizeof(const unsigned int)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszq, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValuePtr
//
#ifndef Template_zsszp_def
#define Template_zsszp_def
ETW_INLINE
ULONG
Template_zsszp(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_opt_ const void *  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszp 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszp];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], &_Arg4, sizeof(PVOID)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszp, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValueBool
//
#ifndef Template_zsszt_def
#define Template_zsszt_def
ETW_INLINE
ULONG
Template_zsszt(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_ const BOOL  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszt 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszt];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], &_Arg4, sizeof(const BOOL)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszt, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValueGuid
//
#ifndef Template_zsszj_def
#define Template_zsszj_def
ETW_INLINE
ULONG
Template_zsszj(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_ LPCGUID  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszj 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszj];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], _Arg4, sizeof(GUID)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszj, EventData);
}
#endif

//
//Template from manifest : TemplateKeyValueFloat
//
#ifndef Template_zsszf_def
#define Template_zsszf_def
ETW_INLINE
ULONG
Template_zsszf(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ PCWSTR  _Arg0,
    _In_opt_ LPCSTR  _Arg1,
    _In_opt_ LPCSTR  _Arg2,
    _In_opt_ PCWSTR  _Arg3,
    _In_ const float  _Arg4
    )
{
#define ARGUMENT_COUNT_zsszf 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_zsszf];

    EventDataDescCreate(&EventData[0], 
                        (_Arg0 != NULL) ? _Arg0 : L"NULL",
                        (_Arg0 != NULL) ? (ULONG)((wcslen(_Arg0) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[1], 
                        (_Arg1 != NULL) ? _Arg1 : "NULL",
                        (_Arg1 != NULL) ? (ULONG)((strlen(_Arg1) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[2], 
                        (_Arg2 != NULL) ? _Arg2 : "NULL",
                        (_Arg2 != NULL) ? (ULONG)((strlen(_Arg2) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], 
                        (_Arg3 != NULL) ? _Arg3 : L"NULL",
                        (_Arg3 != NULL) ? (ULONG)((wcslen(_Arg3) + 1) * sizeof(WCHAR)) : (ULONG)sizeof(L"NULL"));

    EventDataDescCreate(&EventData[4], &_Arg4, sizeof(const float)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_zsszf, EventData);
}
#endif

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION

#if defined(__cplusplus)
};
#endif

#define MSG_level_Error                      0x50000002L
#define MSG_level_Verbose                    0x50000005L
#define MSG_channel_Application              0x90000001L
#define MSG_jenkins_event_101_message        0xB0000065L
#define MSG_jenkins_event_102_message        0xB0000066L
#define MSG_jenkins_event_119_message        0xB0000067L
#define MSG_jenkins_event_120_message        0xB0000068L
#define MSG_jenkins_event_121_message        0xB0000069L
#define MSG_jenkins_event_122_message        0xB000006AL
#define MSG_jenkins_event_103_message        0xB000006BL
#define MSG_jenkins_event_104_message        0xB000006DL
#define MSG_jenkins_event_105_message        0xB000006FL
#define MSG_jenkins_event_106_message        0xB0000070L
#define MSG_jenkins_event_107_message        0xB0000071L
#define MSG_jenkins_event_110_message        0xB0000073L
#define MSG_jenkins_event_111_message        0xB0000075L
#define MSG_jenkins_event_112_message        0xB0000077L
#define MSG_jenkins_event_113_message        0xB0000079L
#define MSG_jenkins_event_123_message        0xB000007BL
#define MSG_jenkins_event_124_message        0xB000007CL
