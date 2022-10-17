#!/bin/sh

set -eou pipefail

# Attempt to share the capability
./write | ./read
