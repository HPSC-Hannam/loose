#ifndef LOGGER_H
#define LOGGER_H

// 로그 메시지 출력 함수 (기본)
void write_log(const char *message);

// 로그 메시지 출력 함수 (포맷 포함)
void write_logf(const char *format, ...);

#endif
