
CC = gcc
CXX = g++

INCLUDE_DIR := -Ineo-ai-dlr/include

LDFLAGS := -Lneo-ai-dlr/lib -ldlr -lpthread

release: all


all: run-resnet run-resnet2


run-resnet: run-resnet.c
	mkdir -p bin
	$(CC) -O2 -std=c11 $(INCLUDE_DIR) run-resnet.c -o bin/run-resnet $(LDFLAGS)

run-resnet2: run-resnet.cc
	mkdir -p bin
	$(CXX) -O2 -std=c++11 $(INCLUDE_DIR) run-resnet.cc -o bin/run-resnet2 $(LDFLAGS)

clean:
	rm -rf bin/*
