CC=$(HOME)/cheri/output/sdk/bin/riscv64-unknown-freebsd13-clang

CFLAGS=-march=rv64imafdcxcheri -mabi=l64pc128d --sysroot=$(HOME)/cheri/output/rootfs-riscv64-hybrid -mno-relax -g -O0 -I../..
SSHPORT=10003
export 

cfiles := $(wildcard *.c)
examples := $(patsubst %.c,bin/%,$(cfiles))


good_client: good_client.c bitmap_alloc.c

bad_client: bad_client.c bitmap_alloc.c

test:
	scp -P $(SSHPORT) good_client bad_client root@127.0.0.1:/root
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/good_client'
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/bad_client'

bin/%: %.c
	$(CC) $(CFLAGS) $< -o $@

run-%: bin/%
	scp -P $(SSHPORT) bin/$(<F) $(<F).c root@127.0.0.1:/root
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/$(<F)'

clean: 
	rm -rv bin/*
