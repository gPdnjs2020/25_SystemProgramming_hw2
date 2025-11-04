#ifndef BMP_H
#define BMP_H

#include <stdint.h> 

// 구조체 패딩을 1바이트로 설정하여 파일의 이진 데이터 구조와 정확히 일치시킴
#pragma pack(push, 1)

// BMP 파일 헤더와 DIB 헤더를 합친 단일 구조체 (총 54바이트)
typedef struct {
  uint16_t  type;             // 매직 넘버: 0x4D42 ('B', 'M')
  uint32_t  size;             // 파일 전체 크기 (바이트)
  uint16_t  reserved1;        // 예약 공간 (사용 안 함)
  uint16_t  reserved2;        // 예약 공간 (사용 안 함)
  uint32_t  offset;           // 파일 시작부터 픽셀 데이터까지의 거리 (바이트)
  uint32_t  dib_header_size;  // DIB 헤더 크기 (일반적으로 40)
  int32_t   width_px;         // 이미지 너비 (픽셀)
  int32_t   height_px;        // 이미지 높이 (픽셀)
  uint16_t  num_planes;       // 색상 평면 수 (항상 1)
  uint16_t  bits_per_pixel;   // 픽셀 당 비트 수 (e.g., 24)
  uint32_t  compression;      // 압축 유형 (0: BI_RGB, 압축 없음)
  uint32_t  image_size_bytes; // 순수 이미지 데이터 크기 (바이트)
  int32_t   x_resolution_ppm; // 가로 해상도 (미터당 픽셀)
  int32_t   y_resolution_ppm; // 세로 해상도 (미터당 픽셀)
  uint32_t  num_colors;       // 실제 사용되는 색상 수
  uint32_t  important_colors; // 중요한 색상 수
} BMPHeader;

// BMP 이미지를 메모리에서 관리하기 위한 구조체
typedef struct {
  BMPHeader header;       // BMP 헤더 정보
  unsigned char* data;    // 픽셀 데이터 (raw bytes)
} BMPImage;

#pragma pack(pop)

#endif // BMP_H