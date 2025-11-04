// bmp_utils.h 파일의 전체 내용을 아래 코드로 덮어쓰세요.

#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include <stdio.h>
#include "bmp.h"

// 함수 선언부
// main.c에서 사용할 함수들을 여기에 모두 약속(선언)합니다.
void           read_header(FILE* fp, BMPHeader* header);
unsigned char* read_data(FILE* fp, const BMPHeader* header);
void           write_data(FILE* fp, const BMPImage* image);
void           change_to_grayscale(unsigned char* data, int width, int height);

#endif // BMP_UTILS_H