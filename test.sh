#!/usr/bin/env bash

set -e

setup() {
    echo "Setup"
    test -f testdata/output.fits && rm testdata/output.fits || true
}

compile() {
    echo "Compile"
    make
}

run() {
    echo "Run"
    /usr/bin/time bin/condense testdata/filelist.txt -o testdata/output.fits
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
