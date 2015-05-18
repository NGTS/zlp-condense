#include <iostream>
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
    if (fptr_) {
        fits_close_file(fptr_, &status_);
        check();
    }
}

void FITSFile::check() {
    if (status_) {
        fits_report_error(stderr, status_);
        exit(status_);
    }
}

void FITSFile::toHDU(const string &hdu_name) {
    fits_movnam_hdu(fptr_, BINARY_TBL, const_cast<char *>(hdu_name.c_str()), 0,
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
    fits_write_key(fptr_, TSTRING, "EXTNAME", (void*)name.c_str(), NULL, &status_);
    check();
}
