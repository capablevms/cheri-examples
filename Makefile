CC=$(HOME)/cheri/output/sdk/bin/clang
CXX=$(HOME)/cheri/output/sdk/bin/clang++
CFLAGS=-fuse-ld=lld --config cheribsd-riscv64-purecap.cfg
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
