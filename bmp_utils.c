#include <stdio.h>
#include <stdlib.h>
#include "bmp_utils.h"

// BMP 파일을 읽어 메모리에 BMPImage 구조체로 로드하는 함수
BMPImage* read_bmp(const char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        perror("파일 열기 실패");
        return NULL;
    }

    BMPImage* img = (BMPImage*)malloc(sizeof(BMPImage));
    if (!img) {
        perror("BMPImage 메모리 할당 실패");
        fclose(fp);
        return NULL;
    }

    // 헤더 읽기 (54바이트 전체를 한 번에 읽음)
    fread(&img->header, sizeof(BMPHeader), 1, fp);

    // 픽셀 데이터 메모리 할당 및 읽기
    uint32_t pixelDataSize = img->header.image_size_bytes;
    if (pixelDataSize == 0) { // image_size_bytes가 0일 경우 직접 계산
        pixelDataSize = img->header.width_px * img->header.height_px * (img->header.bits_per_pixel / 8);
    }
    
    img->data = (unsigned char*)malloc(pixelDataSize);
    if (!img->data) {
        perror("픽셀 데이터 메모리 할당 실패");
        free(img);
        fclose(fp);
        return NULL;
    }

    // 파일 포인터를 픽셀 데이터 시작 위치로 이동
    fseek(fp, img->header.offset, SEEK_SET);
    fread(img->data, pixelDataSize, 1, fp);

    fclose(fp);
    return img;
}

// BMPImage 구조체 데이터를 파일에 쓰는 함수
int write_bmp(const char* filename, BMPImage* image) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        perror("파일 쓰기 실패");
        return -1;
    }

    // 헤더 쓰기
    fwrite(&image->header, sizeof(BMPHeader), 1, fp);
    
    // 픽셀 데이터 쓰기
    fseek(fp, image->header.offset, SEEK_SET);
    fwrite(image->data, image->header.image_size_bytes, 1, fp);

    fclose(fp);
    return 0;
}

// 할당된 메모리를 해제하는 함수
void free_bmp(BMPImage* image) {
    if (image) {
        if (image->data) {
            free(image->data);
        }
        free(image);
    }
}