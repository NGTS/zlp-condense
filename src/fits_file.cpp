#include "fits_file.h"

using namespace std;

FITSFile* FITSFile::createFile(const string &filename) {
    FITSFile *f = new FITSFile;;
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

FITSFile::FITSFile(const string &filename) {
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
