# "syscall-restrict" example

## Description

This example demonstrates `CHERI_PERM_SYSCALL`. If this permission is absent
from the user-space caller's PCC, the CheriBSD kernel will reject all syscalls.

Many syscalls have the potential to be a compartment escape. For example,
mmap-like syscalls can be used to change what is reachable by existing
capabilities (for example by changing virtual address mappings). In addition,
signal handlers cannot currently be safely registered by any compartment other
than the top level, root compartment.

Finally, it may also be desirable to limit access to system-wide resources not
explicitly protected by virtual address compartmentalisation. Examples include
file systems, or file descriptors.

Blocking access to syscalls permits a conservative, allow-list-based approach.
This example does not demonstrate such an allow-list, nor the necessary
compartmentalisation, but it demonstrates the behaviour of `CHERI_PERM_SYSCALL`
handling in the CheriBSD kernel.

## Building and running

This example is not automatically built by the top-level makefiles, but is
built in a similar manner:

```
$ make -f Makefile.<platform>
```

Refer to the top-level [README](../README.md) for usage details.

For example:

```
$ SSHPORT=1234 make -f Makefile.morello-hybrid run-syscall-restrict
```
