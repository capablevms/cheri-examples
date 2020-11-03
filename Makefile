CC=$(HOME)/cheri/output/sdk/bin/riscv64-unknown-freebsd13-clang
CXX=$(HOME)/cheri/output/sdk/bin/riscv64-unknown-freebsd13-clang++
CFLAGS=-march=rv64imafdcxcheri -mabi=l64pc128d --sysroot=$(HOME)/cheri/output/rootfs-riscv64-purecap -mno-relax -g -O0
SSHPORT=10021
export 

cfiles := $(wildcard *.c)
cppfiles := $(wildcard *.cpp)
examples := $(patsubst %.c,bin/%,$(cfiles)) $(patsubst %.cpp,bin/%,$(cfiles))

.PHONY: all run clean

all: $(examples)

bin/%: %.c
	$(CC) $(CFLAGS) $< -o $@

bin/%: %.cpp
	$(CXX) $(CFLAGS) $< -o $@

.SECONDEXPANSION:
run-%: bin/% $$(wildcard %.c) $$(wildcard %.cpp)
	scp -P $(SSHPORT) $(word 2,$^) bin/$(<F) root@127.0.0.1:/root
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/$(<F)'

clean: 
	rm -rv bin/*
