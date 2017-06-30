#!/bin/bash
set -e
source "$1"
time expandResponseParams "$2"
s="${params[@]}"
echo "${#s} chars, ${#params[@]} args"
