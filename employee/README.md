# Standalone examples

These examples don't build with the top-level makefile. Basic usage for a given
`<platform>` (from a directory like `.../cheri-examples/<example>` looks like
this:

```
$ make -f Makefile.<platform>
```

## Prerequisites

- An SDK for your `<platform>`, like those built by [`cheribuild`][cheribuild].
  By default, the makefiles will search `~/cheri/` for an appropriate SDK, but
  this can be overridden by defining `CHERIBASE`. Alternatively, a direct path
  to the SDK can be given in `SDKBASE`.
- For `run-<binary>` targets, a machine or model that can run the result,
  reachable by SSH.

[cheribuild]: https://github.com/CTSRD-CHERI/cheribuild

## Usage

Available targets are:

- `all`, the default, which builds all binaries,
- `bin/<binary>`, to build a specific binary,
- `run-<binary>`, to `scp` a binary and run it,
- `clang-format`, to format all example code,
- `clean`, to remove generated files, for this `<platform>` only.

All build output is stored in `bin/<platform>`.

Note that the `run-<binary>` targets require `SSHPORT` to be defined. `RUNUSER`
and `RUNHOST` can also be specified if required, but default to `root` and
`localhost`, respectively, since this is convenient for CheriBSD models.

For example (in `.../cheri-examples/employee`):

```
$ SSH_PORT=12345 make -f Makefile.morello-purecap run-sealed
```
