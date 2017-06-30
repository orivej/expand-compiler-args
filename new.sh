#!/bin/bash

expandResponseParams() {
    readarray -d '' params < <(./expand-compiler-args "$@")
}
