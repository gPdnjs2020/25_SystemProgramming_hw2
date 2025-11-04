// main.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_utils.h"

void process_header(const char *filename);
void process_hex_dump(const char *infile, const char *outfile);
void process_grayscale(const char *infile, const char *outfile);
void process_encrypt(const char *bmp_in, const char *msg_file, const char *bmp_out);
void process_decrypt(const char *bmp_in);

// 메인 함수 (인자 처리)
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }
    char *option = argv[1];

    if (strcmp(option, "-h") == 0)
    {
        if (argc != 3)
        {
            return 1;
        }
        process_header(argv[2]);
    }
    else if (strcmp(option, "-o") == 0)
    {
        if (argc != 4)
        {
            return 1;
        }
        process_hex_dump(argv[2], argv[3]);
    }
    else if (strcmp(option, "-g") == 0)
    {
        if (argc != 4)
        {
            return 1;
        }
        process_grayscale(argv[2], argv[3]);
    }
    else if (strcmp(option, "-e") == 0)
    {
        if (argc != 5)
        {
            return 1;
        }
        process_encrypt(argv[2], argv[3], argv[4]);
    }
    else if (strcmp(option, "-d") == 0)
    {
        if (argc != 3)
        {
            return 1;
        }
        process_decrypt(argv[2]);
    }
    else
    {
        return 1;
    }
    return 0;
}

// 1-1~1-3 기능 구현: 신혜원
// 1-1
void process_header(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        perror("파일 열기 실패");
        return;
    }
    BMPHeader header;
    read_header(fp, &header);
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

// 1-2
void process_hex_dump(const char *infile, const char *outfile)
{
    FILE *fp_in = fopen(infile, "rb");
    if (!fp_in)
    {
        perror("입력 파일 열기 실패");
        return;
    }
    FILE *fp_out = fopen(outfile, "w");
    if (!fp_out)
    {
        perror("출력 파일 열기 실패");
        fclose(fp_in);
        return;
    }
    BMPHeader header;
    read_header(fp_in, &header);
    unsigned char *data = read_data(fp_in, &header);
    if (!data)
    {
        fclose(fp_in);
        fclose(fp_out);
        return;
    }
    uint32_t data_size = header.image_size_bytes;
    if (data_size == 0)
    {
        data_size = header.width_px * header.height_px * (header.bits_per_pixel / 8);
    }
    printf("'%s'의 픽셀 데이터를 16진수 덤프하여 '%s'에 저장합니다...\n", infile, outfile);
    for (uint32_t i = 0; i < data_size; ++i)
    {
        if (i % 8 == 0)
        {
            fprintf(fp_out, "%08X: ", i);
        }
        fprintf(fp_out, "%02X ", data[i]);
        if ((i + 1) % 8 == 0 || i == data_size - 1)
        {
            fprintf(fp_out, "\n");
        }
    }
    fclose(fp_in);
    fclose(fp_out);
    free(data);
    printf("완료되었습니다.\n");
}

// 1-3
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
    read_header(fp_in, &bmp_img.header);
    bmp_img.data = read_data(fp_in, &bmp_img.header);
    if (!bmp_img.data)
    {
        fclose(fp_in);
        fclose(fp_out);
        return;
    }
    change_to_grayscale(bmp_img.data, bmp_img.header.width_px, bmp_img.header.height_px);
    write_data(fp_out, &bmp_img);
    fclose(fp_in);
    fclose(fp_out);
    free(bmp_img.data);
    printf("'%s' 파일이 그레이스케일로 변환되어 '%s'에 저장되었습니다.\n", infile, outfile);
}

// 2.1~2.2 기능 구현 : 신태현 -----

