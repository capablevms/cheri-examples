# Compartmentalising Allocator

A modification of the "bump allocator" in this repo. This allocator creates
compartments on the heap, and returns capabilities to allocations within the
compartment.

Because allocations are made against a compartment's (previously `mmap`'d)
buffer, we need to identify the compartment being used when we call `malloc`.
`init_compartment` returns a capability used to identify the compartment it
creates; calls to `malloc_compartment` accept this identifier as a parameter.

`main.c` shows a simple use, with allocations against two different
compartments.

Example is run using `make` in the typical fashion for the repo (i.e. along the
lines of `make -f Makefile.morello-hybrid run-main`).

## What are "compartments" here?

A "compartment" as created by this allocator is different conceptually to the
compartments created by the other examples in this repo. It's worth explaining
what "compartment" means in this context.

Here, the "compartment" is some heap-allocated space that only capabilities are
used to access. There are obviously ways to abuse "just some space on the heap"
if the DDC isn't also restricted. The easiest way to explain this is to draw it
out.

Assuming only the DDC is being used for compartmentalisation (so we're
constraining, say, dereferencing of addresses for data but not jump points etc)
and it's being used to bound which addresses can be legally dereferenced, we
could draw memory out as a "number line" of the address space and mark points
where a compartment's bounds begin and end. So we could diagram compartments
like so:

```
|--------------------|
0  g   f      F   G  ♾️
```

…where a compartment's lower bound is a lowercase letter and its upper bound
the corresponding upper case letter (here ♾️ is the maximum address, and the `G`
compartment's lower bound is marked by `g`, and the upper case marked by `G`;
there's also a compartment, `F`, which is more restricted than `G` and is a
strict subset of `G`). Clearly, this just defines compartment bounds relative
to each other, and not in any kind of detail, but all we're interested in is
the relative positioning of compartment boundaries here.

The compartments this example constructs are intended to sit _outwith_ all
other compartments, so that any access performed against them _must_ be done
through the capabilities returned by `malloc_compartment`. For this to work,
the DDC needs to be restricted when a program first runs, so that no future
compartment's bounds will overlap with the ones we construct on the heap. Then,
the ordinary compartments we create (derived from the now-restricted initial
DDC) can't overlap with these separated heap-y compartments.

I've taken to thinking of them as _"side compartments"_ to differentiate them
from ordinary compartments.

To illustrate what I mean, we can create a special space adjacent to our
ordinary compartments and allocations like this:

```
|---------------------|
0              ><     ♾️ 
```

…where `>` marks the upper bound of a DDC we install right when a program
launches, and `<` marks the lower bound of possible "side" compartments. To
extend the first diagram, we might make a side compartment that looks like…:

```
|------------------------------|
0  g   f    F   G  ><  p   P   ♾️
```

…and the "side" compartment `P` is what `compartment` means in this instance.
Those compartments are similar to the buffers created in the `bump-allocator`
example in their structure.

The intended use of these compartments is to create an isolated area of memory
which is only accessible via capabilities, so that a process can't just
calculate an address within the isolated area and dereference it (because the
limited DDC prevents this).
