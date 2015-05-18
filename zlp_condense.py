#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import division, print_function, absolute_import
import argparse
import logging
from astropy.io import fits
from contextlib import contextmanager
import numpy as np

logging.basicConfig(
    level='INFO', format='%(levelname)7s %(message)s')
logger = logging.getLogger(__name__)

class SourceFile(object):
    def __init__(self, hdulist):
        self.hdulist = hdulist
        self.hdu = self.hdulist['apm-binarytable']
        self.data = self.hdu.data
        self.header = self.hdu.header


    @classmethod
    def open_file(cls, filename):
        with fits.open(filename) as hdulist:
            return cls(hdulist)

@contextmanager
def create_output_file(filename):
    outfile = fits.HDUList()
    phdu = fits.PrimaryHDU()
    outfile.append(phdu)
    yield outfile
    outfile.writeto(filename, clobber=True)

class FITSImage(object):
    def __init__(self, name, nimages, napertures, data_type=np.float64):
        self.name = name
        self.data = np.zeros((napertures, nimages), dtype=data_type)

    def set_data(self, i, data):
        self.data[:, i] = data

    def hdu(self):
        return fits.ImageHDU(self.data, name=self.name)

def sort_by_mjd(files):
    return sorted(files, key=lambda f: fits.getheader(f)['mjd'])

def main(args):
    if args.verbose:
        logger.setLevel('DEBUG')
    logger.debug(args)

    sorted_images = sort_by_mjd(args.filename)
    nimages = len(sorted_images)
    first = SourceFile.open_file(sorted_images[0])
    napertures = len(first.data)

    print('{:d} images, {:d} apertures'.format(nimages, napertures))

    catalogue_data = np.recarray(napertures, dtype=[
        ('OBJ_ID', 'a26'),
        ('RA', np.float64),
        ('DEC', np.float64),
        ('FLUX_MEAN', np.float64),
        ('FLUX_MEDIAN', np.float64),
        ('BLEND_FRACTION', np.float32),
        ('NPTS', np.int64),
        ])
    catalogue_data['OBJ_ID'] = np.array(['NG{:06d}'.format(i) for i in
        np.arange(napertures)])
    catalogue_data['NPTS'] = np.ones(napertures) * nimages
    catalogue_data['RA'] = np.degrees(first.data['ra'])
    catalogue_data['DEC'] = np.degrees(first.data['dec'])
    catalogue_data['BLEND_FRACTION'] = np.zeros(napertures, dtype=np.float32)

    imagelist_data = np.recarray(nimages, dtype=[
        ('TMID', np.float64),
        ('AIRMASS', np.float32),
        ('IMAGE_ID', np.int64),
        ('EXPOSURE', np.float32),
        ('HICOUNT', np.int64),
        ('LOCOUNT', np.int64),
        ('CHSTEMP', np.float32),
        ('CCDTEMP', np.float32),
        ('TEL_RA', np.float32),
        ('TEL_DEC', np.float32),
        ('MOONDIST', np.float32),
        ('MOONFRAC', np.float32),
        ('WXTEMP', np.float32),
        ('WXPRES', np.float32),
        ('WXWNDSPD', np.float32),
        ('WXWNDDIR', np.float32),
        ('WXHUMID', np.float32),
        ('WXDEWPNT', np.float32),
        ('AG_ERRX', np.float32),
        ('AG_ERRY', np.float32),
        ('AG_CORRX', np.float32),
        ('AG_CORRY', np.float32),
        ('AG_DELTX', np.float32),
        ('AG_DELTY', np.float32),
        ('SKY_LEVEL', np.float32),
        ('SKY_NOISE', np.float32),
        ('NUMBRMS', np.int64),
        ('STDCRMS', np.float32),
        ('WCSF_NS', np.int64),
        ('WCSF_RMS', np.float32),
        ('WCSF_RA', np.float32),
        ('WCSF_DEC', np.float64),
        ])

    image = lambda name: FITSImage(name, nimages, napertures)
    image_names = ['HJD', 'FLUX', 'FLUXERR', 'CCDX', 'CCDY', 'SKYBKG']
    image_names.extend(['FLUX_{}'.format(i + 1) for i in list(range(4))])
    image_names.extend(['ERROR_{}'.format(i + 1) for i in list(range(4))])
    image_map = { name: image(name) for name in image_names }

    for i, filename in enumerate(sorted_images):
        source = SourceFile.open_file(filename)
        mjd = source.header['mjd']
        imagelist_data['TMID'][i] = mjd
        for key in ['airmass', 'image_id', 'exposure', 'chstemp',
                'tel_ra', 'tel_dec', 'moondist', 'moonfrac',
                'wxtemp', 'wxpres', 'wxwndspd', 'wxwnddir',
                'wxdewpnt', 'ag_errx', 'ag_erry', 'ag_corrx', 'ag_corry',
                'ag_deltx', 'ag_delty', 'numbrms', 'stdcrms',
                'wcsf_ns', 'wcsf_rms', 'wcsf_ra', 'wcsf_dec']:
            imagelist_data[key.upper()][i] = source.header[key]

        image_map['HJD'].set_data(i, mjd + source.data['hjd_correction'])
        image_map['FLUX'].set_data(i, source.data['Aper_flux_3'])
        image_map['FLUXERR'].set_data(i, source.data['Aper_flux_3_err'])
        image_map['CCDX'].set_data(i, source.data['X_coordinate'])
        image_map['CCDY'].set_data(i, source.data['Y_coordinate'])
        image_map['SKYBKG'].set_data(i, source.data['Sky_level'])

        for (image_index, image_key) in enumerate([2, 4, 5, 6]):
            hdu_key = 'FLUX_{}'.format(image_index + 1)
            error_key = 'ERROR_{}'.format(image_index + 1)
            source_flux_key = 'Aper_flux_{}'.format(image_key)
            source_error_key = '{}_err'.format(source_flux_key)
            image_map[hdu_key].set_data(i, source.data[source_flux_key])
            image_map[error_key].set_data(i, source.data[source_error_key])

    imagelist_data['LOCOUNT'] = np.zeros(nimages)
    imagelist_data['HICOUNT'] = np.zeros(nimages)
    catalogue_data['FLUX_MEAN'] = np.mean(image_map['FLUX'].data, axis=1)
    catalogue_data['FLUX_MEDIAN'] = np.median(image_map['FLUX'].data, axis=1)

    with create_output_file(args.output) as hdulist:
        hdulist.append(fits.BinTableHDU(imagelist_data, name='IMAGELIST'))
        hdulist.append(fits.BinTableHDU(catalogue_data, name='CATALOGUE'))
        for image in image_map.values():
            hdulist.append(image.hdu())


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', nargs='+')
    parser.add_argument('-o', '--output', required=True)
    parser.add_argument('-v', '--verbose', action='store_true')
    main(parser.parse_args())
