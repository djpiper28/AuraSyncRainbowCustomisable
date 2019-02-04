#pragma once

#include <Windows.h>

#undef _UNICODE

typedef void* MbLightControl;
typedef void* DramLightControl;


typedef DWORD(WINAPI* EnumerateMbControllerFunc)(MbLightControl handles[], DWORD size);
typedef DWORD(WINAPI* SetMbModeFunc) (MbLightControl handle, DWORD mode);
typedef DWORD(WINAPI* SetMbColorFunc) (MbLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetMbColorFunc) (MbLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetMbLedCountFunc)(MbLightControl handle);

typedef DWORD(WINAPI* EnumerateDramFunc)(DramLightControl handles[], DWORD size);
typedef DWORD(WINAPI* SetDramModeFunc) (DramLightControl handle, DWORD mode);
typedef DWORD(WINAPI* SetDramColorFunc) (DramLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetDramColorFunc) (DramLightControl handle, BYTE* color, DWORD size);
typedef DWORD(WINAPI* GetDramLedCountFunc)(DramLightControl handle);