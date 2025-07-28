#include <windows.h>
#include "../include/screen_capture_block.h"
#include "../include/logger.h"

static HHOOK hScreenHook = NULL;

// 화면 캡처(프린트스크린) 훅 콜백
static LRESULT CALLBACK ScreenCaptureHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
        if (p->vkCode == VK_SNAPSHOT)
        {
            write_log("[SCREEN_CAPTURE] PrintScreen 키 입력 감지됨. 화면 캡처 차단.");
            // 이벤트를 소비하여 화면 캡처 방지
            return 1;
        }
    }
    return CallNextHookEx(hScreenHook, nCode, wParam, lParam);
}

void initialize_screen_capture_block(void)
{
    hScreenHook = SetWindowsHookEx(WH_KEYBOARD_LL, ScreenCaptureHookProc, NULL, 0);
    if (hScreenHook == NULL)
    {
        write_log("[ERROR] PrintScreen 차단 훅 설치 실패.");
    }
    else
    {
        write_log("[INFO] PrintScreen 차단 훅 설치 완료.");
    }
}
