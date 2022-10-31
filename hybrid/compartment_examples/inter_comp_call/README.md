# `inter_comp_call` examples

These examples are not automatically built by the top-level makefiles, but are
built in a similar manner.

## `base`

To run the `base` example:

```
$ cd base && make -f Makefile.morello-hybrid run-main
```

## `malicious_compartments`

To run one of the `malicious_compartments` examples:

```
$ cd secure && make -f Makefile.morello-hybrid run-inter_comp_call-<example>
```

Refer to the top-level [README](../../../README.md) for usage details.
