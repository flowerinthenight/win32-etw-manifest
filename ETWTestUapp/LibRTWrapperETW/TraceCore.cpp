#include "pch.h"
#include "TraceCore.h"

using namespace LibRTWrapperETW;
using namespace Platform;

#include "jytrace.h"

TraceCore^ TraceCore::m_Instance = nullptr;

TraceCore::TraceCore()
{
	EventRegisterJyTrace();
}

TraceCore::~TraceCore()
{
	EventUnregisterJyTrace();
}

void TraceCore::Verbose(String^ mod, String^ file, String^ func, String^ m)
{
	EventWriteSimple(mod->Data(), file->Data(), func->Data(), L"Trace", m->Data());
}
