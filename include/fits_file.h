#ifndef FITS_FILE_H

#define FITS_FILE_H

#include <string>
#include <fitsio.h>

struct FITSFile {
    fitsfile *fptr_;
    int status_;

    FITSFile(fitsfile *fptr) : fptr_(fptr), status_(0) {}
    FITSFile() : FITSFile(NULL) {}
    FITSFile(const std::string &filename);
    ~FITSFile(); 

    static FITSFile createFile(const std::string &filename);

    void check();
};


#endif /* end of include guard: FITS_FILE_H */
