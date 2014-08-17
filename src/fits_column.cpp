#include "fits_column.h"

using namespace std;

FitsColumn::FitsColumn(int n) : _size(n) {
    _data.resize(n);
}

void FitsColumn::read(fitsfile *fptr, const string &colname) {
    int status = 0;
    fits_get_colnum(fptr, CASEINSEN, (char*)colname.c_str(), &_colnum, &status);
    fits_read_col(fptr, TDOUBLE, _colnum, 1, 1, _size, NULL, &_data[0],
            NULL, &status);

    if (status) {
        fits_report_error(stderr, status);
        exit(status);
    }

}

void FitsColumn::write(double *element, long index) {
    *element = _data[index];
}

