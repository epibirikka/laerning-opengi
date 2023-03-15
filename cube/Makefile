INCLUDE=-I . -I ./include
MAIN=-o main main.cpp
OBJ=glad.o cube.o program.o skybox.o

LINK=`pkg-config --libs glfw3 gl`

all:
	make compile
	make run
compile:
	clang -c cube.cpp skybox.cpp program.cpp src/glad.c $(INCLUDE)
	clang++ $(MAIN) $(OBJ) $(INCLUDE) $(LINK)
run:
	./main
