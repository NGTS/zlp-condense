#!/usr/bin/env bash

setup() {
    echo "Setup"
    test -f testdata/output.fits && rm testdata/output.fits
}

compile() {
    echo "Compile"
    make
}

run() {
    echo "Run"
    gdb --args bin/condense testdata/filelist.txt -o testdata/output.fits
}

teardown() {
    echo "Teardown"
    true
}

main() {
    setup
    compile
    run
    teardown
}

main
