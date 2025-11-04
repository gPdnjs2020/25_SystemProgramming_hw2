#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include "bmp.h" // BMPImage 구조체를 사용하기 위해 포함

// 함수 선언
BMPImage* read_bmp(const char* filename);
int       write_bmp(const char* filename, BMPImage* image);
void      free_bmp(BMPImage* image);

#endif // BMP_UTILS_H