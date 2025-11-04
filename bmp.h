// BMP 헤더 구조체 정의

#ifndef BMP_H
#define BMP_H

// 구조체 멤버 사이에 추가적인 패딩 바이트가 생기지 않도록 설정
// BMP 파일 구조를 C언어 구조체로 정확하게 읽기 위해 필수적입니다.
#pragma pack(push, 1)

typedef struct {
    unsigned short bfType;      // BMP 파일 매직 넘버 (항상 'BM')
    unsigned int   bfSize;      // 파일 크기 (바이트)
    unsigned short bfReserved1; // 예약 공간 (0)
    unsigned short bfReserved2; // 예약 공간 (0)
    unsigned int   bfOffBits;   // 픽셀 데이터의 시작 위치 (오프셋)
} BITMAPFILEHEADER;

typedef struct {
    unsigned int   biSize;          // 정보 헤더의 크기 (40)
    int            biWidth;         // 이미지 너비 (픽셀)
    int            biHeight;        // 이미지 높이 (픽셀)
    unsigned short biPlanes;        // 색상 평면 수 (항상 1)
    unsigned short biBitCount;      // 픽셀당 비트 수 (1, 4, 8, 24, 32 등)
    unsigned int   biCompression;   // 압축 유형 (0: BI_RGB)
    unsigned int   biSizeImage;     // 이미지 데이터의 크기 (바이트)
    int            biXPelsPerMeter; // 가로 해상도
    int            biYPelsPerMeter; // 세로 해상도
    unsigned int   biClrUsed;       // 사용된 색상 수
    unsigned int   biClrImportant;  // 중요한 색상 수
} BITMAPINFOHEADER;

#pragma pack(pop)

#endif // BMP_H