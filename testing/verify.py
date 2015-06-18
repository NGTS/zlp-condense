#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
from astropy.io import fits

def main():
    fname = sys.argv[1]
    imagelist = fits.getdata(fname, 'imagelist')
    column_names = set(key.lower() for key in imagelist.columns.names)

    required_keys = ['psf_a_5', 'psf_b_5']

    for key in required_keys:
        assert key in column_names, 'Cannot find key {}'.format(key)
if __name__ == '__main__':
    main()
