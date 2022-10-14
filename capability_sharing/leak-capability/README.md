# Leak a capability to another process

This example shows how a process can "leak" its capabilities

The setup consists of two processes:
* a writer process that writes a capability to a file, and
* a reader process that reads the capability written by the writer process. The
  reader processs successfully reads a valid capability

According to the CHERI C/C++ programming guide:

> in the CheriABI process environment, only untagged data (not tagged pointers)
> may be written to or read from files. If a region of memory containing valid
> pointers is written to a file, and then read back, the pointers in that region
> will no longer be valid

However, as this example demonstrates, this behaviour appears to be broken, as
it is possible to read a valid capability from a file.

## Running

This example is not automatically built by the top-level makefiles, but are
built in a similar manner:

```
$ make -f Makefile.<platform> run-leak-capability
```

Refer to the top-level [README][../README.md] for usage details.
