#include "condense_files.h"

#include <fstream>
#include <iostream>
#include <fitsio.h>

#include "filelist.h"

using namespace std;

namespace {
    struct FitsColumn {
        int _colnum;
        vector<double> _data;
        int _size;

        FitsColumn(int n) : _size(n) {
            _data.resize(n);
        }

        void read(fitsfile *fptr, const string &colname) {
            int status = 0;
            fits_get_colnum(fptr, CASEINSEN, (char*)colname.c_str(), &_colnum, &status);
            fits_read_col(fptr, TDOUBLE, _colnum, 1, 1, _size, NULL, &_data[0],
                    NULL, &status);

            if (status) {
                fits_report_error(stderr, status);
                exit(status);
            }

        }
    };
}


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

    return;
}

void FileCondenser::load_data() {
    _filelist.reset(new FileList(_filelist_name));
    _filelist->parse();

    _nfiles = _filelist->nfiles();
    _napertures = _filelist->napertures();

    cout << "Condensing " << _nfiles << " files" << endl;
    cout << "Condensing " << _napertures << " apertures" << endl;

    initialize();
}

void FileCondenser::initialize() {
    _mjd_arr.resize(_nfiles * _napertures);
    _flux_arr.resize(_nfiles * _napertures);
}

void FileCondenser::read_file(const string &fname, long index) {
    fitsfile *fptr;
    int status = 0;
    double mjd = 0;
    FitsColumn flux(_napertures);

    fits_open_file(&fptr, fname.c_str(), READONLY, &status);
    fits_movrel_hdu(fptr, 1, 0, &status);

    fits_read_key(fptr, TDOUBLE, "mjd", &mjd, NULL, &status);

    flux.read(fptr, "core2_flux");

    for (long ap=0; ap<_napertures; ap++) {
        long _write_index = index * _napertures + ap;

        _mjd_arr[_write_index] = mjd;
        _flux_arr[_write_index] = flux._data[ap];
    }

    fits_close_file(fptr, &status);

    if (status) {
        fits_report_error(stderr, status);
        exit(status);
    }

}
