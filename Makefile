# 컴파일러 지정
CC = gcc

# 컴파일 옵션
CFLAGS = -Wall -Wextra -g

# 실행 파일 이름
TARGET = hw2BMP

# 소스 파일 목록
SOURCES = main.c bmp_utils.c

# 오브젝트 파일 목록
OBJECTS = $(SOURCES:.c=.o)

# 최종 목표: 실행 파일 생성
all: $(TARGET)

# 실행 파일은 오브젝트 파일들을 링크하여 생성
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# .c 파일을 .o 파일로 컴파일하는 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 컴파일 중간 파일들 삭제
clean:
	rm -f $(OBJECTS) $(TARGET)