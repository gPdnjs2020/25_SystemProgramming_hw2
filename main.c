#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_utils.h"

// ----- 개발자 A 담당 기능 선언 -----
void process_header(const char *filename);
void process_hex_dump(const char *infile, const char *outfile);
void process_grayscale(const char *infile, const char *outfile);

// ----- 개발자 B 담당 기능 선언 -----
void process_encrypt(const char *bmp_in, const char *msg_file, const char *bmp_out);
void process_decrypt(const char *bmp_in);

// ----- 메인 함수 (인자 처리) -----
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "사용법: ./hw2BMP <옵션> [파일...]\n");
        return 1;
    }
    char *option = argv[1];

    if (strcmp(option, "-h") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "사용법: ./hw2BMP -h <input.bmp>\n");
            return 1;
        }
        process_header(argv[2]);
    }
    else if (strcmp(option, "-o") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "사용법: ./hw2BMP -o <input.bmp> <output.txt>\n");
            return 1;
        }
        process_hex_dump(argv[2], argv[3]);
    }
    else if (strcmp(option, "-g") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "사용법: ./hw2BMP -g <input.bmp> <output.bmp>\n");
            return 1;
        }
        process_grayscale(argv[2], argv[3]);
    }
    else if (strcmp(option, "-e") == 0)
    {
        // ... (이하 생략, 이전과 동일)
    }
    else if (strcmp(option, "-d") == 0)
    {
        // ... (이하 생략, 이전과 동일)
    }
    return 0;
}

// ----- 개발자 A 기능 구현 영역 -----

void process_header(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror("파일 열기 실패");
        return;
    }

    BMPHeader header;
    read_header(fp, &header); // bmp_utils.c에 구현된 함수 호출

    printf("========== BMP Header Info ==========\n");
    printf(" 매직 넘버 (Type)      : %c%c (0x%04X)\n", (char)(header.type), (char)(header.type >> 8), header.type);
    printf(" 파일 크기 (Size)        : %u bytes\n", header.size);
    printf(" 픽셀 데이터 오프셋     : %u bytes\n", header.offset);
    printf(" DIB 헤더 크기          : %u bytes\n", header.dib_header_size);
    printf(" 이미지 너비 (Width)     : %d px\n", header.width_px);
    printf(" 이미지 높이 (Height)    : %d px\n", header.height_px);
    printf(" 색상 평면 수           : %u\n", header.num_planes);
    printf(" 픽셀당 비트 수         : %u bits\n", header.bits_per_pixel);
    printf(" 압축 방식              : %u\n", header.compression);
    printf(" 이미지 데이터 크기     : %u bytes\n", header.image_size_bytes);
    printf(" 수평 해상도            : %d ppm\n", header.x_resolution_ppm);
    printf(" 수직 해상도            : %d ppm\n", header.y_resolution_ppm);
    printf(" 사용 색상 수           : %u\n", header.num_colors);
    printf(" 중요 색상 수           : %u\n", header.important_colors);
    printf("====================================\n");

    fclose(fp);
}

void process_hex_dump(const char* infile, const char* outfile) {
    //infile을 "rb" 모드로 열기
    FILE* fp_in = fopen(infile, "rb");
    if (!fp_in) {
        perror("입력 파일 열기 실패");
        return;
    }
    
    //outfile 열기
    FILE* fp_out = fopen(outfile, "w");
    if (!fp_out) {
        perror("출력 파일 열기 실패");
        fclose(fp_in); // 입력 파일은 이미 열었으므로 닫아줘야 합니다.
        return;
    }
    
    //BMPHeader를 읽고, read_data로 픽셀 데이터 읽기
    BMPHeader header;
    read_header(fp_in, &header);
    
    unsigned char* data = read_data(fp_in, &header);
    if (!data) {
        // read_data 내부에서 에러 메시지가 출력
        fclose(fp_in);
        fclose(fp_out);
        return;
    }
    
    // 픽셀 데이터의 전체 크기를 계산 (헤더의 image_size_bytes가 0일 경우 대비)
    uint32_t data_size = header.image_size_bytes;
    if (data_size == 0) {
        data_size = header.width_px * header.height_px * (header.bits_per_pixel / 8);
    }

    printf("'%s'의 픽셀 데이터를 16진수 덤프하여 '%s'에 저장합니다...\n", infile, outfile);

    // 픽셀 데이터를 형식에 맞게 16진수로 outfile에 쓰기
    for (uint32_t i = 0; i < data_size; ++i) {
        // 각 줄의 시작: 8바이트마다 상대 주소를 16진수로 출력
        if (i % 8 == 0) {
            // "%08X": 8자리 16진수, 비는 공간은 0으로 채움
            fprintf(fp_out, "%08X: ", i);
        }
        
        // 16진수 데이터 출력
        // "%02X": 2자리 16진수, 비는 공간은 0으로 채움
        fprintf(fp_out, "%02X ", data[i]);
        
        // 줄바꿈: 8바이트를 출력했거나, 데이터의 마지막 바이트인 경우
        if ((i + 1) % 8 == 0 || i == data_size - 1) {
            fprintf(fp_out, "\n");
        }
    }
    
    // 파일 닫고 메모리 해제
    fclose(fp_in);
    fclose(fp_out);
    free(data);
    
    printf("완료되었습니다.\n");
}

// 제공해주신 구조를 기반으로 작성된 그레이스케일 함수
void process_grayscale(const char *infile, const char *outfile)
{
    FILE *fp_in = fopen(infile, "rb");
    FILE *fp_out = fopen(outfile, "wb");

    if (!fp_in || !fp_out)
    {
        perror("파일 열기 오류");
        if (fp_in)
              fclose(fp_in);
        if (fp_out)
            fclose(fp_out);
        return;
    }

    BMPImage bmp_img;

    // 1. 헤더 읽기
    read_header(fp_in, &bmp_img.header);

    // 2. 픽셀 데이터 읽기
    bmp_img.data = read_data(fp_in, &bmp_img.header);
    if (!bmp_img.data)
    {
        fclose(fp_in);
        fclose(fp_out);
        return;
    }

    // 3. 그레이스케일로 변환
    change_to_grayscale(bmp_img.data, bmp_img.header.width_px, bmp_img.header.height_px);

    // 4. 새로운 파일에 쓰기
    write_data(fp_out, &bmp_img);

    // 5. 자원 해제
    fclose(fp_in);
    fclose(fp_out);
    free(bmp_img.data);

    printf("'%s' 파일이 그레이스케일로 변환되어 '%s'에 저장되었습니다.\n", infile, outfile);
}

// ----- 개발자 B 기능 구현 영역 -----

void process_encrypt(const char *bmp_in, const char *msg_file, const char *bmp_out)
{
    // TODO: 개발자 B가 구현
    // process_grayscale 함수와 유사한 구조로 작성
    // 1. 파일들 열기
    // 2. 헤더, 데이터 읽기
    // 3. 메시지 파일을 읽어서 픽셀 데이터에 암호화 (LSB 변조)
    // 4. 새로운 파일에 쓰기
    // 5. 자원 해제
    printf("암호화 기능은 아직 구현되지 않았습니다.\n");
}

void process_decrypt(const char *bmp_in)
{
    // TODO: 개발자 B가 구현
    // 1. 파일 열기
    // 2. 헤더, 데이터 읽기
    // 3. 픽셀 데이터에서 LSB를 추출하여 메시지 복호화
    // 4. 메시지 화면에 출력
    // 5. 자원 해제
    printf("복호화 기능은 아직 구현되지 않았습니다.\n");
}