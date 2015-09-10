#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import division, print_function, absolute_import
import argparse
import logging
from astropy.io import fits
from contextlib import contextmanager
import numpy as np
from multiprocessing.dummy import Pool as ThreadPool

logging.basicConfig(level='INFO', format='%(levelname)7s %(message)s')
logger = logging.getLogger(__name__)


class SourceFile(object):
    '''
    Container for catalogue file
    '''

    def __init__(self, hdulist):
        self.hdulist = hdulist
        self.hdu = self.hdulist['apm-binarytable']
        self.data = self.hdu.data
        self.header = self.hdu.header

    @classmethod
    def open_file(cls, filename):
        with fits.open(filename) as hdulist:
            return cls(hdulist)

    def __len__(self):
        return len(self.data)


class Image(SourceFile):
    '''
    Container for source image
    '''

    def __init__(self, hdulist):
        self.hdulist = hdulist
        self.header = hdulist[0].header


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

def get_mjd(fname):
    return fname, fits.getheader(fname)['mjd']

def sort_by_mjd(files):
    pool = ThreadPool()
    mapping = pool.map(get_mjd, files)
    return [row[0] for row in sorted(mapping, key=lambda row: row[1])]


def main(args):
    if args.verbose:
        logger.setLevel('DEBUG')
    logger.debug(args)

    logger.info('Sorting images by mjd')
    sorted_images = sort_by_mjd(args.filename)

    nimages = len(sorted_images)
    first = SourceFile.open_file(sorted_images[0])
    napertures = len(first.data)

    logger.info('{:d} images, {:d} apertures'.format(nimages, napertures))

    logger.debug('Allocating memory for catalogue')
    catalogue_data = np.recarray(napertures,
                                 dtype=[('OBJ_ID', '26a'),
                                        ('RA', np.float64),
                                        ('DEC', np.float64),
                                        ('FLUX_MEAN', np.float64),
                                        ('FLUX_MEDIAN', np.float64),
                                        ('BLEND_FRACTION', np.float32),
                                        ('NPTS', np.int64),])

    logger.debug('Filling catalogue data')
    catalogue_data['OBJ_ID'] = np.array(['NG{:06d}'.format(i)
                                         for i in np.arange(napertures)])
    catalogue_data['NPTS'] = np.ones(napertures) * nimages
    catalogue_data['RA'] = np.degrees(first.data['ra'])
    catalogue_data['DEC'] = np.degrees(first.data['dec'])
    catalogue_data['BLEND_FRACTION'] = np.zeros(napertures, dtype=np.float32)

    logger.debug('Allocating memory for imagelist')
    imagelist_from_header_data_dtype = [('adu_max', np.float32),
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
                                        ('biasover', np.float32),
                                        ('biaspre', np.float32),
                                        ('bkg_mean', np.float32),
                                        ('bkg_rms', np.float32),
                                        ('cameraid', np.int32),
                                        ('campaign', 'a16'),
                                        ('ccdtemp', np.float32),
                                        ('ccdtempx', np.float32),
                                        ('chstemp', np.float32),
                                        ('frame_sn', np.float32),
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
                                        ('vss_usec', np.float32),
                                        ('wcscompl', np.bool),
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
                                        ('xencpos0', np.int64),
                                        ('xencpos1', np.int64),
                                        ('yencpos0', np.int64),
                                        ('yencpos1', np.int64),
                                        ('adu_dev', np.float32),
                                        ('vi_plus', np.int32),
                                        ('vi_minus', np.int32),
                                        ]

    # Add the psf measurements
    for i in range(1, 10):
        for key in ['a', 'b', 't']:
            imagelist_from_header_data_dtype.append(
                    ('psf_{key}_{i}'.format(key=key, i=i), np.float32)
                    )


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
                             ('nsources', np.int64),]

    # Don't error if these keys are missing
    optional_keys = {
        'frame_sn', 'decpos', 'dec_move', 'dec_s', 'rapos', 'ra_move', 'ra_s',
        'fwhm', 'seeing', 'shift', 't', 'wcscompl', 'vi_plus', 'vi_minus',
    }
    full_imagelist_data_type = [
        (key.upper(), typ)
        for (key, typ) in imagelist_from_header_data_dtype + imagelist_extra_dtype
    ]

    imagelist_data = np.recarray(nimages, dtype=full_imagelist_data_type)

    logger.debug('Allocating memory for image HDUs')
    flux_hdu_indexes = list(range(1, 14))
    image = lambda name: FITSImage(name, nimages, napertures)
    image_names = ['HJD', 'FLUX', 'FLUXERR', 'CCDX', 'CCDY', 'SKYBKG']
    image_names.extend(['FLUX_{}'.format(i) for i in flux_hdu_indexes])
    image_names.extend(['ERROR_{}'.format(i) for i in flux_hdu_indexes])
    image_map = {name: image(name) for name in image_names}

    logger.info('Iterating over files')
    for i, filename in enumerate(sorted_images):
        logger.info(filename)
        source = SourceFile.open_file(filename)
        image_filename = filename.replace('.phot', '')
        source_extract = SourceFile.open_file(image_filename.replace('.fits', '.cat'))

        logger.debug('Extracting header data')
        mjd = source.header['mjd']
        imagelist_data['TMID'][i] = mjd
        for (key, typ) in imagelist_from_header_data_dtype:
            if key.lower() in optional_keys:
                imagelist_data[key.upper()][i] = source.header.get(key, 0)
            else:
                imagelist_data[key.upper()][i] = source.header[key]

        # WCS headers that have been renamed
        imagelist_data['CTYPE1'][i] = source.header['TCTYP3']
        imagelist_data['CTYPE2'][i] = source.header['TCTYP6']
        imagelist_data['CRPIX1'][i] = source.header['TCRPX3']
        imagelist_data['CRPIX2'][i] = source.header['TCRPX6']
        imagelist_data['CRVAL1'][i] = source.header['TCRVL3']
        imagelist_data['CRVAL2'][i] = source.header['TCRVL6']
        imagelist_data['CD1_1'][i] = source.header['TC3_3']
        imagelist_data['CD1_2'][i] = source.header['TC3_6']
        imagelist_data['CD2_1'][i] = source.header['TC6_3']
        imagelist_data['CD2_2'][i] = source.header['TC6_6']
        imagelist_data['PV2_1'][i] = source.header['TV6_1']
        imagelist_data['PV2_3'][i] = source.header['TV6_3']
        imagelist_data['PV2_5'][i] = source.header['TV6_5']
        imagelist_data['PV2_7'][i] = source.header['PV2_7']

        imagelist_data['NSOURCES'][i] = len(source_extract)

        logger.debug('Extracting image data')
        image_map['HJD'].set_data(i, mjd + source.data['hjd_correction'])
        image_map['FLUX'].set_data(i, source.data['Aper_flux_3'])
        image_map['FLUXERR'].set_data(i, source.data['Aper_flux_3_err'])
        image_map['CCDX'].set_data(i, source.data['X_coordinate'])
        image_map['CCDY'].set_data(i, source.data['Y_coordinate'])
        image_map['SKYBKG'].set_data(i, source.data['Sky_level'])

        for image_key in flux_hdu_indexes:
            hdu_key = 'FLUX_{}'.format(image_key)
            error_key = 'ERROR_{}'.format(image_key)
            source_flux_key = 'Aper_flux_{}'.format(image_key)
            source_error_key = '{}_err'.format(source_flux_key)
            image_map[hdu_key].set_data(i, source.data[source_flux_key])
            image_map[error_key].set_data(i, source.data[source_error_key])

        del source

    logger.info('Post processing')
    imagelist_data['LOCOUNT'] = np.zeros(nimages)
    imagelist_data['HICOUNT'] = np.zeros(nimages)
    catalogue_data['FLUX_MEAN'] = np.mean(image_map['FLUX'].data, axis=1)
    catalogue_data['FLUX_MEDIAN'] = np.median(image_map['FLUX'].data, axis=1)

    logger.info('Rendering to %s', args.output)
    with create_output_file(args.output) as hdulist:
        hdulist.append(fits.BinTableHDU(imagelist_data, name='IMAGELIST'))
        hdulist.append(fits.BinTableHDU(catalogue_data, name='CATALOGUE'))
        for image in image_map.values():
            hdulist.append(image.hdu())

        if args.sha:
            key_length = 72
            logger.info('Rendering pipeline sha %s', args.sha)
            short_sha = args.sha[:key_length]
            logger.debug('short sha %s', short_sha)
            hdulist[0].header['PIPESHA'] = (short_sha, 'git sha of the pipeline')


if __name__ == '__main__':
    description = '''Extract data from `imcore_list` output'''
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('filename', nargs='+')
    parser.add_argument('-o', '--output', required=True)
    parser.add_argument('-v', '--verbose', action='store_true')
    parser.add_argument('--sha', help='Store pipeline sha',
            required=False)
    main(parser.parse_args())
