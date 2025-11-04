// bmp_utils.c 파일의 전체 내용을 아래 코드로 덮어쓰세요.

#include <stdio.h>
#include <stdlib.h>
#include "bmp_utils.h" // 자신의 헤더 파일을 포함하여 선언과 구현이 일치하는지 컴파일러가 검사하게 함

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
    
    // 파일 포인터를 픽셀 데이터 시작 위치로 이동
    fseek(fp, header->offset, SEEK_SET);
    fread(data, dataSize, 1, fp);
    
    return data;
}

// BMPImage 데이터를 파일 포인터에 쓰는 함수 (헤더 + 픽셀 데이터)
void write_data(FILE* fp, const BMPImage* image) {
    // 헤더 쓰기
    fwrite(&image->header, sizeof(BMPHeader), 1, fp);
    
    // 픽셀 데이터 쓰기
    fseek(fp, image->header.offset, SEEK_SET);
    fwrite(image->data, image->header.image_size_bytes, 1, fp);
}

// 픽셀 데이터를 그레이스케일로 변환하는 함수 (24비트 BMP 기준)
void change_to_grayscale(unsigned char* data, int width, int height) {
    long image_size = width * height * 3; // 24비트(3바이트) 기준
    for (long i = 0; i < image_size; i += 3) {
        // BMP는 Blue, Green, Red 순서
        unsigned char blue  = data[i];
        unsigned char green = data[i+1];
        unsigned char red   = data[i+2];
        
        // 평균값 계산
        unsigned char gray = (unsigned char)(((int)red + (int)green + (int)blue) / 3);
        
        // 모든 채널을 동일한 평균값으로 설정
        data[i]   = gray;
        data[i+1] = gray;
        data[i+2] = gray;
    }
}