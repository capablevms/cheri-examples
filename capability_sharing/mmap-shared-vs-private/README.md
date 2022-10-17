# Writing capabilities to anonymous and file-backed mappings

These examples demonstrate what happens when you write tagged pointer
values to various types of memory mappings:
 * Anonymous private mapping (OK)
 * Anonymous shared mapping (OK)
 * File-backed private mapping (OK)
 * File-backed shared mapping (segfault)

These examples are not automatically built by the top-level makefiles, but are
built in a similar manner:

```
$ make -f Makefile.<platform> run-private_anon_main
$ make -f Makefile.<platform> run-private_file_main
$ make -f Makefile.<platform> run-shared_anon_main
$ make -f Makefile.<platform> run-shared_file_main
```

Refer to the top-level [README][../README.md] for usage details.
