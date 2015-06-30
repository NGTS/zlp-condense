#!/usr/bin/env sh

set -e

FNAME=out.fits
export TESTSHA=testsha

main() {
    echo "Running main script"
    ./zlp_condense.py -o ${FNAME} $(find ../zlp-script/testdata/Reduction/output/ZLPTest/ZLPTest_image_NG0953-4538/ -name '*.phot') --sha $TESTSHA -v
    echo "Verifying"
    verify
}

verify() {
    python testing/verify.py ${FNAME}
}

main
