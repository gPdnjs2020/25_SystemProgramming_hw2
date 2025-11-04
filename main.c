// 메인 로직 및 각 기능 구현

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
    // TODO:
    // 1. read_bmp() 함수로 BMP 파일 읽기
    // 2. 읽어온 BMPImage 구조체의 헤더 정보들을 형식에 맞게 출력
    // 3. free_bmp()로 메모리 해제
}

void process_hex_dump(const char* infile, const char* outfile) {
    printf("===== 개발자 A: 16진수 덤프 기능 (-o) =====\n");
    // TODO:
    // 1. read_bmp() 함수로 BMP 파일 읽기
    // 2. 출력 파일을 쓰기 모드로 열기
    // 3. BMPImage의 pixelData를 순회하면서 8바이트씩 16진수 형식으로 파일에 출력
    // 4. free_bmp()로 메모리 해제
}

void process_grayscale(const char* infile, const char* outfile) {
    printf("===== 개발자 A: 그레이스케일 변환 기능 (-g) =====\n");
    // TODO:
    // 1. read_bmp() 함수로 BMP 파일 읽기
    // 2. 픽셀 데이터(B, G, R 순서)를 순회하며 (R+G+B)/3 평균값 계산
    // 3. 각 픽셀의 B, G, R 값을 모두 평균값으로 변경
    // 4. write_bmp() 함수로 변경된 이미지 데이터를 새로운 파일에 저장
    // 5. free_bmp()로 메모리 해제
}


// ----- 개발자 B 기능 구현 영역 -----

void process_encrypt(const char* bmp_in, const char* msg_file, const char* bmp_out) {
    printf("===== 개발자 B: 암호화 기능 (-e) =====\n");
    // TODO:
    // 1. read_bmp()로 원본 BMP 파일 읽기
    // 2. 메시지 파일을 읽어 숨길 텍스트 가져오기
    // 3. 픽셀 데이터를 순회하며, 텍스트의 각 비트를 LSB에 숨기기
    // 4. write_bmp()로 암호화된 이미지 데이터를 새 파일에 저장
    // 5. free_bmp()로 메모리 해제
}

void process_decrypt(const char* bmp_in) {
    printf("===== 개발자 B: 복호화 기능 (-d) =====\n");
    // TODO:
    // 1. read_bmp()로 암호화된 BMP 파일 읽기
    // 2. 픽셀 데이터를 순회하며 각 바이트의 LSB를 추출
    // 3. 8비트씩 모아 하나의 문자로 만들고, 메시지 끝을 만나면 종료
    // 4. 추출된 메시지를 화면에 출력
    // 5. free_bmp()로 메모리 해제
}