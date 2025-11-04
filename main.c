#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_utils.h"

// ----- 개발자 A 담당 기능 선언 -----
void process_header(const char* filename);
void process_hex_dump(const char* infile, const char* outfile);
void process_grayscale(const char* infile, const char* outfile);

// ----- 개발자 B 담당 기능 선언 -----
void process_encrypt(const char* bmp_in, const char* msg_file, const char* bmp_out);
void process_decrypt(const char* bmp_in);


// ----- 메인 함수 (인자 처리) -----
int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: hw2BMP <옵션> [파일...]\n");
        return 1;
    }

    char* option = argv[1];

    if (strcmp(option, "-h") == 0) {
        if (argc != 3) { fprintf(stderr, "사용법: hw2BMP -h <input.bmp>\n"); return 1; }
        process_header(argv[2]);
    } else if (strcmp(option, "-o") == 0) {
        if (argc != 4) { fprintf(stderr, "사용법: hw2BMP -o <input.bmp> <output.txt>\n"); return 1; }
        process_hex_dump(argv[2], argv[3]);
    } else if (strcmp(option, "-g") == 0) {
        if (argc != 4) { fprintf(stderr, "사용법: hw2BMP -g <input.bmp> <output.bmp>\n"); return 1; }
        process_grayscale(argv[2], argv[3]);
    } else if (strcmp(option, "-e") == 0) {
        if (argc != 5) { fprintf(stderr, "사용법: hw2BMP -e <org.bmp> <msg.txt> <enc.bmp>\n"); return 1; }
        process_encrypt(argv[2], argv[3], argv[4]);
    } else if (strcmp(option, "-d") == 0) {
        if (argc != 3) { fprintf(stderr, "사용법: hw2BMP -d <encrypt.bmp>\n"); return 1; }
        process_decrypt(argv[2]);
    } else {
        fprintf(stderr, "알 수 없는 옵션: %s\n", option);
        return 1;
    }

    return 0;
}


// ----- 개발자 A 기능 구현 영역 -----

void process_header(const char* filename) {
    printf("===== 개발자 A: 헤더 정보 출력 기능 (-h) =====\n");
    BMPImage* img = read_bmp(filename);
    if (!img) return;

    // TODO: img->header의 멤버들을 형식에 맞게 출력하세요.
    // 예: printf("파일 크기: %u\n", img->header.size);
    //     printf("이미지 너비: %d\n", img->header.width_px);
    
    free_bmp(img);
}

void process_hex_dump(const char* infile, const char* outfile) {
    printf("===== 개발자 A: 16진수 덤프 기능 (-o) =====\n");
    BMPImage* img = read_bmp(infile);
    if (!img) return;

    // TODO: 출력 파일(outfile)을 쓰기 모드로 열고,
    // img->data의 내용을 과제 형식에 맞게 16진수로 출력하세요.
    
    free_bmp(img);
}

void process_grayscale(const char* infile, const char* outfile) {
    printf("===== 개발자 A: 그레이스케일 변환 기능 (-g) =====\n");
    BMPImage* img = read_bmp(infile);
    if (!img) return;

    // TODO: 
    // 1. img->data의 픽셀 데이터를 순회합니다. (총 크기는 img->header.image_size_bytes)
    // 2. 24비트 BMP 기준, 3바이트(Blue, Green, Red)씩 읽어 평균값(gray)을 계산합니다.
    // 3. 해당 3바이트를 모두 gray 값으로 덮어씁니다.
    // 4. write_bmp(outfile, img) 함수로 결과를 저장합니다.

    free_bmp(img);
}


// ----- 개발자 B 기능 구현 영역 -----

void process_encrypt(const char* bmp_in, const char* msg_file, const char* bmp_out) {
    printf("===== 개발자 B: 암호화 기능 (-e) =====\n");
    BMPImage* img = read_bmp(bmp_in);
    if (!img) return;
    
    // TODO: 
    // 1. msg_file의 내용을 읽어옵니다.
    // 2. img->data의 각 바이트의 최하위 비트(LSB)에 메시지의 각 비트를 숨깁니다.
    // 3. write_bmp(bmp_out, img) 함수로 결과를 저장합니다.

    free_bmp(img);
}

void process_decrypt(const char* bmp_in) {
    printf("===== 개발자 B: 복호화 기능 (-d) =====\n");
    BMPImage* img = read_bmp(bmp_in);
    if (!img) return;
    
    // TODO:
    // 1. img->data의 각 바이트에서 최하위 비트(LSB)를 순서대로 추출합니다.
    // 2. 8개의 비트를 모아 하나의 문자를 만듭니다.
    // 3. 메시지 끝을 나타내는 신호(e.g., NULL 문자)를 만날 때까지 반복합니다.
    // 4. 완성된 메시지를 화면에 출력합니다.
    
    free_bmp(img);
}