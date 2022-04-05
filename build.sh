#!/bin/bash

exe=xc
src=src/*.c
cc=gcc
inc=-I.
ldir=-Llib

libs=(
    -lutopia 
    -lxstring
)

flags=(
    -std=c99
    -Wall 
    -Wextra
    -O2
)

buildlib() {
    pushd $1 && ./build.sh $2 && mv *.a ../lib/ && popd
}

build() {
    [ ! -d lib ] && mkdir lib
    buildlib utopia -slib
    buildlib xstring static
}

comp() {
    $cc ${flags[*]} $inc $ldir ${libs[*]} $src -o $exe
}

clean() {
    [ -d lib ] && rm -r lib && echo "Deleted 'lib'."
    [ -f $exe ] && rm $exe && echo "Deleted '$exe'."
    return 0
}

case "$1" in
    "build")
        build;;
    "comp")
        comp;;
    "clean")
        clean;;
    *)
        echo "Use with 'build', 'comp' or 'clean'";;
esac
