#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>

// 바이트 배열 입력 → AES‑256‑CBC 암호화 (출력 버퍼는 malloc으로 할당)
// 반환값: 암호문 길이, 실패 시 –1
int aes_encrypt(const unsigned char *plaintext, int plaintext_len,
                unsigned char **ciphertext_out);

// 복호화 (decrypt_tool.c에서 사용)
int aes_decrypt(const unsigned char *ciphertext, int ciphertext_len,
                unsigned char **plaintext_out);

#endif
