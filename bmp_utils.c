// bmp_utils.c

#include <stdio.h>
#include <stdlib.h>
#include "bmp_utils.h"

// 파일 포인터로부터 BMP 헤더를 읽어오는 함수
void read_header(FILE* fp, BMPHeader* header) {
    fread(header, sizeof(BMPHeader), 1, fp);
}

// 파일 포인터와 헤더 정보를 이용해 픽셀 데이터를 읽어오는 함수
unsigned char* read_data(FILE* fp, const BMPHeader* header) {
    uint32_t dataSize = header->image_size_bytes;
    if (dataSize == 0) {
        dataSize = header->width_px * header->height_px * (header->bits_per_pixel / 8);
    }
    unsigned char* data = (unsigned char*)malloc(dataSize);
    if (!data) {
        perror("픽셀 데이터 메모리 할당 실패");
        return NULL;
    }
    fseek(fp, header->offset, SEEK_SET);
    fread(data, dataSize, 1, fp);
    return data;
}

// BMPImage 데이터를 파일 포인터에 쓰는 함수 (헤더 + 픽셀 데이터)
void write_data(FILE* fp, const BMPImage* image) {
    fwrite(&image->header, sizeof(BMPHeader), 1, fp);
    fseek(fp, image->header.offset, SEEK_SET);
    fwrite(image->data, image->header.image_size_bytes, 1, fp);
}

// 픽셀 데이터를 그레이스케일로 변환하는 함수 (24비트 BMP 기준)
void change_to_grayscale(unsigned char* data, int width, int height) {
    long image_size = width * height * 3;
    for (long i = 0; i < image_size; i += 3) {
        unsigned char blue  = data[i];
        unsigned char green = data[i+1];
        unsigned char red  = data[i+2];
        unsigned char gray = (unsigned char)(((int)red + (int)green + (int)blue) / 3);
        data[i]   = gray;
        data[i+1] = gray;
        data[i+2] = gray;
    }
}

char* read_text_file(const char* filename, size_t* file_len) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("메시지 파일 열기 실패");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    *file_len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = (char*)malloc(*file_len + 1);
    if (!buffer) {
        perror("메시지 버퍼 할당 실패");
        fclose(fp);
        return NULL;
    }

    fread(buffer, *file_len, 1, fp);
    buffer[*file_len] = '\0'; // 문자열 끝 표시

    fclose(fp);
    return buffer;
}