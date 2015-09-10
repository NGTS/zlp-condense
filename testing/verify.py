#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import division, print_function, absolute_import

import sys
from astropy.io import fits
import os
import inspect
from functools import wraps

def test(fn):
    @wraps(fn)
    def inner(*args, **kwargs):
        print('\t', fn.func_code.co_name, end='')
        fn(*args, **kwargs)
        print('\t\tok')
    return inner


@test
def assert_psf_keys_present(fname):
    imagelist = fits.getdata(fname, 'imagelist')
    column_names = set(key.lower() for key in imagelist.columns.names)

    required_keys = ['psf_a_5', 'psf_b_5']

    for key in required_keys:
        assert key in column_names, 'Cannot find key {}'.format(key)


@test
def assert_pipeline_sha_present(fname):
    header = fits.getheader(fname)
    sha = header['pipesha']
    expected = os.environ['TESTSHA']
    assert sha.strip() == expected.strip(), '{} != {}'.format(sha, expected)


@test
def assert_voltages_present(fname):
    imagelist = fits.getdata(fname, 'imagelist')
    column_names = set(key.lower() for key in imagelist.columns.names)

    required_keys = ['vi_plus', 'vi_minus']

    for key in required_keys:
        assert key in column_names, 'Cannot find key {}'.format(key)

@test
def assert_flux_radii_present(fname):
    n_apertures = 13

    with fits.open(fname) as infile:
        for index in range(1, n_apertures + 1):
            flux_key = 'FLUX_{}'.format(index)
            error_key = 'ERROR_{}'.format(index)

            assert infile[flux_key].header['RADIUS']
            assert infile[error_key].header['RADIUS']


def main():
    fname = sys.argv[1]

    assert_psf_keys_present(fname)
    assert_pipeline_sha_present(fname)
    assert_voltages_present(fname)
    assert_flux_radii_present(fname)


if __name__ == '__main__':
    main()
