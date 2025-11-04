// bmp_utils.h 
#ifndef BMP_UTILS_H
#define BMP_UTILS_H

#include <stdio.h>
#include "bmp.h"

// 함수 선언부
void           read_header(FILE* fp, BMPHeader* header);
unsigned char* read_data(FILE* fp, const BMPHeader* header);
void           write_data(FILE* fp, const BMPImage* image);
void           change_to_grayscale(unsigned char* data, int width, int height);

// 텍스트 파일을 읽어 메모리에 올리는 보조 함수
char* read_text_file(const char* filename, size_t* file_len);

#endif