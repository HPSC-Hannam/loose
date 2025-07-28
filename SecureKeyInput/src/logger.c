#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include "../include/logger.h"

static const char *LOG_FILE_NAME = "log.txt";

void write_log(const char *message)
{
    FILE *logFile = fopen(LOG_FILE_NAME, "a");
    if (logFile != NULL)
    {
        time_t now = time(NULL);
        struct tm *local = localtime(&now);

        fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                local->tm_year + 1900,
                local->tm_mon + 1,
                local->tm_mday,
                local->tm_hour,
                local->tm_min,
                local->tm_sec,
                message);
        fclose(logFile);
    }
}

void write_logf(const char *format, ...)
{
    FILE *logFile = fopen(LOG_FILE_NAME, "a");
    if (logFile != NULL)
    {
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] ",
                local->tm_year + 1900,
                local->tm_mon + 1,
                local->tm_mday,
                local->tm_hour,
                local->tm_min,
                local->tm_sec);

        va_list args;
        va_start(args, format);
        vfprintf(logFile, format, args);
        va_end(args);

        fprintf(logFile, "\n");
        fclose(logFile);
    }
}
