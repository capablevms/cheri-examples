# Reading and writing capabilities to a pipe

An example which fails to "leak" capabilities using Unix pipes.

The setup consists of two sibling processes connected to a pipe:
  * a writer process that writes a capability to the write end of the pipe, and
  * a reader process that reads the capability written by the writer process
    to the pipe. The reader processs reads an invalid capability

## Running

This example is not automatically built by the top-level makefiles, but is
built in a similar manner:

```
$ make -f Makefile.<platform> run-main
```

Refer to the top-level [README][../README.md] for usage details.
