# 컴파일러를 gcc로 지정합니다.
CC = gcc

# 컴파일 옵션을 지정합니다.
# -Wall -Wextra : 가능한 모든 경고 메시지를 출력하여 실수를 줄여줍니다.
# -g            : 디버깅 정보를 포함하여 컴파일합니다.
# -o [파일명]   : 출력될 실행 파일의 이름을 지정합니다.
CFLAGS = -Wall -Wextra -g

# 최종적으로 생성될 실행 파일의 이름을 지정합니다.
TARGET = hw2BMP

# 컴파일할 모든 C 소스 파일(.c)의 목록을 작성합니다.
SOURCES = main.c bmp_utils.c

# 소스 파일 목록(.c)을 기반으로 오브젝트 파일(.o) 목록을 자동으로 생성합니다.
# (예: main.c -> main.o)
OBJECTS = $(SOURCES:.c=.o)

# 'make' 명령어를 그냥 실행했을 때 기본적으로 수행될 목표(target)를 지정합니다.
# all 목표는 최종 실행 파일인 $(TARGET)을 만드는 것에 의존합니다.
all: $(TARGET)

# 최종 실행 파일 $(TARGET)을 만드는 규칙입니다.
# 이 목표는 모든 오브젝트 파일들($(OBJECTS))이 준비되어야 실행됩니다.
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# C 소스 파일(.c)을 오브젝트 파일(.o)로 컴파일하는 규칙입니다.
# '%.o: %.c'는 '.c' 파일로부터 '.o' 파일을 만드는 일반적인 규칙을 정의합니다.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 'make clean' 명령어를 실행했을 때 수행될 목표입니다.
# 컴파일 과정에서 생성된 모든 파일들을 삭제하여 프로젝트 폴더를 깨끗하게 정리합니다.
clean:
	rm -f $(OBJECTS) $(TARGET)