//파일 읽기/쓰기 함수 선언

#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include "bmp.h"

// BMP 이미지 데이터를 메모리에서 관리하기 위한 구조체
typedef struct {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    unsigned char*   pixelData;
} BMPImage;

// 함수 선언
BMPImage* read_bmp(const char* filename);
int       write_bmp(const char* filename, BMPImage* image);
void      free_bmp(BMPImage* image);

#endif // BMP_UTILS_H