void process_encrypt(const char *bmp_in, const char *msg_file, const char *bmp_out)
{
    FILE *fp_in = fopen(bmp_in, "rb");
    if (!fp_in)
    {
        perror("원본 BMP 파일 열기 실패");
        return;
    }

    BMPImage bmp_img;
    read_header(fp_in, &bmp_img.header);
    bmp_img.data = read_data(fp_in, &bmp_img.header);
    fclose(fp_in); // 원본 파일 읽기 닫기
    if (!bmp_img.data)
        return;

    // 메시지 파일 로드
    size_t msg_len = 0;
    char *msg = read_text_file(msg_file, &msg_len);
    if (!msg)
    {
        free(bmp_img.data);
        return;
    }

    // 숨길 데이터 [길이(1바이트) + 메시지]의 비트 수 계산
    size_t total_len_to_hide = 1 + msg_len; // 길이 정보 1바이트 + 메시지
    size_t total_bits_to_hide = total_len_to_hide * 8;

    // 이미지에 숨길 수 있는 최대 비트 수 계산 (1바이트당 1비트)
    size_t capacity_bits = bmp_img.header.image_size_bytes;
    if (capacity_bits == 0)
    {
        capacity_bits = bmp_img.header.width_px * bmp_img.header.height_px * 3;
    }

    if (total_bits_to_hide > capacity_bits)
    {
        fprintf(stderr, "오류: 메시지가 너무 길어 이미지에 숨길 수 없습니다.\n");
        fprintf(stderr, "필요한 공간: %zu bits, 사용 가능한 공간: %zu bits\n", total_bits_to_hide, capacity_bits);
        free(msg);
        free(bmp_img.data);
        return;
    }

    // LSB(최하위 비트)에 데이터 숨기기
    size_t bit_idx = 0;

    // 메시지 길이(1 바이트) 먼저 숨기기
    unsigned char len_byte = (unsigned char)msg_len;
    for (int i = 0; i < 8; ++i)
    {
        unsigned char bit = (len_byte >> i) & 1;                      // i번째 비트 추출
        bmp_img.data[bit_idx] = (bmp_img.data[bit_idx] & 0xFE) | bit; // LSB를 bit로 교체
        bit_idx++;
    }

    // 메시지 본문 숨기기
    for (size_t i = 0; i < msg_len; ++i)
    {
        char current_char = msg[i];
        for (int j = 0; j < 8; ++j)
        {
            unsigned char bit = (current_char >> j) & 1;
            bmp_img.data[bit_idx] = (bmp_img.data[bit_idx] & 0xFE) | bit;
            bit_idx++;
        }
    }

    // 4) 암호화된 데이터를 새로운 BMP 파일에 저장
    FILE *fp_out = fopen(bmp_out, "wb");
    if (!fp_out)
    {
        perror("암호화된 BMP 파일 생성 실패");
        free(msg);
        free(bmp_img.data);
        return;
    }

    write_data(fp_out, &bmp_img);
    fclose(fp_out);

    printf("암호화 완료: '%s'의 내용이 '%s'에 저장되었습니다.\n", msg_file, bmp_out);

    // 5) 메모리 해제
    free(msg);
    free(bmp_img.data);
}

void process_decrypt(const char *bmp_in)
{
    FILE *fp_in = fopen(bmp_in, "rb");
    if (!fp_in)
    {
        perror("암호화된 BMP 파일 열기 실패");
        return;
    }

    BMPImage bmp_img;
    read_header(fp_in, &bmp_img.header);
    bmp_img.data = read_data(fp_in, &bmp_img.header);
    fclose(fp_in);
    if (!bmp_img.data)
        return;

    if (bmp_img.header.bits_per_pixel != 24)
    {
        fprintf(stderr, "오류: 24비트 BMP 파일만 지원합니다.\n");
        free(bmp_img.data);
        return;
    }

    size_t capacity_bytes = bmp_img.header.image_size_bytes;
    if (capacity_bytes == 0)
    {
        capacity_bytes = bmp_img.header.width_px * bmp_img.header.height_px * 3;
    }

    if (capacity_bytes < 8)
    {
        fprintf(stderr, "오류: 파일이 너무 작아 길이 정보를 읽을 수 없습니다.\n");
        free(bmp_img.data);
        return;
    }

    size_t bit_idx = 0;

    // 1) 숨겨진 메시지 길이(1 바이트) 먼저 복원
    unsigned char msg_len = 0;
    for (int i = 0; i < 8; ++i)
    {
        unsigned char bit = bmp_img.data[bit_idx] & 1; // LSB 추출
        msg_len |= (bit << i);                         // 비트를 다시 바이트로 조립
        bit_idx++;
    }

    printf("복호화된 메시지 길이: %u bytes\n", msg_len);

    if (capacity_bytes < 8 + (size_t)msg_len * 8)
    {
        fprintf(stderr, "오류: 파일 데이터가 손상되었거나 길이가 잘못되었습니다.\n");
        free(bmp_img.data);
        return;
    }

    // 2) 메시지 본문 복원
    printf("복호화된 메시지: ");
    for (unsigned int i = 0; i < msg_len; ++i)
    {
        char recovered_char = 0;
        for (int j = 0; j < 8; ++j)
        {
            unsigned char bit = bmp_img.data[bit_idx] & 1;
            recovered_char |= (bit << j);
            bit_idx++;
        }
        putchar(recovered_char);
    }
    putchar('\n');

    // 3) 메모리 해제
    free(bmp_img.data);
}