#!/usr/bin/env sh

set -e

FNAME=out.fits
export TESTSHA=testsha

standard_test() {
    echo "Running main script"
    ./zlp_condense.py -o ${FNAME} $(find testdata/standard -name '*.phot') --sha $TESTSHA -v
    echo "Verifying"
    verify
}

without_voltages_test() {
    echo "Running main script"
    ./zlp_condense.py -o ${FNAME} $(find testdata/missing_voltage_key -name '*.phot') --sha $TESTSHA -v
    echo "Verifying"
    verify
}

main() {
    standard_test
    without_voltages_test
}

verify() {
    python testing/verify.py ${FNAME}
}

main
