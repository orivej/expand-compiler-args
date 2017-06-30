#!/bin/bash

expandResponseParams() {
    eval "params=($(./expand-compiler-args "$@"))"
}
