#include "filelist.h"

#include <fstream>
#include <iostream>
#include <fitsio.h>

using namespace std;

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
    ifstream infile(_fname);
    for (string line; getline(infile, line);) {
        _files.push_back(line);
    }
}
