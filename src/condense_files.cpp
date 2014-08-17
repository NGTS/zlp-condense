#include "condense_files.h"

#include <fstream>
#include <iostream>
#include <fitsio.h>

#include "filelist.h"
#include "fits_column.h"

#define RENDER_ARRAY(data, name) \
    fits_create_img(fptr, DOUBLE_IMG, 2, naxes, &status); \
    fits_write_pix(fptr, TDOUBLE, fpixel, _nfiles * _napertures, &data[0], &status); \
    fits_write_key_str(fptr, "EXTNAME", name, NULL, &status);


using namespace std;

FileCondenser::FileCondenser(const string &filelist_name)
: _filelist_name(filelist_name) {
}

void FileCondenser::render(const string &output) {
    load_data();

    long index = 0;
    for (auto fname=_filelist->begin(); fname!=_filelist->end(); fname++) {
        read_file(*fname, index);
        index++;
    }

    fitsfile *fptr;
    int status = 0;
    long naxes[] = {_nfiles, _napertures};
    long fpixel[] = {1, 1};

    fits_create_file(&fptr, output.c_str(), &status);

    // Make a blank first image

    // Now the other hdus
    RENDER_ARRAY(_mjd_arr, "HJD");
    RENDER_ARRAY(_flux_arr, "FLUX");
    RENDER_ARRAY(_skybkg_arr, "SKYBKG");
    RENDER_ARRAY(_ccdx_arr, "CCDX");
    RENDER_ARRAY(_ccdy_arr, "CCDY");

    fits_close_file(fptr, &status);

    if (status) {
        fits_report_error(stderr, status);
        exit(status);
    }

}

void FileCondenser::load_data() {
    _filelist.reset(new FileList(_filelist_name));
    _filelist->parse();

    _nfiles = _filelist->nfiles();
    _napertures = _filelist->napertures();

    _catalogue.reset(new Catalogue(_napertures));
    _imagelist.reset(new Imagelist(_nfiles));

    cout << "Condensing " << _nfiles << " files" << endl;
    cout << "Condensing " << _napertures << " apertures" << endl;

    initialize();
}

void FileCondenser::initialize() {
    _mjd_arr.resize(_nfiles * _napertures);
    _flux_arr.resize(_nfiles * _napertures);
    _skybkg_arr.resize(_nfiles * _napertures);
    _ccdx_arr.resize(_nfiles * _napertures);
    _ccdy_arr.resize(_nfiles * _napertures);
}

void FileCondenser::read_file(const string &fname, long index) {
    fitsfile *fptr;
    int status = 0;

    fits_open_file(&fptr, fname.c_str(), READONLY, &status);
    fits_movrel_hdu(fptr, 1, 0, &status);

    read_image_hdus(fptr, &status, index);
    read_imagelist_row(fptr, &status, index);
    fits_close_file(fptr, &status);

    if (status) {
        fits_report_error(stderr, status);
        exit(status);
    }
}

void FileCondenser::read_image_hdus(fitsfile *fptr, int *status, long index) {
    double mjd = 0;

    FitsColumn flux(_napertures), skybkg(_napertures), ccdx(_napertures), ccdy(_napertures);

    fits_read_key(fptr, TDOUBLE, "mjd", &mjd, NULL, status);

    flux.read(fptr, "core2_flux");
    skybkg.read(fptr, "skylev");
    ccdx.read(fptr, "x_coordinate");
    ccdy.read(fptr, "y_coordinate");

    for (long ap=0; ap<_napertures; ap++) {
        long _write_index = index * _napertures + ap;

        _mjd_arr[_write_index] = mjd;
        flux.write(&_flux_arr[_write_index], ap);
        skybkg.write(&_skybkg_arr[_write_index], ap);
        ccdx.write(&_ccdx_arr[_write_index], ap);
        ccdy.write(&_ccdy_arr[_write_index], ap);

        if (!isnan(flux.at(ap))) {
            _catalogue->mean_flux[ap] += flux.at(ap);
            _catalogue->npts[ap]++;
        }
    }

}

void FileCondenser::read_imagelist_row(fitsfile *fptr, int *status, long index) {
}

void FileCondenser::read_catalogue() {
    for (int i=0; i<_napertures; i++) {
        _catalogue->obj_id[i] = to_string(i);
        _catalogue->mean_flux[i] /= double(_catalogue->npts[i]);
    }
}

void FileCondenser::render_catalogue(fitsfile *fptr, int *status) {
    LONGLONG naxis2 = _napertures;
    int tfields = 3;
    char *ttype[] = {"obj_id", "flux_mean", "npts"};
    char *tform[] = {"S26", "D", "U"};
    char *tunit[] = {"", "counts", ""};

    fits_create_tbl(fptr, BINARY_TBL, naxis2, tfields, ttype, tform, tunit, "CATALOGUE", status);
}

void FileCondenser::render_imagelist(fitsfile *fptr, int *status) {
}

