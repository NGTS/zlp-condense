#include <iostream>
#include <vector>
#include <map>
#include <valarray>
#include "fits_file.h"

using namespace std;

FITSFile *FITSFile::createFile(const string &filename) {
    FITSFile *f = new FITSFile;
    ;
    fits_create_file(&f->fptr_, filename.c_str(), &f->status_);
    if (f->status_ == FILE_NOT_CREATED) {
        remove(filename.c_str());
        f->status_ = 0;
        fits_create_file(&f->fptr_, filename.c_str(), &f->status_);
    }
    f->check();

    /* Add empty primary */
    fits_write_imghdr(f->fptr_, 8, 0, NULL, &f->status_);
    f->check();
    return f;
}

FITSFile::FITSFile(const string &filename) : FITSFile() {
    fits_open_file(&fptr_, filename.c_str(), READONLY, &status_);
    check();
}

FITSFile::~FITSFile() {
    fits_close_file(fptr_, &status_);
    check();
}

void FITSFile::check() {
    if (status_) {
        fits_report_error(stderr, status_);
        exit(status_);
    }
}

void FITSFile::toHDU(const string &hdu_name) {
    fits_movnam_hdu(fptr_, ANY_HDU, const_cast<char *>(hdu_name.c_str()), 0,
                    &status_);
    check();
}

void FITSFile::toHDU(int hdu_number) {
    fits_movabs_hdu(fptr_, hdu_number, NULL, &status_);
    check();
}

void FITSFile::addImage(const string &name, long nimages, long napertures) {
    long naxes[] = {nimages, napertures};
    fits_create_img(fptr_, DOUBLE_IMG, 2, naxes, &status_);
    check();
    fits_write_key(fptr_, TSTRING, "EXTNAME", (void *)name.c_str(), NULL,
                   &status_);
    check();
}

void FITSFile::addTable(const string &name,
                        const vector<pair<string, int>> &columns, long size) {

    int ncols = columns.size();
    cout << "Creating table " << name << " with " << ncols << " columns"
         << endl;
    valarray<char *> colNames(ncols), colType(ncols);

    for (int i = 0; i < ncols; i++) {
        colNames[i] = (char *)columns[i].first.c_str();
        switch (columns[i].second) {
        case TDOUBLE:
            colType[i] = (char *)"1D";
            break;
        case TINT:
            colType[i] = (char *)"1J";
            break;
        case TLONG:
            colType[i] = (char *)"1J";
            break;
        case TFLOAT:
            colType[i] = (char *)"1E";
            break;
        }

        cout << colNames[i] << " => " << colType[i] << endl;
    }

    fits_create_tbl(fptr_, BINARY_TBL, size, ncols, &colNames[0], &colType[0],
                    NULL, name.c_str(), &status_);
}
