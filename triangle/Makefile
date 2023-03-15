INCLUDE=-I ./include -I .

all:
	clang -c ./src/glad.c triangle.cpp $(INCLUDE)
	clang++ $(INCLUDE) -o main main.cpp triangle.o glad.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
