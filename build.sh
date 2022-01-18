#!/bin/bash

exe=xc
src=src/*.c
cc=gcc
std=-std=c99
opt=-O2
inc=-I.

wflags=(
    -Wall 
    -Wextra
)

cflags=(
    ${std[*]} 
    ${opt[*]} 
    ${wflags[*]}
    ${inc[*]}
)

comp() {
    $cc ${cflags[*]} $src -o $exe
}

case "$1" in
    "clean")
        rm $exe;;
    *)
        comp;;
esac
