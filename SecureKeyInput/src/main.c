#include <windows.h>
#include "../include/logger.h"
#include "../include/hook_detector.h"
#include "../include/input_encrypt.h"
#include "../include/screen_capture_block.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    write_log("SecureKeyInput 백그라운드 보안 프로그램 시작됨.");

    initialize_hook_detector();
    initialize_encrypted_keylogger();
    initialize_screen_capture_block();

    // 메시지 루프 (훅 유지용)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
    }

    write_log("SecureKeyInput 종료됨.");
    return 0;
}
