CC=$(HOME)/cheri/output/sdk/bin/clang
CFLAGS=-g -fuse-ld=lld --config cheribsd-riscv64-purecap.cfg
SSHPORT=10007

main: main.c
	$(CC) $(CFLAGS) -O2 -fpie -L. -Wl,-rpath,. -ldl -lstatic_variable -lunexported_function -lcompartment_per_object -g $< -o main

libstatic_variable.so: static_variable.c include/static_variable.h
	$(CC) $(CFLAGS) -O2 -g -fPIC -shared $< -o libstatic_variable.so

libunexported_function.so: unexported_function.c include/unexported_function.h
	$(CC) $(CFLAGS) -O2 -g -fPIC -shared $< -o libunexported_function.so

libcompartment_per_object.so: compartment_per_object.c include/compartment_per_object.h
	$(CC) $(CFLAGS) -O2 -g -fPIC -shared $< -o libcompartment_per_object.so

run:
	scp -r -P $(SSHPORT) * root@127.0.0.1:/root/demo/
	ssh -p $(SSHPORT) root@127.0.0.1 -t 'cd /root/demo; /root/demo/main'
