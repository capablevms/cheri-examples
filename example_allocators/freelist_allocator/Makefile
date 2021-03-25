CC=$(HOME)/cheri/output/sdk/bin/riscv64-unknown-freebsd13-clang

CFLAGS=-march=rv64imafdcxcheri -mabi=l64pc128d --sysroot=$(HOME)/cheri/output/rootfs-riscv64-hybrid -mno-relax -g -O0 -I../.. -lm
SSHPORT=10006
export 

cfiles := $(wildcard *.c)
examples := $(patsubst %.c,bin/%,$(cfiles))


binary_trees: binary_trees.c freelist_allocator.c

test: binary_trees
	scp -P $(SSHPORT) binary_trees root@127.0.0.1:/root
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/binary_trees 10'

bin/%: %.c
	$(CC) $(CFLAGS) $< -o $@

run-%: bin/%
	scp -P $(SSHPORT) bin/$(<F) $(<F).c root@127.0.0.1:/root
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/$(<F)'

clean: 
	rm -rfv binary_trees 
