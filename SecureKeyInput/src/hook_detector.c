#include <windows.h>
#include <stdio.h>
#include "../include/hook_detector.h"
#include "../include/logger.h"

static HHOOK g_testHook = NULL;

static LRESULT CALLBACK DummyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void initialize_hook_detector()
{
    g_testHook = SetWindowsHookEx(WH_KEYBOARD_LL, DummyHookProc, NULL, 0);

    if (g_testHook == NULL)
    {
        // 후킹 설치 실패 → 이미 다른 후킹 존재 가능성
        write_log("[WARNING] WH_KEYBOARD_LL 훅이 설치되지 않음. 이미 후킹된 것으로 의심됨.");
    }
    else
    {
        write_log("[INFO] WH_KEYBOARD_LL 훅 설치 성공. 현재 후킹 없음으로 판단.");
        // 후킹 제거
        UnhookWindowsHookEx(g_testHook);
    }
}
