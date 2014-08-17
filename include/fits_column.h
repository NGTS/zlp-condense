#ifndef FITS_COLUMN_H_
#define FITS_COLUMN_H_

#include <string>
#include <vector>
#include <fitsio.h>

class FitsColumn {
    public:
        FitsColumn(int n);
        void read(fitsfile *fptr, const std::string &colname);
        void write(double *element, long index);

    private:
        int _colnum;
        std::vector<double> _data;
        int _size;

};
#endif //  FITS_COLUMN_H_

