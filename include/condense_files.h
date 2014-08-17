#ifndef CONDENSE_FILES_H_
#define CONDENSE_FILES_H_

#include <string>
#include <memory>
#include <vector>
#include <fitsio.h>
#include <boost/scoped_ptr.hpp>

#include "filelist.h"

struct Catalogue {
    std::vector<std::string> obj_id;
    std::vector<double> flux_mean;
    std::vector<long> npts;

    Catalogue(long n) :
        obj_id(std::vector<std::string>(n)),
        flux_mean(std::vector<double>(n)),
        npts(std::vector<long>(n)) {}
};

struct Imagelist {
    std::vector<double> airmass;
    std::vector<double> tmid;

    Imagelist(long n) :
        airmass(std::vector<double>(n)),
        tmid(std::vector<double>(n))
    {}
};

class FileCondenser {
    public:
        FileCondenser(const std::string &filelist_name);
        void render(const std::string &output);

    private:
        std::string _filelist_name;
        long _nfiles, _napertures;
        boost::scoped_ptr<FileList> _filelist;

        std::vector<double> _mjd_arr, _flux_arr, _skybkg_arr, _ccdx_arr, _ccdy_arr;
        boost::scoped_ptr<Catalogue> _catalogue;
        boost::scoped_ptr<Imagelist> _imagelist;

        void load_data();
        void read_file(const std::string &fname, long index);
        void read_image_hdus(fitsfile *fptr, int *status, long index);
        void read_imagelist_row(fitsfile *fptr, int *status, long index);
        void read_catalogue();
        void initialize();
        void render_catalogue(fitsfile *fptr, int *status);
        void render_imagelist(fitsfile *fptr, int *status);
};


#endif //  CONDENSE_FILES_H_
