# cheri-examples

Examples of CHERI fundamental operations, interesting corner cases, and simple
demonstrative applications.

A `Makefile` is provided for each supported platform, and a basic build command
looks like this:

```
$ make -f Makefile.<platform>
```

For most examples, each `.c` file compiles to a single, self-contained example,
and compiles to a binary with the same name (without the `.c`).

Note that this only builds examples at the top level. Some examples are grouped
into their own directories, with their own makefiles. However, the available
make targets are consistent, where possible. Refer to their respective README
files for details.

## Prerequisites

- An SDK for your `<platform>`, like those built by [`cheribuild`][cheribuild].
  By default, the makefiles will search `~/cheri/` for an appropriate SDK, but
  this can be overridden by defining `CHERIBASE`. Alternatively, a direct path
  to the SDK can be given in `SDKBASE`.
- For `run-<binary>` targets, a machine or model that can run the result,
  reachable by SSH.
- GNU make. Depending on your platform you might need to specify `gmake` rather
  than `make`.

[cheribuild]: https://github.com/CTSRD-CHERI/cheribuild

## Usage

Targets available for all example directories are:

- `all`, the default, which builds all binaries,
- `clang-format`, to format all example code,
- `clean`, to remove generated files, for this `<platform>` only.

The top level examples, and most examples in sub-directories, also allow
individual examples to be built or run:

- `bin/<binary>`, to build a specific binary,
- `run-<binary>`, to `scp` a binary and run it.

Refer to their respective README files for details.

All build output is stored in `bin/<platform>`.

Note that the `run-<binary>` targets require `SSHPORT` to be defined. `RUNUSER`
and `RUNHOST` can also be specified if required, but default to `root` and
`localhost`, respectively, since this is convenient for CheriBSD models.

For example:

```
$ SSHPORT=12345 make -f Makefile.morello-purecap run-seal
```
