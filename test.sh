#!/bin/bash
set -e
source "$1"
time expandResponseParams @ghc_7.rsp
s="${params[@]}"
echo "${#s} chars, ${#params[@]} args"
