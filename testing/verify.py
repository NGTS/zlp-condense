#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import division, print_function, absolute_import

import sys
from astropy.io import fits
import os
import inspect


def assert_psf_keys_present(fname):
    print('\t', inspect.currentframe().f_code.co_name)
    imagelist = fits.getdata(fname, 'imagelist')
    column_names = set(key.lower() for key in imagelist.columns.names)

    required_keys = ['psf_a_5', 'psf_b_5']

    for key in required_keys:
        assert key in column_names, 'Cannot find key {}'.format(key)


def assert_pipeline_sha_present(fname):
    print('\t', inspect.currentframe().f_code.co_name)
    header = fits.getheader(fname)
    sha = header['pipesha']
    expected = os.environ['TESTSHA']
    assert sha.strip() == expected.strip(), '{} != {}'.format(sha, expected)


def assert_voltages_present(fname):
    print('\t', inspect.currentframe().f_code.co_name)

    imagelist = fits.getdata(fname, 'imagelist')
    column_names = set(key.lower() for key in imagelist.columns.names)

    required_keys = ['vi_plus', 'vi_minus']

    for key in required_keys:
        assert key in column_names, 'Cannot find key {}'.format(key)


def main():
    fname = sys.argv[1]

    assert_psf_keys_present(fname)
    assert_pipeline_sha_present(fname)
    assert_voltages_present(fname)


if __name__ == '__main__':
    main()
