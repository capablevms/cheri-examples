# "shared\_objects" examples

These examples are not automatically built by the top-level makefiles, but are
built in a similar manner:

```
$ make -f Makefile.<platform>
```

Unlike most other examples, these compile to a single executable (with
accompanying shared objects). There is only a single `run-shared_objects`
target, and the example is selected interactively.

Otherwise, refer to the top-level [README](../README.md) for usage details.
