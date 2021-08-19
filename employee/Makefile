CC=$(HOME)/cheri/output/sdk/bin/clang
CFORMAT=$(HOME)/cheri/output/sdk/bin/clang-format
CFLAGS=-fuse-ld=lld --config cheribsd-riscv64-purecap.cfg
SSHPORT=10021
export

employee := full_privileges.c read_only.c employee.c include/employee.h
read_only := read_only.c employee.c
full_privileges := full_privileges.c employee.c

all: bin/full_privileges bin/read_only

bin/read_only: $(read_only)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

bin/full_privileges: $(full_privileges)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

run:
	scp -P $(SSHPORT) bin/* root@127.0.0.1:/root
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/full_privileges'
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/read_only'

clang-format:
	$(CFORMAT) -i $(employee)

clean: 
	rm -R bin/*