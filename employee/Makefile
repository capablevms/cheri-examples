CC=$(HOME)/cheri/output/sdk/bin/clang
CFORMAT=$(HOME)/cheri/output/sdk/bin/clang-format
CFLAGS=-fuse-ld=lld --config cheribsd-riscv64-purecap.cfg
SSHPORT=10021
export

cfiles := full_privileges.c read_only.c sealed.c
employee := $(patsubst %.c,bin/%,$(cfiles))

.PHONY: all run clean

all: $(employee)

bin/%: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

.SECONDEXPANSION:
run-%: bin/% $$(wildcard %.c)
	scp -P $(SSHPORT) $(word 2,$^) bin/$(<F) root@127.0.0.1:/root
	ssh -p $(SSHPORT) root@127.0.0.1 -t '/root/$(<F)'

clang-format:
	$(CFORMAT) -i $(cfiles) include/employee.h

clean: 
	rm -R bin/*