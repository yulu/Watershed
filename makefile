watershed:watershed.o
	g++ watershed.o `pkg-config --libs opencv` -o watershed
watershed.o:watershed.cpp
	gcc `pkg-config --cflags opencv` -g -c watershed.cpp
