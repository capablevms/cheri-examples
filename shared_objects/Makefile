CC=$(HOME)/cheri/output/sdk/bin/riscv64-unknown-freebsd13-clang
CFLAGS=-march=rv64imafdcxcheri -mabi=l64pc128d --sysroot=$(HOME)/cheri/output/rootfs-riscv64-hybrid -mno-relax -g
SSHPORT=10021

main: main.c
	$(CC) $(CFLAGS) -L. -l1 -l2 -g $< -o main

lib1.so: lib1.c
	$(CC) $(CFLAGS) -g -fPIC -shared $< -o lib1.so

lib2.so: lib2.c
	$(CC) $(CFLAGS) -g -fPIC -shared $< -o lib2.so

run:
	scp -P $(SSHPORT) * root@127.0.0.1:/root/demo/
	ssh -p $(SSHPORT) root@127.0.0.1 -t 'setenv LD_LIBRARY_PATH /root/demo; /root/demo/main'
