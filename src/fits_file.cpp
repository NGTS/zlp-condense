#include "fits_file.h"

using namespace std;

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
