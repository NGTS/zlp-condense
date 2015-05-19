#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import division, print_function, absolute_import
import argparse
import logging
from astropy.io import fits
from contextlib import contextmanager
import numpy as np

logging.basicConfig(level='INFO', format='%(levelname)7s %(message)s')
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

    catalogue_data = np.recarray(napertures,
                                 dtype=[('OBJ_ID', '26a'),
                                        ('RA', np.float64),
                                        ('DEC', np.float64),
                                        ('FLUX_MEAN', np.float64),
                                        ('FLUX_MEDIAN', np.float64),
                                        ('BLEND_FRACTION', np.float32),
                                        ('NPTS', np.int64),])
    catalogue_data['OBJ_ID'] = np.array(['NG{:06d}'.format(i)
                                         for i in np.arange(napertures)])
    catalogue_data['NPTS'] = np.ones(napertures) * nimages
    catalogue_data['RA'] = np.degrees(first.data['ra'])
    catalogue_data['DEC'] = np.degrees(first.data['dec'])
    catalogue_data['BLEND_FRACTION'] = np.zeros(napertures, dtype=np.float32)

    imagelist_from_header_data_dtype = [('adu_dev', np.float32),
                                        ('adu_max', np.float32),
                                        ('adu_mean', np.float32),
                                        ('adu_med', np.float32),
                                        ('afstatus', '8a'),
                                        ('ag_apply', np.bool),
                                        ('ag_corrx', np.float32),
                                        ('ag_corry', np.float32),
                                        ('ag_deltx', np.float32),
                                        ('ag_delty', np.float32),
                                        ('ag_errx', np.float32),
                                        ('ag_erry', np.float32),
                                        ('agrefimg', np.int64),
                                        ('agstatus', '8a'),
                                        ('airmass', np.float32),
                                        ('biasmean', np.float32),
                                        ('bkg_mean', np.float32),
                                        ('bkg_rms', np.float32),
                                        ('cameraid', np.int32),
                                        ('campaign', 'a16'),
                                        ('ccdtemp', np.float32),
                                        ('ccdtempx', np.float32),
                                        ('chstemp', np.float32),
                                        ('cloud_s', np.float32),
                                        ('clouds', np.float32),
                                        ('cmd_dec', np.float32),
                                        ('cmd_ra', np.float32),
                                        ('crowded', np.int32),
                                        ('cts_dev', np.float32),
                                        ('cts_max', np.float32),
                                        ('cts_mean', np.float32),
                                        ('cts_med', np.float32),
                                        ('dec_move', np.float32),
                                        ('dec_s', np.float32),
                                        ('decpos', np.float32),
                                        ('exposure', np.float32),
                                        ('fcsr_enc', np.float32),
                                        ('fcsr_phy', np.float32),
                                        ('fcsr_tmp', np.float32),
                                        ('field', 'a11'),
                                        ('filtfwhm', np.float32),
                                        ('fldnick', 'a18'),
                                        ('fwhm', np.float32),
                                        ('gain', np.int32),
                                        ('gainfact', np.float32),
                                        ('hss_mhz', np.float32),
                                        ('image_id', np.int64),
                                        ('lst', np.float32),
                                        ('minpix', np.float32),
                                        ('mjd', np.float64),
                                        ('moon_alt', np.float32),
                                        ('moon_az', np.float32),
                                        ('moon_dec', np.float32),
                                        ('moon_ra', np.float32),
                                        ('moondist', np.float32),
                                        ('moonfrac', np.float32),
                                        ('nbsize', np.int32),
                                        ('night', 'a8'),
                                        ('numbrms', np.int64),
                                        ('ra_move', np.float32),
                                        ('ra_s', np.float32),
                                        ('rapos', np.float32),
                                        ('rcore', np.float32),
                                        ('readtime', np.float32),
                                        ('roofstat', 'a9'),
                                        ('seeing', np.float32),
                                        ('shclostm', np.float32),
                                        ('shift', np.float32),
                                        ('shopentm', np.float32),
                                        ('shutmode', 'a8'),
                                        ('sky_move', np.float32),
                                        ('skylevel', np.float32),
                                        ('skynoise', np.float32),
                                        ('stdcrms', np.float32),
                                        ('sun_alt', np.float32),
                                        ('sun_az', np.float32),
                                        ('sun_dec', np.float32),
                                        ('sun_ra', np.float32),
                                        ('sundist', np.float32),
                                        ('t', np.float32),
                                        ('tel_alt', np.float32),
                                        ('tel_az', np.float32),
                                        ('tel_dec', np.float32),
                                        ('tel_ha', np.float32),
                                        ('tel_posa', np.float32),
                                        ('tel_ra', np.float32),
                                        ('threshol', np.float32),
                                        ('wcsf_dec', np.float64),
                                        ('wcsf_ns', np.int64),
                                        ('wcsf_ra', np.float32),
                                        ('wcsf_rms', np.float32),
                                        ('wxdewpnt', np.float32),
                                        ('wxhumid', np.float32),
                                        ('wxpres', np.float32),
                                        ('wxtemp', np.float32),
                                        ('wxwnddir', np.float32),
                                        ('wxwndspd', np.float32),
                                        ('vss_usec', np.float32),
                                        ('wcscompl', np.bool),
                                        ('xencpos0', np.int64),
                                        ('xencpos1', np.int64),
                                        ('yencpos0', np.int64),
                                        ('yencpos1', np.int64),]

    imagelist_extra_dtype = [('cd1_1', np.float64),
                             ('cd1_2', np.float64),
                             ('cd2_1', np.float64),
                             ('cd2_2', np.float64),
                             ('pv2_1', np.float64),
                             ('pv2_3', np.float64),
                             ('pv2_5', np.float64),
                             ('pv2_7', np.float64),
                             ('crpix1', np.float64),
                             ('crpix2', np.float64),
                             ('crval1', np.float64),
                             ('crval2', np.float64),
                             ('ctype1', 'a8'),
                             ('ctype2', 'a8'),
                             ('hicount', np.int64),
                             ('locount', np.int64),
                             ('tmid', np.float64),
                             ('overscan', np.float64),
                             ('overscan_full', np.float64),
                             ('prescan', np.float64),]

    optional_keys = {
        'cloud_s', 'clouds', 'decpos', 'dec_move', 'dec_s', 'rapos', 'ra_move', 'ra_s',
        'fwhm', 'seeing', 'shift', 't', 'wcscompl'
    }
    full_imagelist_data_type = [
        (key.upper(), typ)
        for (key, typ) in imagelist_from_header_data_dtype + imagelist_extra_dtype
    ]

    imagelist_data = np.recarray(nimages, dtype=full_imagelist_data_type)

    image = lambda name: FITSImage(name, nimages, napertures)
    image_names = ['HJD', 'FLUX', 'FLUXERR', 'CCDX', 'CCDY', 'SKYBKG']
    image_names.extend(['FLUX_{}'.format(i + 1) for i in list(range(4))])
    image_names.extend(['ERROR_{}'.format(i + 1) for i in list(range(4))])
    image_map = {name: image(name) for name in image_names}

    for i, filename in enumerate(sorted_images):
        source = SourceFile.open_file(filename)

        # Extract imagelist data
        mjd = source.header['mjd']
        imagelist_data['TMID'][i] = mjd
        for (key, typ) in imagelist_from_header_data_dtype:
            if key.lower() in optional_keys:
                imagelist_data[key.upper()][i] = source.header.get(key)
            else:
                imagelist_data[key.upper()][i] = source.header[key]
        
        # WCS headers that have been renamed
        imagelist_data['CTYPE1'][i] = source.header['TCTYP3']
        imagelist_data['CTYPE2'][i] = source.header['TCTYP3']
        imagelist_data['CRPIX1'][i] = source.header['TCRPX3']
        imagelist_data['CRPIX2'][i] = source.header['TCRPX5']
        imagelist_data['CRVAL1'][i] = source.header['TCRVL3']
        imagelist_data['CRVAL2'][i] = source.header['TCRVL5']
        imagelist_data['CD1_1'][i] = source.header['TC3_3']
        imagelist_data['CD1_2'][i] = source.header['TC3_5']
        imagelist_data['CD2_1'][i] = source.header['TC5_3']
        imagelist_data['CD2_2'][i] = source.header['TC5_5']
        imagelist_data['PV2_1'][i] = source.header['TV5_1']
        imagelist_data['PV2_3'][i] = source.header['TV5_3']
        imagelist_data['PV2_5'][i] = source.header['TV5_5']
        imagelist_data['PV2_7'][i] = source.header['PV2_7']

        # Extract image data
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

        del source

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
