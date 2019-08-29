
CC = gcc
CXX = g++

INCLUDE_DIR := -Ineo-ai-dlr/include
#INCLUDE_DIR := -Ineo-ai-dlr/include
#INCLUDE_DIR := $(INCLUDE_DIR) -Ineo-ai-dlr/3rdparty/tvm/src/runtime
#INCLUDE_DIR := $(INCLUDE_DIR) -Ineo-ai-dlr/3rdparty/tvm/3rdparty/dlpack/include
#INCLUDE_DIR := $(INCLUDE_DIR) -Ineo-ai-dlr/3rdparty/tvm/3rdparty/dmlc-core/include
#INCLUDE_DIR := $(INCLUDE_DIR) -Ineo-ai-dlr/3rdparty/tvm/include
#INCLUDE_DIR := $(INCLUDE_DIR) -Ineo-ai-dlr/3rdparty/treelite/runtime/native/include

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
