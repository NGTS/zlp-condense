#include "filelist.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <fitsio.h>

using namespace std;

namespace {
    double get_mjd(const string &fname) {
        fitsfile *fptr;
        int status = 0;
        double mjd;

        fits_open_file(&fptr, fname.c_str(), READONLY, &status);
        fits_movrel_hdu(fptr, 1, 0, &status);
        fits_read_key(fptr, TDOUBLE, "mjd", &mjd, NULL, &status);
        fits_close_file(fptr, &status);

        if (status) {
            fits_report_error(stderr, status);
            exit(status);
        }

        return mjd;
    }

    bool mjd_order(const string &a, const string &b) {
        return get_mjd(a) < get_mjd(b);
    }
}

FileList::FileList(const string &fname)
: _fname(fname) {
}

long FileList::nfiles() {
    return _files.size();
}

long FileList::napertures() {
    fitsfile *fptr;
    int status = 0;
    long napertures = 0;
    
    fits_open_file(&fptr, _files[0].c_str(), READONLY, &status);
    fits_movrel_hdu(fptr, 1, 0, &status);
    fits_get_num_rows(fptr, &napertures, &status);
    fits_close_file(fptr, &status);

    if (status) {
        fits_report_error(stderr, status);
        exit(status);
    }

    return napertures;
}

void FileList::parse() {
    ifstream infile(_fname.c_str());
    for (string line; getline(infile, line);) {
        _files.push_back(line);
    }

    sort_filelist();
}

void FileList::sort_filelist() {
    sort(_files.begin(), _files.end(), mjd_order);
}

const string FileList::first() const {
    return _files.at(0);
}
