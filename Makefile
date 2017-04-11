all: renderer

renderer: main.cpp scene.cpp
	g++ main.cpp scene.cpp -Iinclude -o renderer

