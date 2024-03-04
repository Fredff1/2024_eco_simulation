#
# Makefile for installing the mingw32 version of the SDL library

# 定义编译器
CC=D:\mingw64\bin\gcc.exe

# 包含的目录
IDIR=-I D:\extra h for c cpp\SDL2-2.28.4\x86_64-w64-mingw32\include\SDL2 \
     -I D:\extra h for c cpp\SDL2_ttf-2.20.2\x86_64-w64-mingw32\include\lSDL2_ttf \
     -I D:\extra h for c cpp\SDL2_image-2.6.3\x86_64-w64-mingw32\include\lSDL2_image

# 库目录
LDIR=-L D:\extra h for c cpp\SDL2-2.28.4\x86_64-w64-mingw32\lib \
     -L D:\extra h for c cpp\SDL2_ttf-2.20.2\x86_64-w64-mingw32\lib \
     -L D:\extra h for c cpp\SDL2_image-2.6.3\x86_64-w64-mingw32\lib

# 编译器选项
CFLAGS=-fdiagnostics-color=always -g -fexec-charset=UTF-8

# 链接器选项
LFLAGS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image

# 源文件
SRC=main.c Conway.c

# 输出的可执行文件
EXEC=${workspaceFolder}/program1.exe

# 默认目标
all: $(EXEC)

# 编译和链接
$(EXEC):
	$(CC) $(CFLAGS) $(IDIR) $(SRC) $(LDIR) $(LFLAGS) -o $(EXEC)

# 清理
clean:
	del $(EXEC)

