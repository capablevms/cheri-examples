# Reading and writing capabilities to a pipe

The setup consists of two processes spawned by the shell:
  * a writer process (whose stdout is redirected to a pipe by the shell), which
    writes a capability to stdout
  * a reader process which reads the capability from stdin (its stdin is redirected
    to the same pipe the writer process writes to). The reader processs reads an
    invalid capability

## Running

This example is not automatically built by the top-level makefiles, but is
built in a similar manner:

```
$ make -f Makefile.<platform> run-read-cap-from-pipe-with-sh
```

Refer to the top-level [README](../../README.md) for usage details.
