#!/bin/sh

set -eou pipefail

# Write the capability
./write
# Read it back
./read
