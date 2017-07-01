#!/bin/bash

# @args.rsp parser.
# Char classes: other, space, backslash, single quote, double quote.
declare -A rspC
rspC=([' ']=1 [$'\t']=1 [$'\r']=1 [$'\n']=1 ['\']=2 ["'"]=3 ['"']=4)
# States: 0 - outside, 1/2 - unquoted arg/slash, 3/4 - 'arg'/slash, 5/6 - "arg"/slash.
# State transitions (char class * 8 + current state -> next state):
rspT='1113355 0013355 2214365 3311355 5513315'

rspExpand() {
    if [ "${1:0:1}" != '@' -o ! -e "${1:1}" ]; then
        printf '%s\0' "$1"
        return
    fi

    local cur=0 next=0 arg='' c

    while read -r -N1 c; do
        next="${rspT:8#${rspC["$c"]}$cur:1}"
        if [[ ( $cur = $next && $cur != 0 ) || ( $cur = 0 && $next = 1 ) ||
                  $cur = 2 || $cur = 4 || $cur = 6 ]]; then
            arg+="$c"
        elif [[ $cur = 1 && $next = 0 ]]; then
            rspExpand "$arg"
            arg=''
        fi
        cur=$next
    done <"${1:1}"

    if [ "$cur" -ne 0 ]; then
        rspExpand "$arg"
    fi
}

expandResponseParams() {
    readarray -d '' params < <(for arg in "$@"; do rspExpand "$arg"; done)
}
