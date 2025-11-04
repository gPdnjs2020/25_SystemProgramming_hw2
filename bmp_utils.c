//파일 읽기/쓰기 함수 구현

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
        perror("메모리 할당 실패");
        fclose(fp);
        return NULL;
    }

    // 헤더 읽기
    fread(&img->fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&img->infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    // 픽셀 데이터 메모리 할당 및 읽기
    int pixelDataSize = img->infoHeader.biSizeImage;
    if (pixelDataSize == 0) { // biSizeImage가 0일 경우 직접 계산
        pixelDataSize = img->infoHeader.biWidth * img->infoHeader.biHeight * (img->infoHeader.biBitCount / 8);
    }
    img->pixelData = (unsigned char*)malloc(pixelDataSize);
    if (!img->pixelData) {
        perror("픽셀 데이터 메모리 할당 실패");
        free(img);
        fclose(fp);
        return NULL;
    }

    // 파일 포인터를 픽셀 데이터 시작 위치로 이동
    fseek(fp, img->fileHeader.bfOffBits, SEEK_SET);
    fread(img->pixelData, pixelDataSize, 1, fp);

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

    fwrite(&image->fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&image->infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    fseek(fp, image->fileHeader.bfOffBits, SEEK_SET);
    fwrite(image->pixelData, image->infoHeader.biSizeImage, 1, fp);

    fclose(fp);
    return 0;
}

// 할당된 메모리를 해제하는 함수
void free_bmp(BMPImage* image) {
    if (image) {
        if (image->pixelData) {
            free(image->pixelData);
        }
        free(image);
    }
}