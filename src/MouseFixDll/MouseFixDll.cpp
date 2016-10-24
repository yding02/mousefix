// MouseFixDll

#define _WIN32_WINNT 0x400
#include <windows.h>
#include <stdio.h>

#include "MouseFixDll.h"



// Shared DATA
#pragma data_seg(".SHARDATA")
static HHOOK hHook = NULL;
static DWORD lastTime = 0;
#pragma data_seg()


#define THRESHOLD 45
#define UPTHRESHOLD 100

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}


bool SetHook(HHOOK hHookNew) {
  hHook = hHookNew;
  return true;
}


LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  bool block = false;
  //if (nCode < 0) ...
  if (nCode == HC_ACTION) {
    PMSLLHOOKSTRUCT pMsllHookStruct = (PMSLLHOOKSTRUCT)lParam;
    DWORD currentTime = pMsllHookStruct->time;
    DWORD elapsedTime = currentTime - lastTime;

    if (wParam == WM_LBUTTONDOWN) {
      if (elapsedTime < THRESHOLD) {
        //char buffer[1024]; sprintf(buffer, "Left button down @ %d, was %d = %d", (int)currentTime, (int)lastTime, (int)elapsedTime); MessageBox(NULL, buffer, "MouseFixDll", MB_OK);
        block = true;
      } else {
        //lastTime = currentTime;
      }
    } else if (wParam == WM_LBUTTONUP) {
		if (elapsedTime < UPTHRESHOLD) {
			//char buffer[1024]; sprintf(buffer, "Left button down @ %d, was %d = %d", (int)currentTime, (int)lastTime, (int)elapsedTime); MessageBox(NULL, buffer, "MouseFixDll", MB_OK);
			block = true;
		}
		else {
			lastTime = currentTime;
		}
    }
  }
  if (!block) {
    return CallNextHookEx(hHook, nCode, wParam, lParam);
  } else {
    return 1;
  }
}
