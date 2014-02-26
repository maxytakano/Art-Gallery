CXX=clang++
CXXFLAGS=-g -std=c++11 -Wno-deprecated
LDFLAGS=-framework GLUT -framework OpenGL -framework CoreServices

objects=Vector3.o Vector4.o Matrix4.o MatrixTransform.o Bench.o Group.o objreader.o tiny_obj_loader.o Object.o Curve.o Vector3L.o Vector4L.o Matrix4L.o Shader.o Camera.o

.PHONY: all clean

all: cube

clean:
	rm -rf *.o cube cube.dSYM

cube: $(objects)
